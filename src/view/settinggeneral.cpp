#include "settinggeneral.h"
#include "ui_settinggeneral.h"

#include <QSettings>

SettingGeneral::SettingGeneral(QWidget *parent, Qt::WindowFlags f ) :
    AbstractSettings(parent, f),
    ui(new Ui::SettingGeneral)
{
    ui->setupUi(this);
    initGUI();
    loadSettings();

}

SettingGeneral::~SettingGeneral()
{
    delete ui;
}

void SettingGeneral::initGUI() {
    m_closeToTray = ui->m_closeToTray;
    m_runOnStart = ui->m_runOnStart;
    m_remRun = ui->m_remRun;

    connect(m_closeToTray, SIGNAL(stateChanged(int)), this, SLOT(someChanged()));
    connect(m_runOnStart, SIGNAL(stateChanged(int)), this, SLOT(someChanged()));
    connect(m_remRun, SIGNAL(stateChanged(int)), this, SLOT(someChanged()));
    m_remRun->setEnabled(false);
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
void SettingGeneral::applySettings() {
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
void SettingGeneral::loadSettings() {
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

void SettingGeneral::setDefaults() {
    m_closeToTray->setChecked(true);
    m_runOnStart->setChecked(false);
    m_remRun->setChecked(false);
}
