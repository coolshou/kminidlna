
#include "restserver.h"
#include <QString>
#include <QSslError>
#include <QFile>
#include <QStringList>
#include <QHash>
#include <QFileInfo>
#include <KConfigGroup>
#include <KGlobal>
#include "serverrequest.h"

#include <QDomDocument>
#include <sys/stat.h>
#include <KConfig>

// bool RESTServer::m_runing = false;

RESTServer::RESTServer(QObject* parent) : QTcpServer(parent), m_port(Server::DEFAULT_PORT), m_runing(false)
{
    m_certPath = "cert/server.crt";
    QFile fkey("cert/server.key");
    fkey.open(QIODevice::ReadOnly | QIODevice::Text);
    m_key = new QSslKey(&fkey, QSsl::Rsa);
    fkey.close();

    loadConfig();

    loadResource();
    addInterfaces(this);
}

RESTServer::~RESTServer() {

}

RESTServer* RESTServer::getInstance()
{
    static RESTServer server;
    return &server;
}

void RESTServer::loadConfig()
{
    m_notFoundFileHtmlPath = "404.html";

    KConfigGroup config = KGlobal::config()->group("server");
    setPort(config.readEntry("port", Server::DEFAULT_PORT));

    QString username = config.readEntry("username", Server::DEFAULT_PASSWORD);
    QString decodedpassword = Server::DEFAULT_PASSWORD;
    QByteArray pass = QByteArray::fromBase64(config.readEntry("password", decodedpassword.toUtf8().toBase64()));
    username.append(":").append(pass);
    m_loginpass = username.toUtf8().toBase64();
}

/**
 * Start server if server not run
 * emit run signal
 */
void RESTServer::startServer()
{
    if (!m_runing) {
        m_runing = listen(QHostAddress::Any, m_port);
        qDebug() << "RESTServer: starting server";
        emit run(m_runing);
    }
}

/**
 * Stop server if server run
 * emit run signal
 */
void RESTServer::stopServer()
{
    if (m_runing) {
        close();
        qDebug() << "RESTServer: stoping serve";
        m_runing = false;
        emit run(m_runing);
    }
}




/**
 * Incoming connection on server
 * TODO if not exist certification
 */
void RESTServer::incomingConnection(int socketDescriptor) {
    nextPendingConnection();
    QSslSocket* socket = new QSslSocket(this);
    

    //DBG
//     qDebug() << "RESTServer: Connection incoming "<< socketDescriptor;
    //End DBG

    if (socket->setSocketDescriptor(socketDescriptor)) {
        if (m_cert == 0) {
            socket->setLocalCertificate(m_certPath);
            m_cert = socket->localCertificate();
        } else {
            socket->setLocalCertificate(m_cert);
        }

        socket->setPrivateKey(*m_key);
        connect(socket, SIGNAL(encrypted()), this, SLOT(handshakeComplete()));
        connect(socket, SIGNAL(sslErrors(QList<QSslError>)), SLOT(errorsSSL(QList<QSslError>)));
        socket->startServerEncryption();
    } else {
        delete socket;
    }

}

void RESTServer::connectionClosed()
{
    QSslSocket* socket = dynamic_cast<QSslSocket *>(sender());
    if (socket != 0) {
        socket->deleteLater();
    }
}

void RESTServer::handshakeComplete()
{
    QSslSocket* socket = dynamic_cast<QSslSocket *>(sender());
    connect(socket, SIGNAL(disconnected()), this, SLOT(connectionClosed()));
    connect(socket, SIGNAL(readyRead()), this, SLOT(receiveData()));
}

void RESTServer::receiveData()
{
    QSslSocket* socket = dynamic_cast<QSslSocket *>(sender());

    if (socket->canReadLine()) {
        ServerRequest* req = receiveRequestHeader(socket);

        if (req->isAuthorized(m_loginpass)) {
            switch (req->method()) {
            case ServerRequest::GET:
                processGETAndSendReply(socket, req);
                break;
            case ServerRequest::PUT:
                processPUTAndSendReply(socket, req);
            case ServerRequest::POST:
                //TODO send not implemented
                break;
            case ServerRequest::NONE:
                //TODO send not implemented
                break;
            }
        } else {
            socket->readAll();
            QByteArray ok = "HTTP/1.1 401 Unauthorized\r\n"
                            "WWW-Authenticate: Basic realm=\"authentication is needed\"\r\n"
                            "\r\n";
            socket->write(ok);
            socket->waitForBytesWritten();
//             socket->waitForReadyRead();
        }
	socket->waitForBytesWritten();
        socket->close();
        delete req;
    }
}


void RESTServer::sendNoContent(QTcpSocket* socket)
{
    sendMSG(socket, "204 No Content");
}

void RESTServer::sendMSG(QTcpSocket* socket, QString msg)
{

    QByteArray send = "HTTP/1.1 "+msg.toUtf8()+"\r\n"
                      "\r\n";
    if (socket->isWritable()) {
        socket->write(send);
    } else {
        qDebug() << "RESTServer: cannot send msg:"+msg+" socket is not writable";
    }
}

ServerRequest* RESTServer::receiveRequestHeader(QTcpSocket* socket)
{
    ServerRequest* req = new ServerRequest();

    if (socket->canReadLine()) {
        bool isSet = req->setFirstLine(socket->readLine());//set first line
        if (isSet) {
            for (int numLine =0 ;numLine< MAXHEADERLINES; ++numLine) {
                if (socket->canReadLine()) {
                    QByteArray tmp = socket->readLine();

                    if (!req->insertRawHeaderLine(tmp)) {
                        break;
                    }
                } else {
                    break;
                }
            }
        }
    }
    return req;
}

void RESTServer::processPUTAndSendReply(QTcpSocket* socket, ServerRequest* req)
{
    QString address = req->path();
    RESTInterfaces* interface = 0;

    QList<RESTInterfaces* >::const_iterator it;
    for (it = m_intefaces.begin(); it != m_intefaces.end(); ++it) {
        if ((*it)->hasResourceOnAddress(address)) {//items are pointers
            qDebug() << "RESTServer::processPUTAndSendReply: " << (*it)->adresses();
            interface = *it;
            break;
        }
    }
    if (interface == 0) {
        sendNoContent(socket);
        return;
    }
    //TODO check if file is xml
    RESTresource* res = interface->resourceOnAddress(address);
    req->setContent(socket->readAll());
    qDebug() << *(req->content());
    QDomDocument* doc = new QDomDocument();
    if (doc->setContent(*(req->content()))) {
        if (res->isValidResource(doc)) {
            if (res->setResource(doc)) {
                sendMSG(socket, "200 Ok");
            } else {
                sendNoContent(socket); //FIXME send right code
            }
        }
    }

    if (doc != 0) {
        delete doc;
    }
}


void RESTServer::processGETAndSendReply(QTcpSocket* socket, const ServerRequest* req)
{
    QString address = req->path();
    RESTInterfaces* interface = 0;

    QList<RESTInterfaces* >::const_iterator it;
    for (it = m_intefaces.begin(); it != m_intefaces.end(); ++it) {
        if ((*it)->hasResourceOnAddress(address)) {//items are pointers
            qDebug() << "RESTServer::processGETAndSendReply: " << (*it)->adresses();
            interface = *it;
            break;
        }
    }

    if (interface == 0) {
        send404NotFound(socket);
        return;
    }

    if (socket->isWritable()) {
        QByteArray ok = "HTTP/1.1 200 Ok\r\n"
                        "Content-Type: text/xml; charset=\"utf-8\"\r\n"
                        "\r\n";
        socket->write(ok);
        RESTresource* res = interface->resourceOnAddress(address);
        socket->write(res->resource()->toByteArray());
        socket->waitForBytesWritten();
    }
}

void RESTServer::send404NotFound(QTcpSocket* socket)
{
//     qDebug() << "RESTServer: File not found";
    if (socket->isWritable()) {
        QByteArray nof = "HTTP/1.1 404 Not Found\r\n"
                         "Content-Type: text/html; charset=\"utf-8\"\r\n"
                         "\r\n";
        socket->write(nof);
        QFileInfo infonof(m_notFoundFileHtmlPath);
        if (infonof.isFile() && infonof.isReadable()) {
            QFile nof404(m_notFoundFileHtmlPath);
            if (nof404.open(QIODevice::ReadOnly)) {
                socket->write(nof404.readAll());
            }
        } else {
            QByteArray html = "<html>\n"
                              "<head><title>404 Not Found</title></head>\n"
                              "\t<body><h1>Not Found</h1>The requested document was not found on this server.</body>\n"
                              "</html>\n";
            socket->write(html);
        }
        socket->waitForBytesWritten();
    }
}

void RESTServer::addInterfaces(RESTInterfaces* interface)
{
    m_intefaces.append(interface);
}

bool RESTServer::removeInterfaces(RESTInterfaces* interface)
{

    if (m_intefaces.contains(interface)) {
        return m_intefaces.removeOne(interface);
    }
    return false;
}

void RESTServer::loadResource()
{
    //Version.xml part
    BasicRESTResource* res = new BasicRESTResource("/version.xml", this);

    QByteArray versionxml = "<kminidlna><version>0.2</version></kminidlna>";

    if (res->setXML(versionxml)) {
        addResource(res);
    }

}

void RESTServer::errorsSSL(QList< QSslError > errs)
{
    foreach(QSslError er, errs) {
        qDebug() << "RESTServer SSL error:" << er.errorString();
    }
}


