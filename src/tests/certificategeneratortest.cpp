/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) 2012  TomÃ¡Å¡ PolednÃ½ <email>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#include "certificategeneratortest.h"
#include <QtTest/QtTest>
#include <QSslCertificate>
#include <QSslKey>
#include <QFile>
#include "../server/certificategenerator.h"
const QString CertificateGeneratorTest::SAVE_PATH = "/tmp/";
void CertificateGeneratorTest::init() {

}
void CertificateGeneratorTest::initTestCase() {

}


void CertificateGeneratorTest::generateValidCertificateAndPKey() {
    X509Value v;
    v.commonName = (unsigned char*) "KMiniDLNA";
    v.countryName = (unsigned char*) "CZ";
    int bits = 1024;
    CertificateGenerator::CreatedCertificteError err = CertificateGenerator::createCertificate(v, SAVE_PATH, (unsigned int)bits);
    QCOMPARE(err, CertificateGenerator::NoError);
    QFile cert(SAVE_PATH + CertificateGenerator::CERTIFICATE_NAME);
    QVERIFY(cert.open(QIODevice::ReadOnly | QIODevice::Text));
    QSslCertificate cer(&cert);
    cert.close();
    QVERIFY(cer.isValid());
    QString cn = cer.subjectInfo(QSslCertificate::CommonName);
    QString otherCN = (const char*) v.commonName;
    QCOMPARE(cn, otherCN);

    //PKey
    QFile pkey(SAVE_PATH + CertificateGenerator::PKEY_NAME);
    QVERIFY(pkey.open(QIODevice::ReadOnly | QIODevice::Text));
    QSslKey key(&pkey, QSsl::Rsa);
    pkey.close();
    QVERIFY(!key.isNull());
    QCOMPARE(key.length(), bits);
}

void CertificateGeneratorTest::cleanup() {

}
void CertificateGeneratorTest::cleanupTestCase() {
    if(QFile::exists(SAVE_PATH + CertificateGenerator::PKEY_NAME)){
      QFile f(SAVE_PATH + CertificateGenerator::PKEY_NAME);
      f.remove();
      QVERIFY(!f.exists());
    }
    if(QFile::exists(SAVE_PATH + CertificateGenerator::CERTIFICATE_NAME)){
      QFile f(SAVE_PATH + CertificateGenerator::CERTIFICATE_NAME);
      f.remove();
      QVERIFY(!f.exists());
    }
}

QTEST_MAIN(CertificateGeneratorTest)
