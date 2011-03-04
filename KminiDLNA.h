#ifndef KminiDLNA_H
#define KminiDLNA_H

#include <KMainWindow>
#include <KSystemTrayIcon>
#include <QCloseEvent>
#include <KMenuBar>
#include <KHelpMenu>
#include <KMenu>
#include "mainwidget.h"
#include "minidlnaprocess.h"

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
    minidlnaProcess *dlnaProcess;
    MainWidget *mw;
    void minidlnaStart(bool start); //set graphic
private slots:
    void showSettings();
public slots:
    void systemTrayActived(QSystemTrayIcon::ActivationReason reason);
    void onBtnStopStart();
    
};

#endif // KminiDLNA_H
