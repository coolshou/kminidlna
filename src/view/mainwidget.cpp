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


#include "mainwidget.h"
#include <KLocalizedString>
#include "kminidlna.h"
#include <QDebug>
#include <KIcon>
#include <QIcon>

MainWidget::MainWidget(QWidget *parent):QWidget(parent)
{
    run = false;
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
    btnStopStart = new QPushButton(KIcon("media-playback-start"), i18n("Start"));
    QHBoxLayout *layoutRun = new QHBoxLayout();
//     layoutRun->addWidget(new QPixmap (QIcon(":/images/ikona.png")));
    QLabel* lbla = new QLabel("Test",this);
    lbla->setPixmap(QPixmap(":/images/ikona.png"));
    layoutRun->addWidget(lbla);
    
    layoutRun->addWidget(new QLabel(i18n("minidlna run:")));
    layoutRun->addWidget(kled);
    mainlayout->addLayout(layoutRun);;
    
    mainlayout->addSpacerItem(new QSpacerItem(10,10,QSizePolicy::Maximum, QSizePolicy::Expanding));
    mainlayout->addWidget(btnStopStart);
    connect(btnStopStart, SIGNAL(pressed()), this, SLOT(onBtnPressed()));
}

void MainWidget::onBtnPressed()
{
  emit pressedBtnStopStart();
}

/**
 * set MaintWidget led and btn to stop or start
 */
void MainWidget::setStopStart(bool ss)
{
    if(!btnStopStart->isEnabled()){
	btnStopStart->setEnabled(true);
    }
    if (ss) {
        kled->setColor(Qt::green);
        kled->setState(KLed::On);
        btnStopStart->setText(i18n("Stop"));
	btnStopStart->setIcon(KIcon("media-playback-stop"));
    } else {
        kled->setColor(Qt::gray);
        kled->setState(KLed::Off);
        btnStopStart->setText(i18n("Start"));
	btnStopStart->setIcon(KIcon("media-playback-start"));
    }
}

void MainWidget::setRunning()
{
    btnStopStart->setText(i18n("Starting ..."));
    btnStopStart->setEnabled(false);
    btnStopStart->setIcon(KIcon("media-record"));
}






