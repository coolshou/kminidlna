#include "restconnectiontest.h"

#include <QtTest/QtTest>
#include <QString>
#include <QDebug>
#include <QByteArray>
#include <QDomDocument>

void RESTConnectionTest::initTestCase() {
    port = 98568;
    restserv = RESTServer::getInstance();
    restserv->setPort(port);
    restserv->startServer();
    QVERIFY2(restserv->isListening(), "Server not listenning");
    c = new QSslSocket(this);
}

void RESTConnectionTest::cleanupTestCase() {

//     delete connection;
//     delete s;
//     delete c;
//     delete serv;
}

void RESTConnectionTest::init() {

}

void RESTConnectionTest::cleanup() {

}

void RESTConnectionTest::testProcessingVersion() {
//     c->connectToHost("localhost", port);
//     QVERIFY2(c->waitForConnected(100), "Client is not connected");
//     c->write("GET /version.xml HTTP/1.1\r\n\r\n");
//     c->flush();
//     c->waitForBytesWritten(100);
// //     s->waitForReadyRead(1000);
//     QVERIFY2(c->waitForReadyRead(100), "connection not reply");
//     QByteArray tmp = c->readAll();
//     QDomDocument d;
// 
//     QVERIFY2(d.setContent(tmp), tmp);

}


QTEST_MAIN(RESTConnectionTest)
