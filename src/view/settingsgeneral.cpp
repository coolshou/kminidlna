/*
qminidlna
http://gitorious.org/qminidlna/pages/Home

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


#include "settingsgeneral.h"
#include <QVBoxLayout>
#include <QLabel>
//#include <KLocalizedString>
//#include <KGlobal>
//#include <KConfigGroup>
//#include <KConfig>
#include <QSettings>
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
SettingsGeneral::SettingsGeneral(QWidget* parent, Qt::WindowFlags f): AbstractSettings(parent, f) {
    initGUI();
    loadSettings();
}

SettingsGeneral::~SettingsGeneral() {
}

void SettingsGeneral::initGUI() {
    QVBoxLayout *central = new QVBoxLayout(this);

    QGroupBox *group = new QGroupBox(tr("General"), this);
    QVBoxLayout *ll = new QVBoxLayout(group);
    central->addWidget(group);

    m_closeToTray = new QCheckBox(tr("Close to system tray"), group);
    connect(m_closeToTray, SIGNAL(stateChanged(int)),
            this, SLOT(someChanged()));
    ll->addWidget(m_closeToTray);

    m_runOnStart = new QCheckBox(tr("Run on start"), group);
    connect(m_runOnStart, SIGNAL(stateChanged(int)),
            this, SLOT(someChanged()));
    ll->addWidget(m_runOnStart);

    m_remRun = new QCheckBox(tr("Remember running"), group);
    connect(m_remRun, SIGNAL(stateChanged(int)),
            this, SLOT(someChanged()));
    m_remRun->setEnabled(false);
    ll->addWidget(m_remRun);

    ll->addSpacerItem(new QSpacerItem(40, 400));
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
void SettingsGeneral::applySettings() {
    QSettings config;
    config.beginGroup("General");
    config.setValue("closetotray", m_closeToTray->isChecked());
    config.setValue("runonstart", m_runOnStart->isChecked());
    config.setValue("rememberrun", m_remRun->isChecked());
    config.sync();
    config.endGroup();
    /*
    KConfigGroup config = KGlobal::config()->group("General");
    config.writeEntry("closetotray", m_closeToTray->isChecked());
    config.writeEntry("runonstart", m_runOnStart->isChecked());
    config.writeEntry("rememberrun", m_remRun->isChecked());
    config.sync();
    */
    AbstractSettings::applySettings();
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
void SettingsGeneral::loadSettings() {
    QSettings config;
    config.beginGroup("General");
    m_closeToTray->setChecked(config.value("closetotray", false).toBool());
    m_runOnStart->setChecked(config.value("runonstart", false).toBool());
    m_remRun->setChecked(config.value("rememberrun", false).toBool());
    config.endGroup();
    /*
    KConfigGroup config = KGlobal::config()->group("General");
    m_closeToTray->setChecked(config.readEntry("closetotray", false));
    m_runOnStart->setChecked(config.readEntry("runonstart", false));
    m_remRun->setChecked(config.readEntry("rememberrun", false));
    */
    m_changed = false;
}

void SettingsGeneral::setDefaults() {
    m_closeToTray->setChecked(true);
    m_runOnStart->setChecked(false);
    m_remRun->setChecked(false);
}

