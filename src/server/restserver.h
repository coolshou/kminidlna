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
#include <QByteArray>
#include <QReadWriteLock>
#include <QPointer>
#include "serverrequest.h"
#include "interface/restresource.h"
#include "interface/restinterfaces.h"
#include "interface/basicrestresource.h"

/**
 *
 * @patern singleton
 */
class RESTServer : public QTcpServer, public RESTInterfaces {
        Q_OBJECT
    public:
        virtual void addInterfaces(RESTInterfaces* interface);
        virtual bool removeInterfaces(RESTInterfaces* interface);
        static RESTServer* getInstance();
        virtual void startServer();
        virtual void stopServer();
        virtual RESTResource* resource(QString address);

        //STATIC
        /**
         * password in md5 hash
         */
        static QByteArray password;
        static void setPassword(const QString& plainPassword);
        /**
         * login name
         */
        static QByteArray login;
        static const int MAX_NUMBER_OF_PORT;
        static const int DEFAULT_PORT;

        inline bool isRuning() {
            return m_runing;
        }

        inline int port() {
            return m_port;
        }
        inline void setPort(int port) {
            m_port = port;
        }
        void setCert(const QSslCertificate& cert);
        void setSslKey(QSslKey* key);
        void loadConfig();

    public slots:
        void errorsSSL(QList<QSslError>);

    protected:
        RESTServer(QObject* parent = 0);
        void loadResource();
        virtual ~RESTServer();

        RESTServer(const RESTServer &); // hide copy constructor
        RESTServer& operator=(const RESTServer &); // hide assign op

    protected slots:
        virtual void incomingConnection(int socketDescriptor);
        virtual void handshakeComplete();
        virtual void connectionClosed();


    private:
        QSharedPointer<QSslKey> m_key;
        QSslCertificate m_cert;
        QString m_notFoundFileHtmlPath;
        quint16 m_port;
        bool m_runing;
        QPointer<BasicRESTResource> res;
//     QByteArray m_loginpass;
        QList<RESTInterfaces*> m_intefaces;
        QReadWriteLock m_lock;
        bool m_customCert;
        QString m_pkeyPath;
        QString m_certPath;
        void loadCert(bool custom = true);
        void loadCert(const QString& pkeyPath, const QString& certPath, bool generate = false);
        bool m_validCertAndKey;
    signals:
        void run(bool state);
        void notValidKeyCertificate();
};


#endif // RESTServer_H
