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

#ifndef KminiDLNA_H
#define KminiDLNA_H

#include <KMainWindow>
#include <KSystemTrayIcon>
#include <QCloseEvent>
#include <KMenuBar>
#include <KHelpMenu>
#include <KMenu>
#include <KAction>
#include "mainwidget.h"
#include "minidlna_process.h"

class KminiDLNA : public KMainWindow
{
Q_OBJECT
public:
    KminiDLNA();
    virtual ~KminiDLNA();
protected:
    void closeEvent(QCloseEvent *event);
private:
    void initGUI();
    KSystemTrayIcon *systemtray; 
    void initSystemTray();
    KMenuBar *menu;
    void createMenu();
    KHelpMenu *aboutMenu;
    KMenu *mTool;
    KMenu *mAbout;
    MinidlnaProcess *dlnaProcess;
    MainWidget *mw;
    KAction* trayStopStart;
    void loadSettings();
    bool sm_closeToTray;
private slots:
    void showSettings();
    void showMediaDir();
    void onMiniDLNAState(QProcess::ProcessState state);
public slots:
    void systemTrayActived(QSystemTrayIcon::ActivationReason reason);
    void onBtnStopStart();  
    void quitKminiDLNA();
};

#endif // KminiDLNA_H
