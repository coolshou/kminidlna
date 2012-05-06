#include "restconnectiontest.h"

#include <QtTest/QtTest>
#include <QString>
#include <QDebug>
#include <QByteArray>
#include <QDomDocument>
#include <QEventLoop>
#include <QNetworkReply>
#include <QUrl>
#include <QXmlSchema>
#include <QFile>

char pemdata_cert[] =
    "-----BEGIN CERTIFICATE-----\n"
    "MIIC5DCCAk2gAwIBAgIJAKkInigqTM1dMA0GCSqGSIb3DQEBBQUAMIGKMQswCQYD\n"
    "VQQGEwJDWjEPMA0GA1UECAwGUHJhZ3VlMQ8wDQYDVQQHDAZQcmFndWUxEDAOBgNV\n"
    "BAoMB1NhbGphY2sxEjAQBgNVBAsMCUttaW5pRExOQTEQMA4GA1UEAwwHU2FsamFj\n"
    "azEhMB8GCSqGSIb3DQEJARYSc2FsamFja3lAZ21haWwuY29tMB4XDTExMTEyNjE1\n"
    "MjYzN1oXDTEyMTEyNTE1MjYzN1owgYoxCzAJBgNVBAYTAkNaMQ8wDQYDVQQIDAZQ\n"
    "cmFndWUxDzANBgNVBAcMBlByYWd1ZTEQMA4GA1UECgwHU2FsamFjazESMBAGA1UE\n"
    "CwwJS21pbmlETE5BMRAwDgYDVQQDDAdTYWxqYWNrMSEwHwYJKoZIhvcNAQkBFhJz\n"
    "YWxqYWNreUBnbWFpbC5jb20wgZ8wDQYJKoZIhvcNAQEBBQADgY0AMIGJAoGBANfz\n"
    "F0mqwtyqXwky1UEeDVmtwadbHQE6MB01h0cy86Ie5zRV+6tUq9YA87MjZ6KOp362\n"
    "M2OWamWEOZHyyyHUttW2t0uMPuMRnpJvBX0g2ZpuioDCP/wl14VwnJjdMKQkpHB/\n"
    "1zEHzd3aiOS19D1C/lAzW9L4c7szIJMnehX0mV0hAgMBAAGjUDBOMB0GA1UdDgQW\n"
    "BBQzZzk57fJAD7pfITU0/c5M0dFjwjAfBgNVHSMEGDAWgBQzZzk57fJAD7pfITU0\n"
    "/c5M0dFjwjAMBgNVHRMEBTADAQH/MA0GCSqGSIb3DQEBBQUAA4GBAHAyHtm0jdnZ\n"
    "49I2ZD4eoj2yx0lxGQh8acLdMmzm31hiLQ2HmLblx0jN2MA9p4hB1xWqooRu+xYI\n"
    "F/8YUTOrgW0CGg+IK9FE3cvj+mpat+VuDyuTJCjlvalnFykT1l9dsZHIH8GagkdW\n"
    "4wWTglPJ4ORufiP/1HTh+8qRnVmWlbfq\n"
    "-----END CERTIFICATE-----\n";

char pemdata_privkey[] =
    "-----BEGIN PRIVATE KEY-----\n"
    "MIICdgIBADANBgkqhkiG9w0BAQEFAASCAmAwggJcAgEAAoGBANfzF0mqwtyqXwky\n"
    "1UEeDVmtwadbHQE6MB01h0cy86Ie5zRV+6tUq9YA87MjZ6KOp362M2OWamWEOZHy\n"
    "yyHUttW2t0uMPuMRnpJvBX0g2ZpuioDCP/wl14VwnJjdMKQkpHB/1zEHzd3aiOS1\n"
    "9D1C/lAzW9L4c7szIJMnehX0mV0hAgMBAAECgYAmo1oRVjpTXTBv5uZ9JjFhUFOL\n"
    "7CN+arjdIQy5UoaBBWJ83L2Q5C9ytOzlqnoKfde+Nj9vnCQhUc0zLevme7bSpe/g\n"
    "eAavlzuumuv6TqZVdkFYqW4m23yuWM39ZJ2pNb/jREqwih8qzw3J6JJSDaPw7zGc\n"
    "IyXqkFqE5XKh2d0YOQJBAPuP0qIZQ//botLWznCTCO5dsPmLAYFNsZoVZhWAzuVM\n"
    "UX28ZSXYXvbf+z6zLYsp2TOVtRHcx8089EEU2/wskTcCQQDbwmz/H7wx3sDVSByI\n"
    "WmOkF7O8KGA2V2ZMEDXpHDXByYl//OmOZKScIC4Y5FmN3onhdZ2ZsmC51KkTB6uo\n"
    "5JBnAkB8bYch5p1b0KeY4eb6h7/DJ1SCGd+EtYlDTZJQ0IWFpVopdyYQshVGx0Le\n"
    "oIHGc8ybV1FYpHuvaJboGgk1klXBAkBoOXvMO9bbqBO4k6zElmnvZzZdJKEM5MKl\n"
    "jIKk/jYYpGbMBr0WRXE0gqaaM6KtgD/QaCHIxSrOt6dAWUD2pwA9AkEAveZggOWZ\n"
    "dktlCyZtm180StgyvtTYXNK7YJ3fMrYVFnVPKlIzf213lDNv7Qg1YVPEO2xi4iQv\n"
    "ffQDAXiVOCshTg==\n"
    "-----END PRIVATE KEY-----\n";

char versionScheme[] =
    "<?xml version=\"1.0\" encoding=\"utf-8\"?>"
    "<xs:schema xmlns:xs=\"http://www.w3.org/2001/XMLSchema\">"
    "  <xs:element name=\"kminidlna\">"
    "    <xs:complexType>"
    "        <xs:sequence>"
    "               <xs:element name=\"version\" type=\"xs:string\"/>"
    "        </xs:sequence> "
    "    </xs:complexType> "
    "  </xs:element>"
    "</xs:schema>";


void RESTConnectionTest::initTestCase() {
    timeout = 1500;
    port = 9487;
    m_sslConf.setPeerVerifyMode(QSslSocket::VerifyNone);
    restserv = RESTServer::getInstance();
    restserv->setPort(port);
    restserv->setCert(QSslCertificate::fromData(pemdata_cert).first());
    restserv->setSslKey(new QSslKey(pemdata_privkey, QSsl::Rsa));
    RESTServer::login = "saljack";
    RESTServer::setPassword("superman");
    restserv->startServer();
    qDebug() << restserv->port();
    QVERIFY2(restserv->isListening(), "Server not listenning");
}

void RESTConnectionTest::cleanupTestCase() {
    restserv->stopServer();
}

void RESTConnectionTest::init() {

}

void RESTConnectionTest::cleanup() {
}

void RESTConnectionTest::versionTest() {
    QNetworkAccessManager *networkMgr = new QNetworkAccessManager(this);
    QString addr = "https://saljack:superman@localhost:" + QString::number(port) + "/version.xml";
    QUrl url(addr);
    qDebug() << addr;
    QNetworkRequest request(url);
//     request.setRawHeader("Authorization", "Basic " +
//                          QByteArray("saljack:superman").toBase64());
    request.setSslConfiguration(m_sslConf);

    QNetworkReply *reply = networkMgr->get(request);

    QEventLoop loop;
    QObject::connect(reply, SIGNAL(readyRead()), &loop, SLOT(quit()));
    QTimer::singleShot(timeout, &loop, SLOT(quit()));

    loop.exec();

    QByteArray tmp = reply->readAll();
    qDebug() << reply->errorString();
    QCOMPARE(reply->error(), QNetworkReply::NoError);

    QDomDocument d;
    QVERIFY2(d.setContent(tmp), tmp);

    QXmlSchema schema;
    QVERIFY2(schema.load(versionScheme), "Scheme is not loaded");
    QVERIFY2(schema.isValid(), "Scheme is not validate");
    QXmlSchemaValidator validator(schema);
    QVERIFY2(validator.validate(d.toByteArray()), "XML is not validate");
    reply->deleteLater();
    networkMgr->deleteLater();
}

void RESTConnectionTest::notFound404Test() {
    QNetworkAccessManager *networkMgr = new QNetworkAccessManager(this);
    QString addr = "https://saljack:superman@localhost:" + QString::number(port) + "/rndfhasduf.xml";
    QUrl url(addr);
    QNetworkRequest request(url);
//     request.setRawHeader("Authorization", "Basic " +
//                          QByteArray("saljack:superman").toBase64());
    request.setSslConfiguration(m_sslConf);

    QNetworkReply *reply = networkMgr->get(request);

    QEventLoop loop;
//     QObject::connect(reply, SIGNAL(readyRead()), &loop, SLOT(quit()));
    QTimer::singleShot(timeout, &loop, SLOT(quit()));

    loop.exec();
//FIXME
//      QCOMPARE(reply->error(), QNetworkReply::ContentNotFoundError);
//      QString nf404 = reply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toString();
//      qDebug() << nf404;
//     int code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
//     QCOMPARE(code, 0);

    reply->deleteLater();
    networkMgr->deleteLater();
}

void RESTConnectionTest::wrongLoginTest() {
    //WRONG USERNEM AND PASSWORD
    QNetworkAccessManager *networkMgr = new QNetworkAccessManager(this);
    QString addr = "https://somewronglogin:somewrongpassword@localhost:" + QString::number(port) + "/version.xml";
    QUrl url(addr);
    QNetworkRequest request(url);
    request.setSslConfiguration(m_sslConf);

    QNetworkReply *reply = networkMgr->get(request);

    QEventLoop loop;
    QObject::connect(reply, SIGNAL(readyRead()), &loop, SLOT(quit()));
    QTimer::singleShot(timeout, &loop, SLOT(quit()));

    loop.exec();
    QCOMPARE(reply->error(), QNetworkReply::AuthenticationRequiredError);
    reply->deleteLater();

    //without password
    addr = "https://localhost:" + QString::number(port) + "/version.xml";
    request.setUrl(QUrl(addr));

    reply = networkMgr->get(request);

    QObject::connect(reply, SIGNAL(readyRead()), &loop, SLOT(quit()));
    QTimer::singleShot(timeout, &loop, SLOT(quit()));

    loop.exec();
    QCOMPARE(reply->error(), QNetworkReply::AuthenticationRequiredError);
    reply->deleteLater();

    //Correct Usernam and password
    addr = "https://saljack:superman@localhost:" + QString::number(port) + "/version.xml";
    request.setUrl(QUrl(addr));

    reply = networkMgr->get(request);

    QObject::connect(reply, SIGNAL(readyRead()), &loop, SLOT(quit()));
    QTimer::singleShot(timeout, &loop, SLOT(quit()));

    loop.exec();

    QCOMPARE(reply->error(), QNetworkReply::NoError);
    reply->deleteLater();

    networkMgr->deleteLater();
}




QTEST_MAIN(RESTConnectionTest)
