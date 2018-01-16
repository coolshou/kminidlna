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


#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
//#include <KLed>
#include "../libs/qled/qled.h"
#include <QLabel>
#include <QProcess>
#include <QVBoxLayout>
#include <QPushButton>
#include <signal.h>


class MainWidget : public QWidget
{
    Q_OBJECT
public:
    MainWidget(QWidget* parent);
    virtual ~MainWidget();
    void setStopStart(bool ss);
    void setRunning();
private:
    void initGUI();
    QVBoxLayout *mainlayout;
    QLabel *lbl;
    QLed *qled;
    //KLed *kled;
    QPushButton *btnStopStart;
    QString minidlnas;
    QStringList argsm;
    bool run;
signals:
      void pressedBtnStopStart();
private slots:
      void onBtnPressed();
};

#endif // MAINWIDGET_H
