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

#include "mainwidget.h"
#include "../core/minidlna_process.h"
#include "../server/restserver.h"
#include "settingsdialog.h"
#include "mediafoldersdialog.h"

#include <KMainWindow>
#include <KSystemTrayIcon>
#include <QCloseEvent>
#include <KMenuBar>
#include <KHelpMenu>
#include <KMenu>
#include <KAction>

class KminiDLNA : public KMainWindow
{
Q_OBJECT
public:
    explicit KminiDLNA(QWidget* parent = 0, Qt::WindowFlags f = 0);
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
    KAction* m_actionStartStopRESTServer;
    MainWidget *mw;
    KAction* trayStopStart;
    void loadSettings();
    bool m_closeToTray;
    
private slots:
    void showSettings();
    void onMiniDLNAState(QProcess::ProcessState state);
public slots:
    void systemTrayActived(QSystemTrayIcon::ActivationReason reason);
    void onBtnStopStart();  
    void quitKminiDLNA();
    void onActionStartStopServer();
    void onRESTServerRun(bool);
    void setFolders();
};

#endif // KminiDLNA_H
