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


#include "settingsgeneral.h"
#include <QVBoxLayout>
#include <QLabel>
#include <KLocalizedString>
#include <KGlobal>
#include <KConfigGroup>
#include <KConfig>
#include <QDebug>

SettingsGeneral::SettingsGeneral(QWidget* parent, Qt::WindowFlags f): QWidget(parent, f)
{
    initGUI();
}

SettingsGeneral::~SettingsGeneral()
{

}

void SettingsGeneral::initGUI()
{
    QVBoxLayout *ll = new QVBoxLayout(this);
    setLayout(ll);
    m_closeToTray = new QCheckBox(i18n("Close to system tray"), this);
    ll->addWidget(m_closeToTray);
    loadSettings();
    
}

void SettingsGeneral::applySettings()
{
    KConfigGroup config = KGlobal::config()->group( "General" );
    config.writeEntry("closetotray", m_closeToTray->isChecked());
    config.sync();
}

void SettingsGeneral::loadSettings()
{
    
    KConfigGroup config = KGlobal::config()->group( "General" );
    m_closeToTray->setChecked(config.readEntry("closetotray", false));
}


