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

//TODO add control for changing page to ask if you want save settings


#include "settingsdialog.h"
#include "settingsgeneral.h"
#include <KDE/KLocalizedString>
#include <QDebug>

SettingsDialog::SettingsDialog(QWidget* parent, Qt::WFlags flags): KPageDialog(parent, flags)
{
    initGUI();
}

SettingsDialog::~SettingsDialog()
{

}

void SettingsDialog::onApply()
{
    KPageWidgetItem* w = currentPage();
    AbstractSettings* tmp = dynamic_cast<AbstractSettings*>(w->widget());
    tmp->applySettings();
}

void SettingsDialog::onDefault()
{
    KPageWidgetItem* w = currentPage();
    AbstractSettings* tmp = dynamic_cast<AbstractSettings*>(w->widget());
    tmp->setDefaults();
}

void SettingsDialog::initGUI()
{
    setWindowModality( Qt::WindowModal );
    setButtons(KDialog::Ok | KDialog::Cancel | KDialog::Apply | KDialog::Default);
    setFaceType( KPageDialog::List);
    setWindowTitle(i18n("Configure KminiDLNA"));
    setMinimumSize(450, 250);

    m_general = new SettingsGeneral(this);
    KPageWidgetItem* generalPage = new KPageWidgetItem(m_general, i18n("General"));
    generalPage->setObjectName("gen");
    generalPage->setIcon(KIcon(":/images/ikona.png"));
    addPage(generalPage);

    m_minidlna = new SettingsMiniDLNA(this);
    KPageWidgetItem* minidlnaPage = new KPageWidgetItem(m_minidlna, i18n("MiniDLNA"));
    minidlnaPage->setObjectName("MiniDLNA");
    minidlnaPage->setIcon(KIcon("configure"));
    addPage(minidlnaPage);

    m_server = new SettingsServer(this);
    KPageWidgetItem* serverPage = new KPageWidgetItem(m_server, i18n("Server interface"));
    serverPage->setObjectName("server");
    serverPage->setIcon(KIcon("applications-internet"));
    addPage(serverPage);

    connect(this, SIGNAL(applyClicked()), this, SLOT(onApply()));
    connect(this, SIGNAL(defaultClicked()), this, SLOT(onDefault()));
    connect(this, SIGNAL(okClicked()), this, SLOT(onApply()));
}

