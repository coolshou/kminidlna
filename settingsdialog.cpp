/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) 2011  Tomáš Poledný <email>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#include "settingsdialog.h"
#include "settingsgeneral.h"
#include <KDE/KLocalizedString>
#include <QDebug>

SettingsDialog::SettingsDialog(QWidget* parent, Qt::WFlags flags): KPageDialog(parent, flags)
{
    setWindowModality( Qt::WindowModal );
    setButtons(KDialog::Ok | KDialog::Cancel | KDialog::Apply);
    setFaceType( KPageDialog::List);
    setWindowTitle(i18n("Configure KminiDLNA"));
    setMinimumSize(450, 250);

    s_general = new SettingsGeneral(this);
    KPageWidgetItem* kp_general = new KPageWidgetItem(s_general, i18n("General"));
    kp_general->setObjectName("gen");
    kp_general->setIcon(KIcon(":/images/ikona.png"));
    addPage(kp_general);

    s_minidlna = new SettingsMiniDLNA(this);
    KPageWidgetItem* kp_minidlna = new KPageWidgetItem(s_minidlna, i18n("minidlna"));
    kp_minidlna->setObjectName("minidlna");
    kp_minidlna->setIcon(KIcon("configure"));
    addPage(kp_minidlna);

    connect(this, SIGNAL(applyClicked()), this, SLOT(onApply()));
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
        s_general->applySettings();
    } else if(w->objectName() == "minidlna"){
        s_minidlna->applySettings();
    }
}
