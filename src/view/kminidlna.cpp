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

#include "kminidlna.h"

#include "mainwidget.h"
#include "mediafolderswidget.h"

#include <QLabel>
#include <KDE/KLocalizedString>
#include <QLayout>
#include <QDebug>
#include <KHelpMenu>
#include <KMenu>
#include <KApplication>
#include <KAboutData>
#include <KCmdLineArgs>
#include <KAction>
#include <KStandardAction>
#include <KConfigDialog>

KminiDLNA::KminiDLNA(QWidget* parent, Qt::WindowFlags f): KMainWindow(parent, f) {
    dlnaProcess = MiniDLNAProcess::getInstance();
    initGUI();
    connect(dlnaProcess, SIGNAL(minidlnaStatus(QProcess::ProcessState)),
            this, SLOT(onMiniDLNAState(QProcess::ProcessState)));
    connect(RESTServer::getInstance(), SIGNAL(notValidKeyCertificate()),
            SLOT(onNotValidCertificateKey()));
    loadSettings();
}

KminiDLNA::~KminiDLNA() {

}

void KminiDLNA::initGUI() {
    mw = new MainWidget(this);
    setCentralWidget(mw);
    createMenu();
    initSystemTray();
    connect(mw, SIGNAL(pressedBtnStopStart()),
            this, SLOT(onBtnStopStart()));
}
void KminiDLNA::initSystemTray() {
    systemtray = new KSystemTrayIcon(QIcon(":/images/ikona.png"), this);

    //add MENU
    KMenu* trayMenu = new KMenu("KminiDLNA", this);
    trayMenu->addTitle(QIcon(":/images/ikona.png"), KGlobal::mainComponent().aboutData()->programName());

    trayStopStart = new KAction(KIcon("media-playback-start"), i18n("Start minidlna"), trayMenu);
    connect(trayStopStart, SIGNAL(triggered(Qt::MouseButtons, Qt::KeyboardModifiers)),
            this, SLOT(onBtnStopStart()));
    trayMenu->addAction(trayStopStart);

    trayMenu->addSeparator();
    trayMenu->addAction(KStandardAction::quit(this, SLOT(quitKminiDLNA()), this));
    systemtray->setContextMenu(trayMenu);

    systemtray->show();
    connect(systemtray, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(systemTrayActived(QSystemTrayIcon::ActivationReason)));
    connect(systemtray, SIGNAL(quitSelected()),
            this, SLOT(quitKminiDLNA()));
}

void KminiDLNA::systemTrayActived(QSystemTrayIcon::ActivationReason reason) {
    if(reason == QSystemTrayIcon::Trigger) {
        if(isVisible() == true) {
            if(canBeRestored(1)) {
                restore(1, false);
            }
        } else {
            restore(1);
        }
    }
}

void KminiDLNA::closeEvent(QCloseEvent* event) {
    if(systemtray->isVisible() && m_closeToTray) {
        hide();
        systemtray->showMessage(i18n("KminiDLNA"), i18n("KminiDLNA was minimalized."), QSystemTrayIcon::Information, 8000);
        event->ignore();
    } else {
        KMainWindow::closeEvent(event);
    }
}

void KminiDLNA::createMenu() {
    menu = new KMenuBar(this);
    setMenuBar(menu);
    mTool = new KMenu(i18n("Tools"), menu);

    KAction *aSetting = new KAction(KIcon("configure"), i18n("Configure KminiDLNA"), mTool);
    connect(aSetting, SIGNAL(triggered(Qt::MouseButtons, Qt::KeyboardModifiers)),
            this, SLOT(showSettings()));
    mTool->addAction(aSetting);

    m_actionStartStopRESTServer = new KAction(KIcon("applications-internet"), i18n("Start HTTP REST server"), mTool);
    connect(m_actionStartStopRESTServer, SIGNAL(triggered(Qt::MouseButtons, Qt::KeyboardModifiers)),
            this, SLOT(onActionStartStopServer()));
    connect(RESTServer::getInstance(), SIGNAL(run(bool)),
            this, SLOT(onRESTServerRun(bool)));
    mTool->addAction(m_actionStartStopRESTServer);

    mTool->addSeparator();

    mTool->addAction(KStandardAction::quit(this, SLOT(quitKminiDLNA()), this));

    menu->addMenu(mTool);
    aboutMenu = new KHelpMenu(menu, KCmdLineArgs::aboutData());
    mAbout = aboutMenu->menu();
    menu->addMenu(aboutMenu->menu());
}

void KminiDLNA::showSettings() {
    SettingsDialog *sdlg = new SettingsDialog(this);
    connect(sdlg, SIGNAL(settingsChanged()),
            this, SLOT(onSettingsChanged()));
    sdlg->exec();
    disconnect(sdlg);
    delete sdlg;

}

/**
 * slot onBtnStopStart
 */
void KminiDLNA::onBtnStopStart() {
    if(dlnaProcess->minidlnaStatus()) {
        dlnaProcess->minidlnaKill();
    } else {
        dlnaProcess->minidlnaStart();
    }
}

/**
 * Load and set settings
 */
void KminiDLNA::loadSettings() {
    KConfigGroup config = KGlobal::config()->group("General");
    m_closeToTray = config.readEntry("closetotray", false);
    systemtray->setVisible(m_closeToTray);

}

/**
 * Slot: If is minidlna status changed
 */
void KminiDLNA::onMiniDLNAState(QProcess::ProcessState state) {
    switch(state) {
    case QProcess::Running:
        mw->setStopStart(true);
        systemtray->setIcon(QIcon(":/images/run.png"));
        trayStopStart->setIcon(KIcon("media-playback-stop"));
        trayStopStart->setText(i18n("Stop"));
        break;
    case QProcess::Starting:
        mw->setRunning();
        break;
    default:
        mw->setStopStart(false);
        systemtray->setIcon(QIcon(":/images/ikona.png"));
        trayStopStart->setIcon(KIcon("media-playback-start"));
        trayStopStart->setText(i18n("Start"));
    }
}
void KminiDLNA::quitKminiDLNA() {
    dlnaProcess->minidlnaKill();
    kapp->quit();
}

void KminiDLNA::onActionStartStopServer() {
    RESTServer* server = RESTServer::getInstance();
    if(server->isRuning()) {
        server->stopServer();
    } else {
	server->loadConfig();
        server->startServer();
    }
}

void KminiDLNA::onRESTServerRun(bool run) {
    if(run) {
        m_actionStartStopRESTServer->setText(i18n("Stop HTTP REST server"));
    } else {
        m_actionStartStopRESTServer->setText(i18n("Start HTTP REST server"));
    }
}

void KminiDLNA::onSettingsChanged() {
    loadSettings();
}

void KminiDLNA::onNotValidCertificateKey() {
    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.setWindowTitle("Cannot start REST server");
    msgBox.setText("Certificate or private key is not valid.\nCheck configuration.");
    msgBox.exec();
}



