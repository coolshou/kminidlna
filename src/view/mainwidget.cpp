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


#include "mainwidget.h"
//#include <KLocalizedString>
#include "qminidlna.h"
#include <QDebug>
//#include <QIcon>
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
    qled = new QLed(this);
    qled->setOffColor(QLed::ledColor::Grey);
    qled->setOnColor(QLed::ledColor::Green);
    qled->setShape(QLed::ledShape::Rounded);
    qled->setMaximumSize(20,20);
    btnStopStart = new QPushButton(QIcon("media-playback-start"), tr("Start"));

    QHBoxLayout *layoutRun = new QHBoxLayout();
    QLabel* lbla = new QLabel("Test",this);
    lbla->setPixmap(QPixmap(":/images/qminidlna.png"));
    layoutRun->addWidget(lbla);
    
    layoutRun->addWidget(new QLabel(tr("minidlna run:")));
    layoutRun->addWidget(qled);
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
        qled->setValue(true);
        //kled->setColor(Qt::green);
        //kled->setState(KLed::On);
        btnStopStart->setText(tr("Stop"));
        btnStopStart->setIcon(QIcon("media-playback-stop"));
    } else {
        qled->setValue(false);
        //kled->setColor(Qt::gray);
        //kled->setState(KLed::Off);
        btnStopStart->setText(tr("Start"));
        btnStopStart->setIcon(QIcon("media-playback-start"));
    }
}

void MainWidget::setRunning()
{
    btnStopStart->setText(tr("Starting ..."));
    btnStopStart->setEnabled(false);
    btnStopStart->setIcon(QIcon("media-record"));
}






