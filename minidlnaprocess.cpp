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


#include "minidlnaprocess.h"
#include <QDebug>
#include <KApplication>

minidlnaProcess::minidlnaProcess()
{
    minidlnas  = "/usr/sbin/minidlna";
    argsm << "-d";
    minidlna = new QProcess();
}

minidlnaProcess::~minidlnaProcess()
{
    if (minidlna->state() == QProcess::Running) {
        minidlna->kill();
    }
    delete minidlna;
}

/**
 * start minidlna a send signal
 */
void minidlnaProcess::minidlnaStart()
{
    qDebug() << "START";
    minidlna->start(minidlnas, argsm);
    emit minidlnaStatus(true);
}


/**
 * kill minidlna a send signal
 */
void minidlnaProcess::minidlnaKill()
{
    qDebug() << "STOP";
    minidlna->kill();
    emit minidlnaStatus(false);
}

/**
 * Status of minidlna process
 * @return true if minidlna runnning, false if not running
 */
bool minidlnaProcess::minidlnaStatus()
{
    if(minidlna->state()==QProcess::NotRunning){
      return false;
    }else{
      return true;
    }
}






