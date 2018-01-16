/*
QminiDLNA
http://gitorious.org/QminiDLNA/pages/Home

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

#include "qminidlna.h"

#include "mainwidget.h"
#include "mediafolderswidget.h"

#include <QLabel>
//#include <KDE/KLocalizedString>
#include <QLayout>
#include <QDebug>
#include <QtConfig>


//#include <KHelpMenu>
//#include <QMenu>
//#include <KApplication>
#include <QApplication>
//#include <KAboutData>
//#include <KCmdLineArgs>
//#include <KStandardAction>
#include <QAction>
#include <QSettings>
//#include <KConfigDialog>

QminiDLNA::QminiDLNA(QWidget* parent, Qt::WindowFlags f): QMainWindow(parent, f) {
    dlnaProcess = MiniDLNAProcess::getInstance();
    initGUI();
    connect(dlnaProcess, SIGNAL(minidlnaStatus(QProcess::ProcessState)),
            this, SLOT(onMiniDLNAState(QProcess::ProcessState)));
    connect(RESTServer::getInstance(), SIGNAL(notValidKeyCertificate()),
            SLOT(onNotValidCertificateKey()));
    loadSettings();
    initSystemTray();
}

QminiDLNA::~QminiDLNA() {

}

void QminiDLNA::initGUI() {
    mw = new MainWidget(this);
    setCentralWidget(mw);
    createMenu();
    initSystemTray();
    connect(mw, SIGNAL(pressedBtnStopStart()),
            this, SLOT(onBtnStopStart()));
}
void QminiDLNA::initSystemTray() {
    systemtray = new QSystemTrayIcon(QIcon(":/images/qminidlna.png"), this);

    //add MENU
    QMenu* trayMenu = new QMenu("QminiDLNA", this);
    //trayMenu->setTitle(QGlobal::mainComponent().aboutData()->programName());

    trayStopStart = new QAction(QIcon("media-playback-start"), tr("Start minidlna"), trayMenu);
    connect(trayStopStart, SIGNAL(triggered(bool)), this, SLOT(onBtnStopStart()));
    trayMenu->addAction(trayStopStart);

    trayMenu->addSeparator();
    //trayMenu->addAction(QStandardAction::quit(this, SLOT(quitQminiDLNA()), this));
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
    menu = new QMenuBar(this);
    setMenuBar(menu);
    mTool = new QMenu(tr("Tools"), menu);

    QAction *aSetting = new QAction(QIcon("configure"), tr("Configure QminiDLNA"), mTool);
    connect(aSetting, SIGNAL(triggered(bool)), this, SLOT(showSettings()));
    mTool->addAction(aSetting);

    m_actionStartStopRESTServer = new QAction(QIcon("applications-internet"), tr("Start HTTP REST server"), mTool);
    connect(m_actionStartStopRESTServer, SIGNAL(triggered(bool)), this, SLOT(onActionStartStopServer()));
    connect(RESTServer::getInstance(), SIGNAL(run(bool)),
            this, SLOT(onRESTServerRun(bool)));
    mTool->addAction(m_actionStartStopRESTServer);

    mTool->addSeparator();

    //mTool->addAction(KStandardAction::quit(this, SLOT(quitQminiDLNA()), this));

    menu->addMenu(mTool);
    /*
    aboutMenu = new KHelpMenu(menu, KCmdLineArgs::aboutData());
    mAbout = aboutMenu->menu();
    menu->addMenu(aboutMenu->menu());
    */
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
        mw->setStopStart(true);
        systemtray->setIcon(QIcon(":/images/run.png"));
        trayStopStart->setIcon(QIcon("media-playback-stop"));
        trayStopStart->setText(tr("Stop"));
        break;
    case QProcess::Starting:
        mw->setRunning();
        break;
    default:
        mw->setStopStart(false);
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
        m_actionStartStopRESTServer->setText(tr("Stop HTTP REST server"));
    } else {
        m_actionStartStopRESTServer->setText(tr("Start HTTP REST server"));
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



