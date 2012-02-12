
#include "configurationfiletest.h"


#include <QtTest/QtTest>
#include <QString>
#include <QDebug>

void ConfigurationFileTest::init()
{
    m_confFile = new ConfigurationFile("kminidlna_test.conf");
}
void ConfigurationFileTest::initTestCase()
{

}

void ConfigurationFileTest::cleanupTestCase()
{

}

void ConfigurationFileTest::cleanup()
{
    delete m_confFile;
    QFile f("kminidlna_test.conf");
    if (f.exists()) {
        if (f.remove()) {
            qDebug() << "File was removed";
        }
    }
}

void ConfigurationFileTest::createFileTest()
{
    QString file = "kminidlna_test.conf";

    QFile f(file);
    if (f.exists()) {
        if (f.remove()) {
            qDebug() << "File was removed";
        }
    }

    QVERIFY(m_confFile->createFile());
    QVERIFY(!m_confFile->createFile());
}

QTEST_MAIN(ConfigurationFileTest)
