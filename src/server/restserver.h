#ifndef RESTServer_H
#define RESTServer_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QSslCertificate>
#include <QSslKey>
#include <QSslSocket>
#include <QStringList>
#include <QString>
#include <QStringList>
#include <QHash>
#include <QList>
#include "serverrequest.h"
#include "interface/restinterfaces.h"
#include "interface/basicrestresource.h"

/**
 *
 * @patern singleton
 */
class RESTServer : public QTcpServer, public RESTInterfaces {
    Q_OBJECT
public:
    virtual ~RESTServer();
    void addInterfaces(RESTInterfaces* interface);
    bool removeInterfaces(RESTInterfaces* interface);
    static RESTServer* getInstance();
    void startServer();
    void stopServer();
    inline bool isRuning() {
        return m_runing;
    };

    inline int port() {
        return m_port;
    };
    inline void setPort(int port) {
        m_port = port;
    };
    
public slots:
    void errorsSSL(QList<QSslError>);

protected slots:
    virtual void incomingConnection(int socketDescriptor);
    void handshakeComplete();
    void connectionClosed();
    void receiveData();

private:
    QString m_certPath;
    QSslKey* m_key;
    QSslCertificate m_cert;
    QString m_notFoundFileHtmlPath;
    quint16 m_port;
    bool m_runing;
    QByteArray m_loginpass;
    QList<RESTInterfaces*> m_intefaces;
    RESTServer(QObject* parent = 0);

    void sendNoContent(QTcpSocket* socket);
    void sendMSG(QTcpSocket* socket, const QString msg);
    void processGETAndSendReply(QTcpSocket* socket, const ServerRequest* req);
    void processPUTAndSendReply(QTcpSocket* socket, ServerRequest* req);
    void send404NotFound(QTcpSocket* socket);
    void loadResource();
    void loadConfig();

    ServerRequest* receiveRequestHeader(QTcpSocket* socket);
signals:
  void run(bool state);
};

namespace Server {
static const int MAX_NUMBER_OF_PORT = 65000;
static const int DEFAULT_PORT = 8080;
static const QString DEFAULT_PASSWORD = "kminidlna";
}


#endif // RESTServer_H
