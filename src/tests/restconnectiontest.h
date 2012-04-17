#include "../server/restconnection.h"
#include <QObject>
#include <QTcpSocket>
#include <QSslSocket>
#include <QTcpServer>
#include <QPointer>
#include <QSslConfiguration>
class MockRESTServer :public RESTServer{
  public:
        MockRESTServer(QObject* parent = 0);    
};



class RESTConnectionTest: public QObject {
        Q_OBJECT
    private:
        QPointer<RESTConnection> connection;
	QPointer<QTcpSocket> s;
	QPointer<QSslSocket> c;
	QPointer<QTcpServer> serv;
	QPointer<RESTServer> restserv;
	int port;
	QSslConfiguration m_sslConf;
	int timeout;

    private slots:
        void initTestCase(); // will be called before the first testfunction is executed.
        void cleanupTestCase(); // will be called after the last testfunction was executed.
        void init(); // will be called before each testfunction is executed.
        void cleanup(); // will be called after every testfunction.
	void notFound404Test();
	void versionTest();
	void wrongLoginTest();
};
