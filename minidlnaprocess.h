/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) 2011  Tomáš Poledný <email>

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
private:
    QProcess* minidlna;
    QString minidlnas;
    QStringList arg;
    QString pathPidFile;
    PidThread* t_pid;

signals:
    void minidlnaStatus ( QProcess::ProcessState state );
private slots:
    void onPidFile ( bool found );
};

#endif // MINIDLNAPROCESS_H
