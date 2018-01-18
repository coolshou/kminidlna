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

#ifndef QminiDLNA_H
#define QminiDLNA_H

//#include "mainwidget.h"
#include "../core/minidlna_process.h"
#include "../server/restserver.h"
#include "settingdialog.h"
#include "../libs/qled/qled.h"

#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QCloseEvent>
#include <QMenuBar>
#include <QMenu>
#include <QAction>

namespace Ui {
class QminiDLNA;
}

class QminiDLNA : public QMainWindow
{
Q_OBJECT
public:
    explicit QminiDLNA(QWidget* parent = 0, Qt::WindowFlags f = 0);
    virtual ~QminiDLNA();
    void setStopStart(bool ss);
    void setRunning();

protected:
    void closeEvent(QCloseEvent *event);
private:
    void initGUI();
    Ui::QminiDLNA *ui;
    QLed *qled;
    QSystemTrayIcon *systemtray;
    void initSystemTray();
    //QMenuBar *menu;
    void createMenu();
    //KHelpMenu *aboutMenu;
    //QMenu *mTool;
    //QMenu *mAbout;
    MiniDLNAProcess *dlnaProcess;
    //QAction* m_actionStartStopRESTServer;
    //MainWidget *mw;
    QAction* trayStopStart;
    QAction* actQuit;
    void loadSettings();
    bool m_closeToTray;
    
private slots:
    void showSettings();
    void onMiniDLNAState(QProcess::ProcessState state);
public slots:
    void systemTrayActived(QSystemTrayIcon::ActivationReason reason);
    void onBtnStopStart();  
    void quitQminiDLNA();
    void onActionStartStopServer();
    void onRESTServerRun(bool);
    void onSettingsChanged();
        void onNotValidCertificateKey();
};

#endif // QminiDLNA_H
