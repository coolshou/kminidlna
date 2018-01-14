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


#ifndef PIDTHREAD_H
#define PIDTHREAD_H

#include <QObject>
#include <QThread>
#include <QString>
#include <QFile>
#include <QMessageBox>
//#include <KMessageBox>
#include <QDebug>
//#include <KLocalizedString>

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

#endif // PIDTHREAD_H
