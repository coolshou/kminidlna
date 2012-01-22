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



MinidlnaProcess::MinidlnaProcess(): RESTInterfaces()
{
    loadSettings();
    t_pid = new PidThread ( this );
    connect ( t_pid, SIGNAL ( foundPidFile ( bool ) ), this, SLOT ( onPidFile ( bool ) ) );

    loadResource();
    minidlna = new QProcess();
    
//     connect(kapp, SIGNAL(aboutToQuit()), this, SLOT());
    RESTServer* server = RESTServer::getInstance();
    server->addInterfaces(this);
}

MinidlnaProcess::~MinidlnaProcess() 
{
    
    if ( minidlna->state() == QProcess::Running )
    {
        minidlna->kill();
    }
    delete minidlna;

    if ( minidlnaStatus() )
    {
        kill ( t_pid->getPid(), 15 );
    }
    if ( t_pid != 0 )
    {
        if ( t_pid->isRunning() )
        {
            t_pid->terminate();
        }
        delete t_pid;
    }
}

MinidlnaProcess* MinidlnaProcess::getInstance()
{
    static MinidlnaProcess instance;
    return &instance;
}


/**
 * start minidlna a send signal
 */
void MinidlnaProcess::minidlnaStart()
{
    if ( !QFile::exists ( t_pid->getPidPath() ) )
    {
        minidlna->start ( minidlnas, arg );
        emit minidlnaStatus ( QProcess::Starting );
        t_pid->setPathPidFile ( pathPidFile+"minidlna.pid" );
        t_pid->start();
    }
    else
    {
        //get PID FROM FILE

        onPidFile ( true );
    }
}


/**
 * kill minidlna a send signal
 */
void MinidlnaProcess::minidlnaKill()
{
    if ( QFile::exists ( t_pid->getPidPath() ) )
    {
        if ( t_pid->getPid() <0 )
        {
            return;
        }
        kill ( t_pid->getPid(), 15 );
        emit minidlnaStatus ( QProcess::NotRunning );
    }
}

/**
 * Status of minidlna process
 * @return true if minidlna runnning, false if not running
 */
bool MinidlnaProcess::minidlnaStatus()
{
    if ( QFile::exists ( t_pid->getPidPath() ) )
    {
// 	emit minidlnaStatus ( QProcess::Running );
        return true;
    }
    return false;
}

/**
 * SLOT send status minidlna to KminiDLNA
 */
void MinidlnaProcess::onPidFile ( bool found )
{
    if ( found )
    {
        emit minidlnaStatus ( QProcess::Running );
    }
    else
    {
        emit minidlnaStatus ( QProcess::NotRunning );
    }
}

void MinidlnaProcess::loadSettings()
{
    KConfigGroup config = KGlobal::config()->group ( "minidlna" );

    //set minidlna path
    QString minidlnapath = config.readEntry ( "minidlnapath", MiniDLNA::MINIDLNA_PATH );
    if ( QFile::exists ( minidlnapath ) )
    {
        minidlnas = minidlnapath;
    }
    else
    {

        QFileInfo def ( MiniDLNA::MINIDLNA_PATH);
        if ( def.exists() && def.isExecutable() )
        {
            minidlnas = def.absoluteFilePath();
        }
        else
        {
            //TODO add error mesage
            qDebug() << "minidlna in " << minidlnapath << " was not found";
        }
    }

    //Set pid path
    QString pidpath = config.readEntry ( "pidpath", MiniDLNA::PIDFILE_PATH );
    QFileInfo pid ( pidpath );
    if ( pid.isDir() && pid.isWritable() )
    {
        pathPidFile = pidpath;
    }
    else
    {
        QFileInfo def(MiniDLNA::PIDFILE_PATH);
        if (def.isDir() && def.isWritable()) {
//             qDebug()<< "setted default pid directory: "<< def.absolutePath();
        } else {
            //TODO add error mesage
            qDebug() << "pid directory is not writable or it was not found";
        }
    }

    if ( config.readEntry ( "scanfile", true ) )
    {
        scanFile = true;
    }
    else
    {
        scanFile = false;
    }

    //Default configuration file
    if (config.readEntry("default_conf_file", true)) {
        defConfFile = true;
    } else {
        defConfFile = false;
    }

    //Path to conf file
    QString conffilepath = config.readEntry ( "conf_file_path", MiniDLNA::CONFFILE_PATH );
    QFileInfo confFile(conffilepath);
    if ( confFile.exists() && confFile.isReadable())
    {
        path_conffile = conffilepath;
    }
    else
    {
        confFile.setFile ( MiniDLNA::CONFFILE_PATH );
        if ( confFile.exists() && confFile.isReadable() )
        {
            path_conffile = confFile.absoluteFilePath();
        }
        else
        {
            //TODO add error mesage
            qDebug() << "minidlna in " << minidlnapath << " was not found";
        }
    }
    setArg();
}

void MinidlnaProcess::setArg()
{
    arg.clear();
    arg << "-P" << pathPidFile+"minidlna.pid";
    if ( scanFile )
    {
        arg << "-R";
    }

    if (!defConfFile) {
        arg << "-f" << path_conffile;
    }
}

void MinidlnaProcess::loadResource()
{
    addResource(new RESTMiniDLNA(this));
}

