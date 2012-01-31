/*
KminiDLNA
http://gitorious.org/kminidlna/pages/Home

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
#include <KApplication>
#include <unistd.h>
#include <KMessageBox>
#include <KLocalizedString>
#include <QTextStream>
#include <KConfigGroup>

#include <sys/types.h>
#include <signal.h>
#include <QFileInfo>
#include "../server/restserver.h"
#include "../server/interface/restminidlna.h"



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
 * SLOT send status minidlna to KminiDLNA
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
    KConfigGroup config = KGlobal::config()->group("minidlna");

    //set minidlna path
    QString minidlnapath = config.readEntry("minidlnapath", MiniDLNA::MINIDLNA_PATH);
    if (QFile::exists(minidlnapath)) {
        minidlnas = minidlnapath;
    } else {
        QFileInfo def(MiniDLNA::MINIDLNA_PATH);
        if (def.exists() && def.isExecutable()) {
            minidlnas = def.absoluteFilePath();
        } else {
            qDebug() << "minidlna in " << minidlnapath << " was not found";
        }
    }

    //Set pid path
    QString pidpath = config.readEntry("pidpath", MiniDLNA::PIDFILE_PATH);
    QFileInfo pid(pidpath);
    if (pid.isDir() && pid.isWritable()) {
        pathPidFile = pidpath;
    } else {
        QFileInfo def(MiniDLNA::PIDFILE_PATH);
        if (def.isDir() && def.isWritable()) {
//             qDebug()<< "setted default pid directory: "<< def.absolutePath();
        } else {
            qDebug() << "pid directory is not writable or it was not found";
        }
    }

    if (config.readEntry("scanfile", true)) {
        scanFile = true;
    } else {
        scanFile = false;
    }

    //Default configuration file
    m_useDefaultConfFile = config.readEntry("default_conf_file", true);

    //Path to conf file
    QString confFilePath = config.readEntry("conf_file_path", MiniDLNA::CONFFILE_PATH);
    if (!m_useDefaultConfFile) {
        QFileInfo confFile(confFilePath);
        if (confFile.exists() && confFile.isReadable()) {
            m_confFilePath = confFilePath;
        } else {
            qDebug() << "minidlna config file in " << m_confFilePath << " was not found using default config file";
            m_useDefaultConfFile = true;
            m_confFilePath = MiniDLNA::CONFFILE_PATH;
        }
    }
    setArg();
}

void MiniDLNAProcess::setArg() {
    arg.clear();
    arg << "-P" << pathPidFile + "minidlna.pid";
    if (scanFile) {
        arg << "-R";
    }
    if (!m_useDefaultConfFile) {
        arg << "-f" << m_confFilePath;
    }
}

void MiniDLNAProcess::loadResource() {
    addResource(new RESTMiniDLNA(this));
}

ConfigurationFile* MiniDLNAProcess::configFile() {
    if (m_confFile == 0) {
        loadConfigFile();
    }
    return m_confFile;
}

void MiniDLNAProcess::loadConfigFile() {
    m_confFile = new ConfigurationFile(m_confFilePath, this);
}




