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


#ifndef MINIDLNAPROCESS_H
#define MINIDLNAPROCESS_H
#include <QProcess>
#include <QFile>
#include <QThread>

class PidThread: public QThread
{
    Q_OBJECT;
public:
    explicit PidThread ( QObject* parent = 0 );
    virtual void run();
    void setPathPidFile ( const QString& path );
    int getPid() const;
    QString getPidPath() const;
private:
    int pid;
    QString pathPidFile;
signals:
    void foundPidFile ( bool found );
};

class MinidlnaProcess: public QObject
{
    Q_OBJECT

public:
    MinidlnaProcess();
    virtual ~MinidlnaProcess();
    void minidlnaStart();
    void minidlnaKill();
    bool minidlnaStatus();
    void loadSettings();
private:
    QProcess* minidlna;
    QString minidlnas;
    QStringList arg;
    QString pathPidFile;
    PidThread* t_pid;
    bool scanFile;
    void setArg();

signals:
    void minidlnaStatus ( QProcess::ProcessState state );
private slots:
    void onPidFile ( bool found );
};

#endif // MINIDLNAPROCESS_H
