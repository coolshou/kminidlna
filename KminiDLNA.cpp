#include "KminiDLNA.h"

#include <QLabel>
#include <KDE/KLocalizedString>
#include <QLayout>
#include <QDebug>
#include "mainwidget.h"
#include <KHelpMenu>
#include <KMenu>
#include <KApplication>
#include <KAboutData>
#include <KCmdLineArgs>
#include <KAction>
#include <KStandardAction>
#include <KConfigDialog>
#include "settingsdialog.h"

KminiDLNA::KminiDLNA()
{
    dlnaProcess = new minidlnaProcess();
    initGUI();
    loadSettings();
}

KminiDLNA::~KminiDLNA()
{
    delete dlnaProcess;
}

void KminiDLNA::initGUI()
{
    mw = new MainWidget(this);
    setCentralWidget(mw);
    createMenu();
    initSystemTray();
    connect(mw, SIGNAL(pressedBtnStopStart()), this, SLOT(onBtnStopStart()));
}
void KminiDLNA::initSystemTray()
{
    systemtray = new KSystemTrayIcon(QIcon(":/images/ikona.png"),this);
    systemtray->show();
    connect(systemtray, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(systemTrayActived(QSystemTrayIcon::ActivationReason)));
}

void KminiDLNA::systemTrayActived(QSystemTrayIcon::ActivationReason reason)
{
    if (reason == QSystemTrayIcon::Trigger) {
        if (isVisible() == true) {
            if (canBeRestored(1)) {
                restore(1, false);
            }
        } else {
            restore(1);
        }
    }
}

void KminiDLNA::closeEvent(QCloseEvent* event)
{
    if (systemtray->isVisible() && sm_closeToTray) {
        hide();
        systemtray->showMessage(i18n("KminiDLNA"), i18n("KminiDLNA was minimalized."),QSystemTrayIcon::Information, 8000);
        event->ignore();
    } else {
        KMainWindow::closeEvent(event);
    }
}
void KminiDLNA::createMenu()
{
    menu = new KMenuBar(this);
    setMenuBar(menu);
    mTool = new KMenu(i18n("Tools"),menu);
    KAction *aSetting = new KAction(KIcon("configure"), i18n("Configure KminiDLNA"), mTool);
    connect(aSetting, SIGNAL(triggered(Qt::MouseButtons,Qt::KeyboardModifiers)), this, SLOT(showSettings()));
    mTool->addAction(aSetting);
    mTool->addAction(KStandardAction::quit(kapp, SLOT(quit()), mTool));
    menu->addMenu(mTool);
    aboutMenu = new KHelpMenu(menu, KCmdLineArgs::aboutData());
    mAbout = aboutMenu->menu();
    menu->addMenu(aboutMenu->menu());
}

void KminiDLNA::showSettings()
{
    qDebug() << "Not implemented";
    SettingsDialog *sdlg = new SettingsDialog(this);
    sdlg->show();
}

/**
 * set graphic
 */
void KminiDLNA::minidlnaStart(bool start)
{
    if (start) {
        systemtray->setIcon(QIcon(":/images/run.png"));
    } else {
        systemtray->setIcon(QIcon(":/images/ikona.png"));
    }
    mw->setStopStart(start);
}

/**
 * slot onBtnStopStart
 */
void KminiDLNA::onBtnStopStart()
{
    if (dlnaProcess->minidlnaStatus()) {
        qDebug()<< "MELO";
        dlnaProcess->minidlnaKill();
        minidlnaStart(false);
    } else {
        qDebug()<< "NEMELO";
        dlnaProcess->minidlnaStart();
        minidlnaStart(true);
    }

}

void KminiDLNA::loadSettings()
{
    KConfigGroup config = KGlobal::config()->group("General");
    qDebug() << config.readEntry("closetotray", false);;
    sm_closeToTray = config.readEntry("closetotray", false);
}








