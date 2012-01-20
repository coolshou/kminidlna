
#include "restserver.h"
#include <QString>
#include <QSslError>
#include <QFile>
#include <QStringList>
#include <QHash>
#include <QFileInfo>
#include "serverrequest.h"

#include <QDomDocument>
#include <sys/stat.h>

bool RESTServer::m_runing = false;

RESTServer::RESTServer(QObject* parent) : QTcpServer(parent), m_port(Server::DEFAULT_PORT) {
    m_certPath = "cert/server.crt";
    QFile fkey("cert/server.key");
    fkey.open(QIODevice::ReadOnly | QIODevice::Text);
    m_key = new QSslKey(&fkey, QSsl::Rsa);
    fkey.close();

    m_notFoundFileHtmlPath = "404.html";
    //DBG
//     QByteArray xmlskeleton = "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>"
//                              "<minidlna><state>ex</state></minidlna>";
//     QDomDocument m_document;
//     if (m_document.setContent(xmlskeleton)) {
// 	QDomElement root = m_document.documentElement();
// 	QDomElement state = root.firstChildElement("state");
// 	QDomElement newState = state;
// 	QDomText old = state.firstChild().toText();
// 	QDomText newText = old;
// 	newText.setData("notrun");
// 	QDomElement ch = m_document.documentElement().firstChild().toElement();
	
	

// 	state.replaceChild(newText, old);
	
// 	state.appendChild(text);
// 	m_stateElement = m_document->replaceChild(state, m_stateElement);
//         qDebug() << state.tagName() << m_document.toByteArray() << ch.tagName();
//     }
    //End DBG
}

RESTServer* RESTServer::getInstance()
{
    static RESTServer server;
    return &server;
}


/**
 * Incoming connection on server
 * TODO if not exist certification
 */
void RESTServer::incomingConnection(int socketDescriptor) {
    nextPendingConnection();
    QSslSocket* socket = new QSslSocket(this);//TODO add socket to list for delete

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
        //DBG
//         QMap<QSsl::AlternateNameEntryType, QString> a=socket->localCertificate().alternateSubjectNames();
//         QMapIterator<QSsl::AlternateNameEntryType, QString> it(a);
//         while (it.hasNext()) {
//             qDebug() << it.next().value();
//         }
        //End DBG

        socket->setPrivateKey(*m_key);
        connect(socket, SIGNAL(encrypted()), this, SLOT(handshakeComplete()));
        connect(socket, SIGNAL(sslErrors(QList<QSslError>)), SLOT(errorySSL(QList<QSslError>)));
        socket->startServerEncryption();
    } else {
        delete socket;
    }

}


RESTServer::~RESTServer() {

}

void RESTServer::connectionClosed()
{
    QSslSocket* socket = dynamic_cast<QSslSocket *>(sender());
    if (socket != 0) {
        disconnect(socket, SIGNAL(disconnected()), this, SLOT(connectionClosed()));
        disconnect(socket, SIGNAL(readyRead()), this, SLOT(receiveData()));
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

        //DBG
        QByteArray pass = "tomas:superman";
        pass = pass.toBase64();
        //End DBG

        if (req->isAuthorized(pass)) {
            switch (req->method()) {
            case ServerRequest::GET:
                processGETAndSendReply(socket, req);
                break;
            case ServerRequest::PUT:
                sendOnPUTReply(socket, req);
            case ServerRequest::POST:
                //TODO send not implemented
                break;
            case ServerRequest::NONE:
                //TODO send not implemented
                break;
            }
        } else {

            //TODO refactoring
            socket->readAll();
            QByteArray ok = "HTTP/1.1 401 Unauthorized\r\n"
                            "WWW-Authenticate: Basic realm=\"authentication is needed\"\r\n"
                            "\r\n";
            socket->write(ok);
            socket->waitForBytesWritten();
	    socket->waitForReadyRead();
        }
        
	socket->close();
        delete req;
    }
}


void RESTServer::sendNoContent(QSslSocket* socket)
{
    sendMSG(socket, "204 No Content");
}

void RESTServer::sendMSG(QSslSocket* socket, QString msg)
{

    QByteArray send = "HTTP/1.1 "+msg.toUtf8()+"\r\n"
                      "\r\n";
    if (socket->isWritable()) {
        socket->write(send);
    } else {
        qDebug() << "RESTServer: cannot send msg:"+msg+" socket is not writable";
    }
}


void RESTServer::errorySSL(QList< QSslError > errs)
{
    foreach(QSslError er, errs) {
        qDebug() << "RESTServer SSL error:" << er.errorString();
    }
}

ServerRequest* RESTServer::receiveRequestHeader(QTcpSocket* socket)
{
    ServerRequest* req = new ServerRequest();

    if (socket->canReadLine()) {
        bool isSet = req->setFirstLine(socket->readLine());//set first line
        if (isSet) {
            while (socket->canReadLine()) {

                QByteArray tmp = socket->readLine();

                if (!req->insertRawHeaderLine(tmp)) {
                    break;
                }

            }
        }
    }
    return req;
}

void RESTServer::receivedPUT(QSslSocket* socket, const QStringList& firstLine, const QHash< QString, QStringList >& header)
{
//     while (socket->canReadLine()) {
//         QString tmp = socket->readLine();
//         qDebug() << tmp;
//         if (tmp == "\r\n") {//testovani konce hlavicky
//             break;
//         }
//     }
// 
//     qDebug() << "jsem v PUT";
//     if (firstLine[1].endsWith(".xml")) { //je to soubor s priponou xml
//         //Vytvoreni souboru
//         qDebug() << "vytvarim sob";
//         QFileInfo info("xml/"+firstLine[1]);
//         if (info.exists()) {//soubor existuje
//             QFile file(info.filePath());
//             if (file.open(QIODevice::WriteOnly)) {
//                 file.write(socket->readAll());
//                 file.flush();
//                 file.close();
//                 sendMSG(socket, "200 Ok");
//             } else {
//                 sendNoContent(socket);
//             }
//         } else {//pokusim se ho vytvorit
//             qDebug() << "jeste jednou vytv";
//             QFile file(info.filePath());
//             if (file.open(QIODevice::WriteOnly)) {
//                 QByteArray v = socket->readAll();
//                 qDebug() << v;
//                 file.write(v);
//                 file.flush();
//                 file.close();
//                 sendMSG(socket, "201 Created");
//                 //TODO poslat jeste adresu souboru
// 
//             } else {
//                 sendNoContent(socket);
//             }
//         }
//     } else {
        sendNoContent(socket);
//     }
}

void RESTServer::sendOnPUTReply(QTcpSocket* socket, const ServerRequest* req)
{

}


void RESTServer::processGETAndSendReply(QTcpSocket* socket, const ServerRequest* req)
{
    QString address = req->path();
    RESTInterfaces* interface = 0;
    //TODO check if path exists on registred interfaces
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
            QByteArray html = "<HTML>\n"
                              "<HEAD><TITLE>404 Not Found</TITLE></HEAD>\n"
                              "<BODY><H1>Not Found</H1>The requested document was not found on this server.</BODY>\n"
                              "</HTML>\n";
            socket->write(html);
        }
        socket->waitForBytesWritten();
    }
}


bool RESTServer::isRuning()
{
    return m_runing;
}

void RESTServer::start()
{
    m_runing = listen(QHostAddress::Any, m_port);
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

