#include "settingminidlna.h"
#include "ui_settingminidlna.h"

#include <QSettings>
#include "../core/minidlna_process.h"

SettingMiniDLNA::SettingMiniDLNA(QWidget *parent, Qt::WindowFlags f) :
    AbstractSettings(parent, f) ,
    ui(new Ui::SettingMiniDLNA)
{
    ui->setupUi(this);
    initGUI();
    loadSettings();
}

SettingMiniDLNA::~SettingMiniDLNA()
{
    delete ui;
}
/**
 * init settings minidlna
 */
void SettingMiniDLNA::initGUI() {

    m_minidlnaPath = ui->m_minidlnaPath;
    connect(m_minidlnaPath, SIGNAL(textChanged(QString)),
            this, SLOT(someChanged()));

    m_browsePath = ui->m_browsePath;
    connect(m_browsePath, SIGNAL(pressed()), this, SLOT(onBrowsePath()));

    //PATH pid file
    m_pidFilePath = ui->m_pidFilePath;
    connect(m_pidFilePath, SIGNAL(textChanged(QString)), this, SLOT(someChanged()));

    m_pidbrowsePath = ui->m_pidbrowsePath;
    connect(m_pidbrowsePath, SIGNAL(pressed()), this, SLOT(onPidBrowsePath()));

    //Conf file group
    //Default path
    m_radioDefaultConfPath = ui->m_radioDefaultConfPath;
    connect(m_radioDefaultConfPath, SIGNAL(toggled(bool)), this, SLOT(fileOptionsChanged()));

    //Global path
    m_radioGlobalConfPath = ui->m_radioGlobalConfPath;
    connect(m_radioGlobalConfPath, SIGNAL(toggled(bool)), this, SLOT(fileOptionsChanged()));

    //User defined
    m_radioUserConfPath = ui->m_radioUserConfPath;
    connect(m_radioUserConfPath, SIGNAL(toggled(bool)), this, SLOT(onToogledUserConfPath(bool)));
    connect(m_radioUserConfPath, SIGNAL(toggled(bool)), this, SLOT(fileOptionsChanged()));

    //Conf file selection

    m_confFilePath = ui->m_confFilePath;
    connect(m_confFilePath, SIGNAL(textChanged(QString)), this, SLOT(fileOptionsChanged()));

    m_browseConfFile = ui->m_browseConfFile;
    connect(m_browseConfFile, SIGNAL(pressed()), this, SLOT(onBrowseConfFile()));

    //Scan file on start
    m_loadFile = ui->m_loadFile;
    connect(m_loadFile, SIGNAL(stateChanged(int)), this, SLOT(someChanged()));

    //Port
    m_port = ui->m_port;
    connect(m_port, SIGNAL(valueChanged(int)),this, SLOT(someChanged()));
}

void SettingMiniDLNA::applySettings() {
    AbstractSettings::applySettings();
    QSettings config;
    config.beginGroup("minidlna");
    config.setValue("minidlnapath", m_minidlnaPath->text());
    config.setValue("pidpath", m_pidFilePath->text());
    config.setValue("scanfile", m_loadFile->isChecked());
    config.setValue("use_conf_file", QVariant(usedConfFile()));
    config.setValue("conf_file_path", m_confFilePath->text());
    config.setValue("minidlna_port", m_port->value());

    /*
    KConfigGroup config = KGlobal::config()->group("minidlna");
    config.writeEntry("minidlnapath", m_minidlnaPath->text());
    config.writeEntry("pidpath", m_pidFilePath->text());
    config.writeEntry("scanfile", m_loadFile->isChecked());
    config.writeEntry("use_conf_file", QVariant(usedConfFile()));
    config.writeEntry("conf_file_path", m_confFilePath->text());
    config.writeEntry("minidlna_port", m_port->value());
    */
    config.sync();
    config.endGroup();
    if (m_fileOptionsChanged) {
        MiniDLNAProcess::getInstance()->configFileChanged();
        m_fileOptionsChanged = false;
    }
}

void SettingMiniDLNA::loadSettings() {
    QSettings config;
    config.beginGroup("minidlna");
    m_minidlnaPath->setText(config.value("minidlnapath", MiniDLNAProcess::MINIDLNA_PATH).toString());
    m_pidFilePath->setText(config.value("pidpath", MiniDLNAProcess::PIDFILE_PATH).toString());
    m_loadFile->setChecked(config.value("scanfile", false).toBool());
    ConfigurationFile::ConfFile conf = ConfigurationFile::ConfFile(config.value("use_conf_file", -1).toInt());
    setUsedConfFile(conf);
    m_confFilePath->setText(config.value("conf_file_path", MiniDLNAProcess::GLOBALCONFFILE_PATH).toString());
    m_port->setValue(config.value("minidlna_port", MiniDLNAProcess::DEFAULTPORT).toInt());
    /*
    KConfigGroup config = KGlobal::config()->group("minidlna");
    m_minidlnaPath->setText(config.readEntry("minidlnapath", MiniDLNAProcess::MINIDLNA_PATH));
    m_pidFilePath->setText(config.readEntry("pidpath", MiniDLNAProcess::PIDFILE_PATH));
    m_loadFile->setChecked(config.readEntry("scanfile", false));
    ConfigurationFile::ConfFile conf = ConfigurationFile::ConfFile(config.readEntry("use_conf_file", -1));
    setUsedConfFile(conf);
    m_confFilePath->setText(config.readEntry("conf_file_path", MiniDLNAProcess::GLOBALCONFFILE_PATH));
    m_port->setValue(config.readEntry("minidlna_port", MiniDLNAProcess::DEFAULTPORT));
    */
    m_changed = false;
}

void SettingMiniDLNA::onBrowsePath() {
    /*TODO
    QUrl url = QFileDialog.getOpenFileUrl(QUrl(m_minidlnaPath->text()), "minidlna", this, tr("minidlna path"));
    //KUrl url = KFileDialog::getOpenUrl(KUrl(m_minidlnaPath->text()), "minidlna", this, tr("minidlna path"));
    if (!url.isEmpty()) {
        m_minidlnaPath->setText(url.path());
    }
    */
}

void SettingMiniDLNA::onPidBrowsePath() {
    /*TODO
//     KUrl url = KFileDialog::getExistingDirectory(KUrl ( m_pidFilePath->text() ), this, tr("Pid file directory"));
    QString path = QFileDialog::getExistingDirectory(QUrl(m_pidFilePath->text()), this, tr("Pid file directory") + "/");
    if (!path.isEmpty()) {
        m_pidFilePath->setText(path);
    }
    */
}

void SettingMiniDLNA::setDefaults() {
    m_minidlnaPath->setText(MiniDLNAProcess::MINIDLNA_PATH);
    m_pidFilePath->setText(MiniDLNAProcess::PIDFILE_PATH);
    m_loadFile->setChecked(true);
    m_radioGlobalConfPath->setChecked(true);
    m_confFilePath->setText(MiniDLNAProcess::GLOBALCONFFILE_PATH);
}

void SettingMiniDLNA::onBrowseConfFile() {
    /*TODO
    QString path = QFileDialog::getOpenFileName(QUrl(m_confFilePath->text()), "", this, tr("Configuration file") + "/");
    if (!path.isEmpty()) {
        m_confFilePath->setText(path);
    }
    */
}

void SettingMiniDLNA::onToogledUserConfPath(bool toogled) {
    if (toogled) {
        m_confFilePath->setEnabled(true);
        m_lblConfFile->setEnabled(true);
    } else {
        m_confFilePath->setEnabled(false);
        m_lblConfFile->setEnabled(false);
    }
}

void SettingMiniDLNA::setUsedConfFile(ConfigurationFile::ConfFile use) {

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

ConfigurationFile::ConfFile SettingMiniDLNA::usedConfFile() {

    if (m_radioDefaultConfPath->isChecked()) {
        return ConfigurationFile::DEFAULT;
    }
    if (m_radioUserConfPath->isChecked()) {
        return ConfigurationFile::USER;
    }
    return ConfigurationFile::GLOBAL;
}

void SettingMiniDLNA::fileOptionsChanged() {
    someChanged();
    m_fileOptionsChanged = true;
}

