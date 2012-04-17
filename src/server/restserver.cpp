
// #include "restserver.h"
#include <QString>
#include <QSslError>
#include <QFile>
#include <QStringList>
#include <QHash>
#include <QFileInfo>
#include <KConfigGroup>
#include <KGlobal>
#include "serverrequest.h"
#include "restconnection.h"

#include <QDomDocument>
#include <sys/stat.h>
#include <KConfig>

// bool RESTServer::m_runing = false;

QByteArray RESTServer::login = QByteArray();
QByteArray RESTServer::password = QByteArray();
const int RESTServer::MAX_NUMBER_OF_PORT = 65000;
const int RESTServer::DEFAULT_PORT = 8080;

RESTServer::RESTServer(QObject* parent) : QTcpServer(parent), m_port(DEFAULT_PORT), m_runing(false) {
    m_certPath = "cert/server.crt";
    QFile fkey("cert/server.key");
    fkey.open(QIODevice::ReadOnly | QIODevice::Text);
    setSslKey(new QSslKey(&fkey, QSsl::Rsa));
    fkey.close();

    loadConfig();

    loadResource();
    addInterfaces(this);
}

RESTServer::~RESTServer() {

}

RESTServer* RESTServer::getInstance() {
    static RESTServer server;
    return &server;
}

void RESTServer::loadConfig() {
    m_notFoundFileHtmlPath = "404.html";

    KConfigGroup config = KGlobal::config()->group("server");
    setPort(config.readEntry("port", DEFAULT_PORT));

    RESTServer::login = config.readEntry("username", QByteArray());
    RESTServer::password = config.readEntry("password", QByteArray());

//     m_loginpass = username.toUtf8().toBase64();
}

/**
 * Start server if server not run
 * emit run signal
 */
void RESTServer::startServer() {
    if(!m_runing) {
        m_runing = listen(QHostAddress::Any, m_port);
        qDebug() << "RESTServer: starting server";
        qDebug() << errorString();
        emit run(m_runing);
    }
}

/**
 * Stop server if server run
 * emit run signal
 */
void RESTServer::stopServer() {
    if(m_runing) {
        close();
        qDebug() << "RESTServer: stoping server";
        m_runing = false;
        emit run(m_runing);
    }
}




/**
 * Incoming connection on server
 * TODO if not exist certification
 */
void RESTServer::incomingConnection(int socketDescriptor) {
//     nextPendingConnection();
    QSslSocket* socket = new QSslSocket(this);
    //DBG
//     qDebug() << "RESTServer: Connection incoming "<< socketDescriptor;
    //End DBG

    if(socket->setSocketDescriptor(socketDescriptor)) {
        if(m_cert == 0) {
            socket->setLocalCertificate(m_certPath);
            m_cert = socket->localCertificate();
        } else {
            socket->setLocalCertificate(m_cert);
        }

        socket->setPrivateKey(*m_key);
        connect(socket, SIGNAL(encrypted()), this, SLOT(handshakeComplete()));
        connect(socket, SIGNAL(sslErrors(QList<QSslError>)), SLOT(errorsSSL(QList<QSslError>)));
        connect(socket, SIGNAL(disconnected()), SLOT(connectionClosed()));
        socket->startServerEncryption();
    } else {
        socket->deleteLater();
    }

}

void RESTServer::connectionClosed() {
    QSslSocket* socket = dynamic_cast<QSslSocket *>(sender());
    qDebug() << "Closed: " << socket->socketDescriptor() << " Reason: "<< socket->error() << socket->errorString() ;
    socket->deleteLater();
}

void RESTServer::handshakeComplete() {
    QSslSocket* socket = dynamic_cast<QSslSocket *>(sender());
    disconnect(socket, SIGNAL(disconnected()), this, SLOT(connectionClosed()));

    RESTConnection* connection = new RESTConnection(socket, this);
    connect(connection, SIGNAL(finished()), connection, SLOT(deleteLater()));
    connection->start();
}


/**
 * INTERFACE PART
 */

/**
 * write lock thread
 */
void RESTServer::addInterfaces(RESTInterfaces* interface) {
    QWriteLocker l(&m_lock);
    m_intefaces.append(interface);
}

/**
 * write lock thread
 */
bool RESTServer::removeInterfaces(RESTInterfaces* interface) {
    QWriteLocker l(&m_lock);
    if(m_intefaces.contains(interface)) {
        return m_intefaces.removeOne(interface);
    }
    return false;
}

/**
 * write lock thread
 */
void RESTServer::loadResource() {
    QWriteLocker l(&m_lock);
    //Version.xml part
    res = new BasicRESTResource("/version.xml", this);

    QByteArray versionxml = "<kminidlna><version>0.2</version></kminidlna>";

    if(res->setXML(versionxml)) {
        addResource(res);
    }
}

/**
 * read lock thread
 * @return resource on address if resource not exist return 0
 */
RESTresource* RESTServer::resource(QString address) {
    QReadLocker l(&m_lock);
    QList<RESTInterfaces* >::const_iterator it;
    for(it = m_intefaces.begin(); it != m_intefaces.end(); ++it) {
        if((*it)->hasResourceOnAddress(address)) { //items are pointers
            return (*it)->resourceOnAddress(address);
        }
    }
    return 0;
}


void RESTServer::errorsSSL(QList< QSslError > errs) {
    foreach(QSslError er, errs) {
        if(er != QSslError::SelfSignedCertificate) {
            qDebug() << "RESTServer SSL error:" << er.errorString();
        }
    }
}


void RESTServer::setCert(const QSslCertificate& cert) {
    m_cert = cert;
}

void RESTServer::setSslKey(QSslKey* key) {
    m_key = QSharedPointer<QSslKey>(key);
}

/**
 * plainPassword - password in plain not crypted mode
 * set password. 
 */
void RESTServer::setPassword(const QString& plainPassword) {
    RESTServer::password = QCryptographicHash::hash(plainPassword.toLower().toUtf8(), QCryptographicHash::Md5);
}

