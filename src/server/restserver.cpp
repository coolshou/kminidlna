
#include "restserver.h"
#include <QString>

#include <QSslError>
#include <QFile>
#include <QStringList>
#include <QHash>
#include <QFileInfo>
//#include <KConfigGroup>
//#include <KGlobal>
#include "serverrequest.h"
#include "restconnection.h"
#include "certificategenerator.h"

#include <QDomDocument>
#include <sys/stat.h>
#include <qconfig.h>
#include <QSettings>
//#include <KConfig>
#include <QStandardPaths>
//#include <KStandardDirs>

// bool RESTServer::m_runing = false;

QByteArray RESTServer::login = QByteArray();
QByteArray RESTServer::password = QByteArray();
const int RESTServer::MAX_NUMBER_OF_PORT = 65000;
const int RESTServer::DEFAULT_PORT = 8080;

RESTServer::RESTServer(QObject* parent) : QTcpServer(parent), m_port(DEFAULT_PORT), m_runing(false), m_customCert(false), m_validCertAndKey(false) {
    loadConfig();
//     loadCert(m_customCert);
    qDebug() << "NEW REST SERVER";
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
    QSettings config;
    config.beginGroup("server");
    setPort(config.value("port", DEFAULT_PORT).toInt());

    RESTServer::login = config.value("username", QByteArray()).toByteArray();
    RESTServer::password = config.value("password", QByteArray()).toByteArray();

    m_customCert = config.value("custom_cert", false).toBool();
    m_certPath = config.value("cert_path", QString()).toString();
    m_pkeyPath = config.value("pkey_path", QString()).toString();
    config.endGroup();
    /*
    KConfigGroup config = KGlobal::config()->group("server");
    setPort(config.readEntry("port", DEFAULT_PORT));

    RESTServer::login = config.readEntry("username", QByteArray());
    RESTServer::password = config.readEntry("password", QByteArray());

    m_customCert = config.readEntry("custom_cert", false);
    m_certPath = config.readEntry("cert_path", QString());
    m_pkeyPath = config.readEntry("pkey_path", QString());
    */
}

/**
 * Start server if server not run
 * emit run signal
 */
void RESTServer::startServer() {
//     loadConfig();
    loadCert(m_customCert);
    if(m_validCertAndKey) {
        if(!m_runing) {
            m_runing = listen(QHostAddress::Any, m_port);
            qDebug() << "RESTServer: starting server";
// 	    qDebug() << this->errorsSSL();
	    qDebug() << errorString();
            if(!errorString().isEmpty()) {
                qDebug() << errorString();
            }
            emit run(m_runing);
        }
    } else {
        qDebug() << "RESTServer: cannot start server private key or certificate is not valid";
        emit notValidKeyCertificate();
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
 */
void RESTServer::incomingConnection(int socketDescriptor) {
    if(!m_validCertAndKey) {
        QTcpSocket* soc = nextPendingConnection();
        soc->close();
        soc->deleteLater();
        return;
    }
    QSslSocket* socket = new QSslSocket(this);
    //DBG
//     qDebug() << "RESTServer: Connection incoming "<< socketDescriptor;
    //End DBG

    if(socket->setSocketDescriptor(socketDescriptor)) {
//         if(m_cert == 0) {
//             socket->setLocalCertificate(m_certPath);
//             m_cert = socket->localCertificate();
//         } else {
//             socket->setLocalCertificate(m_cert);
//         }
        socket->setLocalCertificate(m_cert);
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
    qDebug() << "Closed: " << socket->socketDescriptor() << " Reason: " << socket->error() << socket->errorString() ;
    socket->deleteLater();
}

void RESTServer::handshakeComplete() {
    QSslSocket* socket = dynamic_cast<QSslSocket *>(sender());
    disconnect(socket, SIGNAL(disconnected()), this, SLOT(connectionClosed()));
/* TODO: RESTConnection
    RESTConnection* connection = new RESTConnection(socket, this);
    connect(connection, SIGNAL(finished()), connection, SLOT(deleteLater()));
    connection->start();
    */
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

    QByteArray versionxml = "<qminidlna><version>0.9pre</version></qminidlna>";

    if(res->setXML(versionxml)) {
        addResource(res);
    }
}

/**
 * read lock thread
 * @return resource on address if resource not exist return 0
 */
RESTResource* RESTServer::resource(QString address) {
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

void RESTServer::loadCert(bool custom) {
    if(custom) {
        loadCert(m_pkeyPath, m_certPath);
    } else {
        //TODO: CertificateGenerator
        //KStandardDirs dirs;
        //QString path =  dirs.saveLocation("data") + "qminidlna/";
        //loadCert(path + CertificateGenerator::PKEY_NAME, path + CertificateGenerator::CERTIFICATE_NAME, true);
    }
}

void RESTServer::loadCert(const QString& pkeyPath, const QString& certPath, bool generate) {
    qDebug() << certPath;
    if(QFile::exists(pkeyPath) && QFile::exists(certPath)) {
        QFile fkey(pkeyPath);
        if(fkey.open(QIODevice::ReadOnly | QIODevice::Text)) {
            m_key = QSharedPointer<QSslKey>(new QSslKey(&fkey, QSsl::Rsa));
            fkey.close();
            if(m_key.isNull()) {
                qDebug() << "RESTServer: private key is not valid";
                m_validCertAndKey = false;
                return;
            }
        } else {
            qDebug() << "RESTServer: cannot open private key path: " << pkeyPath;
            m_validCertAndKey = false;
            return;
        }

        QFile fcert(certPath);
        if(fcert.open(QIODevice::ReadOnly | QIODevice::Text)) {
            m_cert = QSslCertificate(&fcert);
            fcert.close();
            /*
            if(!m_cert.isValid()) {
                qDebug() << "RESTServer: certificate is not valid";
                m_validCertAndKey = false;
                return;
            }*/
        } else {
            qDebug() << "RESTServer: cannot open certificate path: " << certPath;
            m_validCertAndKey = false;
            return;
        }
        m_validCertAndKey = true;
    } else if(generate) {
        /*
        X509Value v;
        v.commonName = (unsigned char*) "qminidlna";
        v.countryName = (unsigned char*) "CZ";
        KStandardDirs dirs;
        QString path = dirs.saveLocation("data") + "qminidlna/";
        CertificateGenerator::createCertificate(v, path);
        loadCert(path + CertificateGenerator::PKEY_NAME, path + CertificateGenerator::CERTIFICATE_NAME);
        */
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

