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


#include "mainwidget.h"
#include <KLocalizedString>

MainWidget::MainWidget()
{
    minidlnas  = "/usr/sbin/minidlna";
    argsm << "-d";
    initGUI();
}

MainWidget::~MainWidget()
{

}

void MainWidget::initGUI()
{
    mainlayout = new QVBoxLayout();
    setLayout(mainlayout);
    kled = new KLed(Qt::gray, KLed::Off, KLed::Sunken, KLed::Circular);
    kled->setMaximumSize(20,20);
    btnStopStart = new QPushButton(i18n("Start"));
    QHBoxLayout *layoutRun = new QHBoxLayout();
    layoutRun->addWidget(new QLabel(i18n("minidlna run:")));
    layoutRun->addWidget(kled);
    mainlayout->addLayout(layoutRun);;
    mainlayout->addWidget(btnStopStart);
    minidlna = new QProcess(this);
    connect(btnStopStart, SIGNAL(pressed()), this, SLOT(onBtnStopStart()));
}

void MainWidget::onBtnStopStart()
{
    if (minidlna->state() == QProcess::NotRunning) {
        minidlna->start(minidlnas, argsm);
        kled->setColor(Qt::green);
        kled->setState(KLed::On);
        btnStopStart->setText(i18n("Stop"));
    } else {
        minidlna->kill();
        kled->setColor(Qt::gray);
        kled->setState(KLed::Off);
        btnStopStart->setText(i18n("Start"));

    }

}

