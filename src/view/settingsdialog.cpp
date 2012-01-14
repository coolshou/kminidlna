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


#include "settingsdialog.h"
#include "settingsgeneral.h"
#include <KDE/KLocalizedString>
#include <QDebug>

SettingsDialog::SettingsDialog(QWidget* parent, Qt::WFlags flags): KPageDialog(parent, flags)
{
    setWindowModality( Qt::WindowModal );
    setButtons(KDialog::Ok | KDialog::Cancel | KDialog::Apply | KDialog::Default);
    setFaceType( KPageDialog::List);
    setWindowTitle(i18n("Configure KminiDLNA"));
    setMinimumSize(450, 250);

    m_general = new SettingsGeneral(this);
    KPageWidgetItem* kp_general = new KPageWidgetItem(m_general, i18n("General"));
    kp_general->setObjectName("gen");
    kp_general->setIcon(KIcon(":/images/ikona.png"));
    addPage(kp_general);

    m_minidlna = new SettingsMiniDLNA(this);
    KPageWidgetItem* kp_minidlna = new KPageWidgetItem(m_minidlna, i18n("minidlna"));
    kp_minidlna->setObjectName("minidlna");
    kp_minidlna->setIcon(KIcon("configure"));
    addPage(kp_minidlna);

    connect(this, SIGNAL(applyClicked()), this, SLOT(onApply()));
    connect(this, SIGNAL(defaultClicked()), this, SLOT(onDefault()));
    connect(this, SIGNAL(okClicked()), this, SLOT(onApply()));


}

SettingsDialog::~SettingsDialog()
{

}

void SettingsDialog::onApply()
{
    //TODO add control for changing page to ask if you want save settings
    KPageWidgetItem* w = currentPage();
    if (w->objectName() == "gen") {
        m_general->applySettings();
    } else if (w->objectName() == "minidlna") {
        m_minidlna->applySettings();
    }
}

void SettingsDialog::onDefault()
{
    //TODO remake 
    KPageWidgetItem* w = currentPage();
    if (w->objectName() == "gen") {
        m_general->setDefaults();
    } else if (w->objectName() == "minidlna") {
        m_minidlna->setDefaults();
    }
}

