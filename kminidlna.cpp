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
    dlnaProcess = new MinidlnaProcess();
    initGUI();
    connect ( dlnaProcess, SIGNAL ( minidlnaStatus ( QProcess::ProcessState ) ), this, SLOT ( onMiniDLNAState ( QProcess::ProcessState ) ) );
    loadSettings();
}

KminiDLNA::~KminiDLNA()
{
    delete dlnaProcess;
}

void KminiDLNA::initGUI()
{
    mw = new MainWidget ( this );
    setCentralWidget ( mw );
    createMenu();
    initSystemTray();
    connect ( mw, SIGNAL ( pressedBtnStopStart() ), this, SLOT ( onBtnStopStart() ) );
}
void KminiDLNA::initSystemTray()
{
    systemtray = new KSystemTrayIcon ( QIcon ( ":/images/ikona.png" ),this );
    systemtray->show();
    connect ( systemtray, SIGNAL ( activated ( QSystemTrayIcon::ActivationReason ) ), this, SLOT ( systemTrayActived ( QSystemTrayIcon::ActivationReason ) ) );
    connect (systemtray, SIGNAL(quitSelected()), this, SLOT(quitKminiDLNA()));
}

void KminiDLNA::systemTrayActived ( QSystemTrayIcon::ActivationReason reason )
{
    if ( reason == QSystemTrayIcon::Trigger )
    {
        if ( isVisible() == true )
        {
            if ( canBeRestored ( 1 ) )
            {
                restore ( 1, false );
            }
        }
        else
        {
            restore ( 1 );
        }
    }
}

void KminiDLNA::closeEvent ( QCloseEvent* event )
{
    if ( systemtray->isVisible() && sm_closeToTray )
    {
        hide();
        systemtray->showMessage ( i18n ( "KminiDLNA" ), i18n ( "KminiDLNA was minimalized." ),QSystemTrayIcon::Information, 8000 );
        event->ignore();
    }
    else
    {
        KMainWindow::closeEvent ( event );
    }
}

void KminiDLNA::createMenu()
{
    menu = new KMenuBar ( this );
    setMenuBar ( menu );
    mTool = new KMenu ( i18n ( "Tools" ),menu );
    KAction *aSetting = new KAction ( KIcon ( "configure" ), i18n ( "Configure KminiDLNA" ), mTool );
    connect ( aSetting, SIGNAL ( triggered ( Qt::MouseButtons,Qt::KeyboardModifiers ) ), this, SLOT ( showSettings() ) );
    mTool->addAction ( aSetting );
    mTool->addAction ( KStandardAction::quit (this, SLOT(quitKminiDLNA()), this));
    menu->addMenu ( mTool );
    aboutMenu = new KHelpMenu ( menu, KCmdLineArgs::aboutData() );
    mAbout = aboutMenu->menu();
    menu->addMenu ( aboutMenu->menu() );
}

void KminiDLNA::showSettings()
{
    SettingsDialog *sdlg = new SettingsDialog ( this );
    sdlg->exec();
    loadSettings();
}

/**
 * slot onBtnStopStart
 */
void KminiDLNA::onBtnStopStart()
{
    if ( dlnaProcess->minidlnaStatus() )
    {
        dlnaProcess->minidlnaKill();
    }
    else
    {
        dlnaProcess->minidlnaStart();
    }

}

/**
 * Load and set settings
 */
void KminiDLNA::loadSettings()
{
    KConfigGroup config = KGlobal::config()->group ( "General" );
    sm_closeToTray = config.readEntry ( "closetotray", false );


    if ( config.readEntry ( "runonstart", false ) )
    {
        if ( !dlnaProcess->minidlnaStatus() )
        {
            dlnaProcess->minidlnaStart();
        }
    }
    dlnaProcess->loadSettings();
}

/**
 * Slot: If is minidlna status changed
 */
void KminiDLNA::onMiniDLNAState ( QProcess::ProcessState state )
{
    switch ( state )
    {
    case QProcess::Running:
        mw->setStopStart ( true );
        systemtray->setIcon ( QIcon ( ":/images/run.png" ) );
        break;
    case QProcess::Starting:
        mw->setRunning();
        break;
    default:
        mw->setStopStart ( false );
        systemtray->setIcon ( QIcon ( ":/images/ikona.png" ) );
    }
}
void KminiDLNA::quitKminiDLNA()
{
    dlnaProcess->minidlnaKill();
    kapp->quit();
}










