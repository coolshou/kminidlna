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


class RESTServer : public QTcpServer {
    Q_OBJECT
public:
    RESTServer(int port, QObject* parent = 0);
    virtual ~RESTServer();
    inline int port(){return m_port;};
    static bool isRuning();
    void addInterfaces(RESTInterfaces* interface);
    bool removeInterfaces(RESTInterfaces* interface);
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
    static bool m_runing;
    QList<RESTInterfaces*> m_intefaces;
    
    void sendNoContent(QSslSocket* socket);
    void sendMSG(QSslSocket* socket, const QString msg);
    void receivedPUT(QSslSocket* socket, const QStringList& firstLine, const QHash<QString, QStringList>& header);
    void sendOnGETReply(QTcpSocket* socket, const ServerRequest* req);
    void sendOnPUTReply(QTcpSocket* socket, const ServerRequest* req);

    ServerRequest* receiveRequestHeader(QTcpSocket* socket);

public slots:
    void errorySSL(QList<QSslError>);
};

namespace Server {
static const int MAX_NUMBER_OF_PORT = 65000;
static const int DEFAULT_PORT = 8080;
static const QString DEFAULT_PASSWORD = "kminidlna";
}


#endif // RESTServer_H
