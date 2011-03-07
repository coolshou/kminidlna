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

#include "minidlnaprocess.h"
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



MinidlnaProcess::MinidlnaProcess()
{
    loadSettings();
    t_pid = new PidThread ( this );
    connect ( t_pid, SIGNAL ( foundPidFile ( bool ) ), this, SLOT ( onPidFile ( bool ) ) );

    minidlna = new QProcess();
//     connect(kapp, SIGNAL(aboutToQuit()), this, SLOT());
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
    QString minidlnapath = config.readEntry ( "minidlnapath", "/usr/sbin/minidlna" );
    if ( QFile::exists ( minidlnapath ) )
    {
        qDebug() << "Change path: " << minidlnapath;
        minidlnas = minidlnapath;
    }
    else
    {

        QFileInfo def ( "/usr/sbin/minidlna" );
        if ( def.exists() && def.isExecutable() )
        {
            qDebug() << "setted default executable path: " << def.absoluteFilePath();
            minidlnas = def.absoluteFilePath();
        }
        else
        {
            //TODO add error mesage
            qDebug() << "minidlna in " << minidlnapath << " was not found";
        }
    }

    QString pidpath = config.readEntry ( "pidpath", "/tmp/" );
    QFileInfo pid ( pidpath );
    if ( pid.isDir() && pid.isWritable() )
    {
        qDebug() << "Change pid directory: " << pidpath;
        pathPidFile = pidpath;
    }
    else
    {
      QFileInfo def("/tmp/");
      if(def.isDir() && def.isWritable()){
	qDebug()<< "setted default pid directory: "<< def.absolutePath();
      }else{
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

    qDebug() << arg[0] << "\t" << arg[1];
}




/**
 *
 *
 *
 * PART THREAD TO LOOK FOR PID FILE
 *
 *
 */
PidThread::PidThread ( QObject* parent ) : QThread ( parent ), pid ( -1 )
{
}

void PidThread::run()
{

    int i = 0;
    while ( !QFile::exists ( pathPidFile ) )
    {
        if ( i>20 )
        {
            emit foundPidFile ( false );
            return;
        }
        qDebug() << "KminiDLNA: minidlna starting " << i++;
        sleep ( 1 );

    }
    emit foundPidFile ( true );
    QFile pidfile ( pathPidFile );
    if ( !pidfile.open ( QIODevice::ReadOnly ) )
    {
        KMessageBox::information ( 0,i18n ( "Error" ), pidfile.errorString() );
    }

    QTextStream stream ( &pidfile );
    pid = stream.readLine().toInt();
    pidfile.close();
    qDebug() << "KminiDLNA: minidlna pid is: " << pid;
}

void PidThread::setPathPidFile ( const QString& path )
{
    pathPidFile = path;
}

int PidThread::getPid() const
{
    return pid;
}

QString PidThread::getPidPath() const
{
    return pathPidFile;
}












