/*
QminiDLNA
http://github.com/coolshou/qminidlna

Copyright (C) 2018 jimmy

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

#include "qminidlna.h"
#include "ui_qminidlna.h"

#include "mediafolderswidget.h"

#include <QDebug>
#include <QtConfig>
#include <QApplication>
#include <QAction>
#include <QSettings>

QminiDLNA::QminiDLNA(QWidget* parent, Qt::WindowFlags f):
    QMainWindow(parent, f),
    ui(new Ui::QminiDLNA)
{
    ui->setupUi(this);
    dlnaProcess = MiniDLNAProcess::getInstance();
    initGUI();
    connect(dlnaProcess, SIGNAL(minidlnaStatus(QProcess::ProcessState)),
            this, SLOT(onMiniDLNAState(QProcess::ProcessState)));
    connect(RESTServer::getInstance(), SIGNAL(notValidKeyCertificate()),
            SLOT(onNotValidCertificateKey()));
    loadSettings();
    initSystemTray();
}

QminiDLNA::~QminiDLNA()
{

}

void QminiDLNA::initGUI()
{
    //mw = new MainWidget(this);
    //setCentralWidget(mw);
    qled = new QLed(this);
    qled->setOffColor(QLed::ledColor::Grey);
    qled->setOnColor(QLed::ledColor::Green);
    qled->setShape(QLed::ledShape::Rounded);
    qled->setMaximumSize(20,20);
    ui->layoutRun->addWidget(qled);

    createMenu();
    initSystemTray();
    connect(ui->btnStopStart, SIGNAL(toggled(bool)), this, SLOT(onBtnStopStart()));
    //connect(mw, SIGNAL(pressedBtnStopStart()), this, SLOT(onBtnStopStart()));
}
void QminiDLNA::initSystemTray()
{
    systemtray = new QSystemTrayIcon(QIcon(":/images/qminidlna.png"), this);

    //add MENU
    QMenu* trayMenu = new QMenu("QminiDLNA", this);
    //trayMenu->setTitle(QGlobal::mainComponent().aboutData()->programName());

    trayStopStart = new QAction(QIcon("media-playback-start"), tr("Start minidlna"), trayMenu);
    connect(trayStopStart, SIGNAL(triggered(bool)), this, SLOT(onBtnStopStart()));
    trayMenu->addAction(trayStopStart);

    trayMenu->addSeparator();

    actQuit = new QAction(QIcon("quit"), tr("Quit"), trayMenu);
    connect(actQuit, SIGNAL(triggered(bool)), this, SLOT(quitQminiDLNA()));
    trayMenu->addAction(actQuit);

    systemtray->setContextMenu(trayMenu);

    systemtray->show();
    connect(systemtray, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(systemTrayActived(QSystemTrayIcon::ActivationReason)));
    //connect(systemtray, SIGNAL(quitSelected()), this, SLOT(quitQminiDLNA()));
}

void QminiDLNA::systemTrayActived(QSystemTrayIcon::ActivationReason reason) {
    if(reason == QSystemTrayIcon::Trigger) {
        if(isVisible() == true) {
            /*
            if(canBeRestored(1)) {
                restore(1, false);
            }
        } else {
            restore(1);
            */
        }
    }
}

void QminiDLNA::closeEvent(QCloseEvent* event) {
    if(systemtray->isVisible() && m_closeToTray) {
        hide();
        systemtray->showMessage(tr("QminiDLNA"), tr("QminiDLNA was minimalized."), QSystemTrayIcon::Information, 8000);
        event->ignore();
    } else {
        QMainWindow::closeEvent(event);
    }
}

void QminiDLNA::createMenu() {
    connect(ui->actionconfigure, SIGNAL(triggered(bool)), this, SLOT(showSettings()));
    connect(ui->actionStart_HTTP_REST_server, SIGNAL(triggered(bool)), this, SLOT(onActionStartStopServer()));
    connect(RESTServer::getInstance(), SIGNAL(run(bool)), this, SLOT(onRESTServerRun(bool)));
    connect(ui->actionQuit, SIGNAL(triggered(bool)), this , SLOT(quitQminiDLNA()));
    //TODO: about dialog
    //connect(ui->actionAbout, SIGNAL(triggered(bool)), this, SLOT(about()));
}

void QminiDLNA::showSettings() {
    SettingDialog *sdlg = new SettingDialog(this);

    //SettingsDialog *sdlg = new SettingsDialog(this);
    connect(sdlg, SIGNAL(settingsChanged()), this, SLOT(onSettingsChanged()));
    sdlg->exec();
    disconnect(sdlg);
    delete sdlg;

}

/**
 * slot onBtnStopStart
 */
void QminiDLNA::onBtnStopStart() {
    if(dlnaProcess->minidlnaStatus()) {
        dlnaProcess->minidlnaKill();
    } else {
        dlnaProcess->minidlnaStart();
    }
}

/**
 * Load and set settings
 */
void QminiDLNA::loadSettings() {
    QSettings config;
    config.beginGroup("General");
    m_closeToTray = config.value("closetotray", false).toBool();
    /*
    KConfigGroup config = KGlobal::config()->group("General");
    m_closeToTray = config.readEntry("closetotray", false);
    */
    config.endGroup();
    systemtray->setVisible(m_closeToTray);

}

/**
 * Slot: If is minidlna status changed
 */
void QminiDLNA::onMiniDLNAState(QProcess::ProcessState state) {
    switch(state) {
    case QProcess::Running:
        setStopStart(true);
        systemtray->setIcon(QIcon(":/images/run.png"));
        trayStopStart->setIcon(QIcon("media-playback-stop"));
        trayStopStart->setText(tr("Stop"));
        break;
    case QProcess::Starting:
        setRunning();
        break;
    default:
        setStopStart(false);
        systemtray->setIcon(QIcon(":/images/qminidlna.png"));
        trayStopStart->setIcon(QIcon("media-playback-start"));
        trayStopStart->setText(tr("Start"));
    }
}
void QminiDLNA::quitQminiDLNA() {
    dlnaProcess->minidlnaKill();
    qApp->quit();
}

void QminiDLNA::onActionStartStopServer() {
    RESTServer* server = RESTServer::getInstance();
    if(server->isRuning()) {
        server->stopServer();
    } else {
	server->loadConfig();
        server->startServer();
    }
}

void QminiDLNA::onRESTServerRun(bool run) {
    if(run) {
        ui->actionStart_HTTP_REST_server->setText(tr("Stop HTTP REST server"));
        //m_actionStartStopRESTServer->setText(tr("Stop HTTP REST server"));
    } else {
        ui->actionStart_HTTP_REST_server->setText(tr("Start HTTP REST server"));
        //m_actionStartStopRESTServer->setText(tr("Start HTTP REST server"));
    }
}

void QminiDLNA::onSettingsChanged() {
    loadSettings();
}

void QminiDLNA::onNotValidCertificateKey() {
    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.setWindowTitle("Cannot start REST server");
    msgBox.setText("Certificate or private key is not valid.\nCheck configuration.");
    msgBox.exec();
}

/**
 * set QminiDLNA led and btn to stop or start
 */
void QminiDLNA::setStopStart(bool ss)
{
    if(!ui->btnStopStart->isEnabled()){
        ui->btnStopStart->setEnabled(true);
    }
    if (ss) {
        qled->setValue(true);
        //kled->setColor(Qt::green);
        //kled->setState(KLed::On);
        ui->btnStopStart->setText(tr("Stop"));
        ui->btnStopStart->setIcon(QIcon("media-playback-stop"));
    } else {
        qled->setValue(false);
        //kled->setColor(Qt::gray);
        //kled->setState(KLed::Off);
        ui->btnStopStart->setText(tr("Start"));
        ui->btnStopStart->setIcon(QIcon("media-playback-start"));
    }
}

void QminiDLNA::setRunning()
{
    ui->btnStopStart->setText(tr("Starting ..."));
    ui->btnStopStart->setEnabled(false);
    ui->btnStopStart->setIcon(QIcon("media-record"));
}

