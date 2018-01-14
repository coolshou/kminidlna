/*
qminidlna
http://gitorious.org/qminidlna/pages/Home

Copyright (C) 2011 Saljack <saljacky a gmail dot com>

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
version 2 as published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#include "minidlna_process.h"
#include <QDebug>
#include <QApplication>
//#include <KApplication>
#include <unistd.h>
#include <QMessageBox>
//#include <KMessageBox>
#include <QLocale>
//#include <KLocalizedString>
#include <QTextStream>
#include <QSettings>
//#include <KConfigGroup>

#include <sys/types.h>
#include <signal.h>
#include <QFileInfo>
#include "../server/restserver.h"
#include "../server/interface/restminidlna.h"
#include <QDir>
#include "../server/interface/restmediafolder.h"
#include "../server/interface/restminidlnaput.h"

const QString MiniDLNAProcess::MINIDLNA_PATH = "/usr/sbin/minidlna";
const QString MiniDLNAProcess::PIDFILE_PATH = "/tmp/";
const QString MiniDLNAProcess::GLOBALCONFFILE_PATH = "/etc/minidlna.conf";
const int MiniDLNAProcess::DEFAULTPORT = 8200;

MiniDLNAProcess::MiniDLNAProcess(): RESTInterfaces() {
    m_confFile = 0;
    loadConfiguration();
    t_pid = new PidThread(this);
    connect(t_pid, SIGNAL(foundPidFile(bool)),
            this, SLOT(onPidFile(bool)));

    loadResource();
    minidlna = new QProcess();

    RESTServer* server = RESTServer::getInstance();
    server->addInterfaces(this);
}

MiniDLNAProcess::~MiniDLNAProcess() {

    if (minidlna->state() == QProcess::Running) {
        minidlna->kill();
    }
    delete minidlna;

    if (minidlnaStatus()) {
        kill(t_pid->getPid(), 15);
    }
    if (t_pid != 0) {
        if (t_pid->isRunning()) {
            t_pid->terminate();
        }
        delete t_pid;
    }
}

MiniDLNAProcess* MiniDLNAProcess::getInstance() {
    static MiniDLNAProcess instance;
    return &instance;
}


/**
 * start minidlna a send signal
 */
void MiniDLNAProcess::minidlnaStart() {
    if (!QFile::exists(t_pid->getPidPath())) {
        minidlna->start(minidlnas, arg);
        emit minidlnaStatus(QProcess::Starting);
        t_pid->setPathPidFile(pathPidFile + "minidlna.pid");
        t_pid->start();
    } else {
        //get PID FROM FILE
        onPidFile(true);
    }
}


/**
 * kill minidlna a send signal
 */
void MiniDLNAProcess::minidlnaKill() {
    if (QFile::exists(t_pid->getPidPath())) {
        if (t_pid->getPid() < 0) {
            return;
        }
        kill(t_pid->getPid(), 15);
        emit minidlnaStatus(QProcess::NotRunning);
    }
}

/**
 * Status of minidlna process
 * @return true if minidlna runnning, false if not running
 */
bool MiniDLNAProcess::minidlnaStatus() {
    if (QFile::exists(t_pid->getPidPath())) {
        return true;
    }
    return false;
}

/**
 * SLOT send status minidlna to qminidlna
 */
void MiniDLNAProcess::onPidFile(bool found) {
    if (found) {
        emit minidlnaStatus(QProcess::Running);
    } else {
        emit minidlnaStatus(QProcess::NotRunning);
    }
}

void MiniDLNAProcess::loadConfiguration() {
    loadSettings();
}

void MiniDLNAProcess::loadSettings() {
    QSettings config;
    config.beginGroup("minidlna");
    //KConfigGroup config = KGlobal::config()->group("minidlna");

    //set minidlna path
    QString minidlnapath = config.value("minidlnapath", MiniDLNAProcess::MINIDLNA_PATH).toString();
    if (QFile::exists(minidlnapath)) {
        minidlnas = minidlnapath;
    } else {
        QFileInfo def(MiniDLNAProcess::MINIDLNA_PATH);
        if (def.exists() && def.isExecutable()) {
            minidlnas = def.absoluteFilePath();
        } else {
            qDebug() << tr("MiniDLNA in ") << minidlnapath << tr(" was not found");
        }
    }

    //Set pid path
    QString pidpath = config.value("pidpath", MiniDLNAProcess::PIDFILE_PATH).toString();
    QFileInfo pid(pidpath);
    if (pid.isDir() && pid.isWritable()) {
        pathPidFile = pidpath;
    } else {
        QFileInfo def(MiniDLNAProcess::PIDFILE_PATH);
        if (def.isDir() && def.isWritable()) {
//             qDebug()<< "setted default pid directory: "<< def.absolutePath();
        } else {
            qDebug() << "pid directory is not writable or it was not found";
        }
    }

    m_fullRescanFile = config.value("scanfile", true).toBool();

    m_port = config.value("minidlna_port", MiniDLNAProcess::DEFAULTPORT).toInt();

    //Set configuration file
    m_usedConfFile = ConfigurationFile::ConfFile(config.value("use_conf_file", -1).toInt());

    QString confFilePath = config.value("conf_file_path", ConfigurationFile::PATH_TO_DEFAULT_LOCAL_FILE).toString();

    QFileInfo configFile;
    switch (m_usedConfFile) {
    case ConfigurationFile::DEFAULT:
        confFilePath = ConfigurationFile::PATH_TO_DEFAULT_LOCAL_FILE;
    case ConfigurationFile::USER:
        configFile.setFile(confFilePath);
        if (!configFile.exists()) {
            createConfigFile(confFilePath);
        }
        if (configFile.isReadable()) {
            m_confFilePath = confFilePath;
            break;
        }
        qDebug() << "minidlna config file in " << m_confFilePath << " was not found using global config file";
    case ConfigurationFile::GLOBAL:
        m_usedConfFile = ConfigurationFile::GLOBAL;
        m_confFilePath = MiniDLNAProcess::GLOBALCONFFILE_PATH;
    }
    config.endGroup();
    setArg();
}

void MiniDLNAProcess::setArg() {
    arg.clear();
    arg << "-P" << pathPidFile + "minidlna.pid";
    if (m_fullRescanFile) {
        arg << "-R";
    }
    if (m_usedConfFile != ConfigurationFile::GLOBAL) {
        arg << "-f" << m_confFilePath;
    }
    if (m_port != MiniDLNAProcess::DEFAULTPORT) {
        arg << "-p" << QString(m_port);
    }
}

void MiniDLNAProcess::loadResource() {
    addResource(new RESTMiniDLNA(this));
    addResource(new RESTMediaFolder(this));
    //addResource(new RESTMiniDLNAPUT(this));
}

/**
 * @return actual configuration file (path from settings)
 */
ConfigurationFile* MiniDLNAProcess::configFile() {
    if (m_confFile == 0) {
        qDebug() << "MiniDLNAProcess: Loading config file";
        loadConfigFile();
    } else if (m_isConfigFileOptionsChanged) {
        if (m_confFile->path() != m_confFilePath) {
            delete m_confFile;
            loadConfigFile();
        } else {
            m_confFile->reload();
            m_isConfigFileOptionsChanged = false;
        }
    }
    return m_confFile;
}

void MiniDLNAProcess::loadConfigFile() {
    m_confFile = new ConfigurationFile(m_confFilePath, this);
    m_isConfigFileOptionsChanged = false;
}

void MiniDLNAProcess::createConfigFile(QString path) {

    QFileInfo info(path);
    QDir dir = info.dir();
    if (!dir.exists()) {
        if (!dir.mkpath(dir.path())) {
            return;
        }
        //Create file
        if (m_confFile != 0) {
            delete m_confFile;
        }
        m_confFile = new ConfigurationFile(path, this);
        m_confFile->createFile();
    }
}

void MiniDLNAProcess::configFileChanged() {
    loadSettings();
    m_isConfigFileOptionsChanged = true;
    emit configurationFileChanged();
}
