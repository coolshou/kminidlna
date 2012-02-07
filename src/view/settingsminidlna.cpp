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
#include "settingsminidlna.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <KLocalizedString>
#include <KGlobal>
#include <KConfigGroup>
#include <KConfig>
#include <QDebug>
#include <KFileDialog>
#include <QGroupBox>
#include <KUrl>
#include "../core/minidlna_process.h"
#include "../core/configurationfile.h"


SettingsMiniDLNA::SettingsMiniDLNA(QWidget* parent, Qt::WindowFlags f) : AbstractSettings(parent, f) {
    initGUI();
    loadSettings();
}

SettingsMiniDLNA::~SettingsMiniDLNA() {

}

/**
 * init settings minidlna
 */
void SettingsMiniDLNA::initGUI() {
    QVBoxLayout* central = new QVBoxLayout(this);

    QGroupBox* groupmini = new QGroupBox(i18n("MiniDLNA"), this);
    central->addWidget(groupmini);

    QVBoxLayout* vl = new QVBoxLayout(groupmini);
    QHBoxLayout* llhMiniDLNAPath =  new QHBoxLayout();

    //PATH to minidlna
    llhMiniDLNAPath->addWidget(new QLabel(i18n("MiniDLNA path:"), groupmini));

    m_minidlnaPath = new QLineEdit("", groupmini);
    connect(m_minidlnaPath, SIGNAL(textChanged(QString)),
            this, SLOT(someChanged()));
    llhMiniDLNAPath->addWidget(m_minidlnaPath);

    m_browsePath = new QToolButton(groupmini);
    m_browsePath->setIcon(KIcon("document-open"));
    connect(m_browsePath, SIGNAL(pressed()),
            this, SLOT(onBrowsePath()));
    llhMiniDLNAPath->addWidget(m_browsePath);

    vl->addLayout(llhMiniDLNAPath);

    //PATH pid file
    QHBoxLayout* llhPIDPath = new QHBoxLayout();
    llhPIDPath->addWidget(new QLabel(i18n("Path to pid file:"), groupmini));

    m_pidFilePath = new QLineEdit("", groupmini);
    connect(m_pidFilePath, SIGNAL(textChanged(QString)),
            this, SLOT(someChanged()));
    llhPIDPath->addWidget(m_pidFilePath);

    m_pidbrowsePath = new QToolButton(groupmini);
    m_pidbrowsePath->setIcon(KIcon("document-open"));
    connect(m_pidbrowsePath, SIGNAL(pressed()), this, SLOT(onPidBrowsePath()));
    llhPIDPath->addWidget(m_pidbrowsePath);

    vl->addLayout(llhPIDPath);

    //Conf file group
    QGroupBox* groupminiconf = new QGroupBox(i18n("MiniDLNA configuration"), this);
    central->addWidget(groupminiconf);
    QVBoxLayout* vlminiconf = new QVBoxLayout(groupminiconf);

    //Default path
    m_radioDefaultConfPath = new QRadioButton(i18n("Use default local MiniDLNA configuration file"));
    m_radioDefaultConfPath->setChecked(true);
    vlminiconf->addWidget(m_radioDefaultConfPath);
    connect(m_radioDefaultConfPath, SIGNAL(toggled(bool)),
            this, SLOT(fileOptionsChanged()));


    //Global path
    m_radioGlobalConfPath = new QRadioButton(i18n("Use global MiniDLNA configuration file (/etc/minidlna.conf)"));
    vlminiconf->addWidget(m_radioGlobalConfPath);
    connect(m_radioGlobalConfPath, SIGNAL(toggled(bool)),
            this, SLOT(fileOptionsChanged()));

    //User defined
    m_radioUserConfPath = new QRadioButton(i18n("Use user defined MiniDLNA configuration file"));
    vlminiconf->addWidget(m_radioUserConfPath);
    connect(m_radioUserConfPath, SIGNAL(toggled(bool)),
            this, SLOT(onToogledUserConfPath(bool)));
    connect(m_radioUserConfPath, SIGNAL(toggled(bool)),
            this, SLOT(fileOptionsChanged()));


    //Conf file selection
    QHBoxLayout* hlconf = new QHBoxLayout();
    m_lblConfFile = new QLabel(i18n("Path to conf file:"), groupmini);
    hlconf->addWidget(m_lblConfFile);

    m_confFilePath = new QLineEdit("", groupminiconf);
    m_confFilePath->setEnabled(false);
    connect(m_confFilePath, SIGNAL(textChanged(QString)),
            this, SLOT(fileOptionsChanged()));
    hlconf->addWidget(m_confFilePath);

    m_browseConfFile = new QToolButton(groupminiconf);
    m_browseConfFile->setIcon(KIcon("document-open"));
    connect(m_browseConfFile, SIGNAL(pressed()),
            this, SLOT(onBrowseConfFile()));
    hlconf->addWidget(m_browseConfFile);

    vlminiconf->addLayout(hlconf);



    //Scan file on start
    m_loadFile = new QCheckBox(i18n("Full rescan on start MiniDLNA"), groupmini);
    connect(m_loadFile, SIGNAL(stateChanged(int)),
            this, SLOT(someChanged()));
    vlminiconf->addWidget(m_loadFile);

    //Port
    QWidget* portWidget = new QWidget(groupmini);
    QHBoxLayout* portLayout = new QHBoxLayout(portWidget);
    portWidget->setLayout(portLayout);
    
    
    QLabel* lblPort = new QLabel(i18n("Port for HTTP traffic MiniDLNA (default 8200)"), portWidget);
    portLayout->addWidget(lblPort);

    m_port = new QSpinBox(portWidget);
    m_port->setMaximum(65000); //FIXME set max int
    m_port->setValue(DEFAULTPORT);
    connect(m_port, SIGNAL(valueChanged(int)),
            this, SLOT(someChanged()));
    portLayout->addWidget(m_port);

    vlminiconf->addWidget(portWidget);

    vlminiconf->addSpacerItem(new QSpacerItem(40, 100));
}


void SettingsMiniDLNA::applySettings() {
    AbstractSettings::applySettings();
    KConfigGroup config = KGlobal::config()->group("minidlna");
    config.writeEntry("minidlnapath", m_minidlnaPath->text());
    config.writeEntry("pidpath", m_pidFilePath->text());
    config.writeEntry("scanfile", m_loadFile->isChecked());
    config.writeEntry("use_conf_file", QVariant(usedConfFile()));
    config.writeEntry("conf_file_path", m_confFilePath->text());
    config.writeEntry("minidlna_port", m_port->value());
    config.sync();

    if (m_fileOptionsChanged) {
        MiniDLNAProcess::getInstance()->configFileChanged();
        m_fileOptionsChanged = false;
    }
}

void SettingsMiniDLNA::loadSettings() {
    KConfigGroup config = KGlobal::config()->group("minidlna");
    m_minidlnaPath->setText(config.readEntry("minidlnapath", MiniDLNA::MINIDLNA_PATH));
    m_pidFilePath->setText(config.readEntry("pidpath", MiniDLNA::PIDFILE_PATH));
    m_loadFile->setChecked(config.readEntry("scanfile", false));
    ConfigurationFile::ConfFile conf = ConfigurationFile::ConfFile(config.readEntry("use_conf_file", -1));
    setUsedConfFile(conf);
    m_confFilePath->setText(config.readEntry("conf_file_path", MiniDLNA::GLOBALCONFFILE_PATH));
    m_port->setValue(config.readEntry("minidlna_port", MiniDLNA::DEFAULTPORT));
    m_changed = false;
}

void SettingsMiniDLNA::onBrowsePath() {
    KUrl url = KFileDialog::getOpenUrl(KUrl(m_minidlnaPath->text()), "minidlna", this, i18n("minidlna path"));
    if (!url.isEmpty()) {
        m_minidlnaPath->setText(url.path());
    }
}

void SettingsMiniDLNA::onPidBrowsePath() {
//     KUrl url = KFileDialog::getExistingDirectory(KUrl ( m_pidFilePath->text() ), this, i18n("Pid file directory"));
    QString path = KFileDialog::getExistingDirectory(KUrl(m_pidFilePath->text()), this, i18n("Pid file directory") + "/");
    if (!path.isEmpty()) {
        m_pidFilePath->setText(path);
    }
}

void SettingsMiniDLNA::setDefaults() {
    m_minidlnaPath->setText(MiniDLNA::MINIDLNA_PATH);
    m_pidFilePath->setText(MiniDLNA::PIDFILE_PATH);
    m_loadFile->setChecked(true);
    m_radioGlobalConfPath->setChecked(true);
    m_confFilePath->setText(MiniDLNA::GLOBALCONFFILE_PATH);
}

void SettingsMiniDLNA::onBrowseConfFile() {
    QString path = KFileDialog::getOpenFileName(KUrl(m_confFilePath->text()), "", this, i18n("Configuration file") + "/");
    if (!path.isEmpty()) {
        m_confFilePath->setText(path);
    }
}

void SettingsMiniDLNA::onToogledUserConfPath(bool toogled) {
    if (toogled) {
        m_confFilePath->setEnabled(true);
        m_lblConfFile->setEnabled(true);
    } else {
        m_confFilePath->setEnabled(false);
        m_lblConfFile->setEnabled(false);
    }
}

void SettingsMiniDLNA::setUsedConfFile(ConfigurationFile::ConfFile use) {

    switch (use) {
    case ConfigurationFile::DEFAULT:
        m_radioDefaultConfPath->setChecked(true);
        break;
    case ConfigurationFile::USER:
        m_radioUserConfPath->setChecked(true);
        break;
    default:
        m_radioGlobalConfPath->setChecked(true);
    }
}

ConfigurationFile::ConfFile SettingsMiniDLNA::usedConfFile() {

    if (m_radioDefaultConfPath->isChecked()) {
        return ConfigurationFile::DEFAULT;
    }
    if (m_radioUserConfPath->isChecked()) {
        return ConfigurationFile::USER;
    }
    return ConfigurationFile::GLOBAL;
}

void SettingsMiniDLNA::fileOptionsChanged() {
    someChanged();
    m_fileOptionsChanged = true;
}








