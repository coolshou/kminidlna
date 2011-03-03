#include "KminiDLNA.h"

#include <QLabel>
#include <KDE/KLocalizedString>
#include <QLayout>
#include <QDebug>
#include "mainwidget.h"

KminiDLNA::KminiDLNA()
{
    initGUI();
    systemtray = new KSystemTrayIcon();
}

KminiDLNA::~KminiDLNA()
{
  
}

void KminiDLNA::initGUI()
{
    setCentralWidget(new MainWidget());
}