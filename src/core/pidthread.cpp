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


#include "pidthread.h"

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
        qDebug() << "qminidlna: minidlna starting " << i++;
        sleep ( 1 );

    }
    emit foundPidFile ( true );
    QFile pidfile ( pathPidFile );
    if ( !pidfile.open ( QIODevice::ReadOnly ) )
    {
        //KMessageBox::information ( 0,tr ( "Error" ), pidfile.errorString() );
        QMessageBox::information ( 0,tr ( "Error" ), pidfile.errorString() );
    }

    QTextStream stream ( &pidfile );
    pid = stream.readLine().toInt();
    pidfile.close();
    qDebug() << "qminidlna: minidlna pid is: " << pid;
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
