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


#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <KLed>
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
    QProcess *minidlna;
    KLed *kled;
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
