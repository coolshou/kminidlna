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
#include <QGroupBox>

/**
 * Page settings widget: General settings
 *
 * Use slot applySettings for save settings
 *
 * Implemented settings:
 * 	closetotray (false)
 * 	runonstart  (false)
 * 	rememberrun (false)
 *
 */
SettingsGeneral::SettingsGeneral(QWidget* parent, Qt::WindowFlags f): QWidget(parent, f)
{
    initGUI();
}

SettingsGeneral::~SettingsGeneral()
{

}

/**
 *
 * Load Widget gui
 *
 */
void SettingsGeneral::initGUI()
{


    QVBoxLayout *central = new QVBoxLayout(this);
    QGroupBox *group = new QGroupBox(i18n("General"), this);
    QVBoxLayout *ll = new QVBoxLayout(group);
    central->addWidget(group);
    group->setLayout(ll);
//     setLayout(central);
//     setLayout(ll);
    m_closeToTray = new QCheckBox(i18n("Close to system tray"), group);
    ll->addWidget(m_closeToTray);

    m_runOnStart = new QCheckBox(i18n("Run on start"), group);
    ll->addWidget(m_runOnStart);

    m_remRun = new QCheckBox(i18n("Remember running"), group);
    m_remRun->setEnabled(false);
    ll->addWidget(m_remRun);

    ll->addSpacerItem(new QSpacerItem(40, 200));

    loadSettings();

}

/**
 *
 * Save settings:
 *
 * Implemented settings:
 * 	closetotray (false)
 * 	runonstart  (false)
 * 	rememberrun (false)
 */
void SettingsGeneral::applySettings()
{
    KConfigGroup config = KGlobal::config()->group( "General" );
    config.writeEntry("closetotray", m_closeToTray->isChecked());
    config.writeEntry("runonstart", m_runOnStart->isChecked());
    config.writeEntry("rememberrun", m_remRun->isChecked());
    config.sync();
}

/**
 *
 * Load settings only for checkbox etc. !!!
 *
 * Implemented settings:
 * 	closetotray (false)
 * 	runonstart  (false)
 * 	rememberrun (false)
 */
void SettingsGeneral::loadSettings()
{

    KConfigGroup config = KGlobal::config()->group( "General" );
    m_closeToTray->setChecked(config.readEntry("closetotray", false));
    m_runOnStart->setChecked(config.readEntry("runonstart", false));
    m_remRun->setChecked(config.readEntry("rememberrun", false));
}


