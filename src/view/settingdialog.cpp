#include "settingdialog.h"
#include "ui_settingdialog.h"

SettingDialog::SettingDialog(QWidget *parent,  Qt::WindowFlags flags) :
    QDialog(parent, flags),
    m_wasNo(false),
    ui(new Ui::SettingDialog)
{
    ui->setupUi(this);
    initGUI();
}

SettingDialog::~SettingDialog()
{
    delete ui;
}

void SettingDialog::initGUI() {
    m_general = new SettingGeneral(ui->tabGeneral);
    ui->tabGeneralLayout->addWidget(m_general);
    connect(m_general, SIGNAL(changed()), this, SLOT(onChange()));

    m_minidlna = new SettingMiniDLNA(ui->tabMiniDLNA);
    ui->tabMiniDLNALayout->addWidget(m_minidlna);
    connect(m_minidlna, SIGNAL(changed()),this, SLOT(onChange()));

    m_mediaFolders = new SettingMediaFolder(ui->tabMediaFolder);
    ui->tabMediaFolderLayout->addWidget(m_mediaFolders);
    connect(m_mediaFolders, SIGNAL(changed()), this, SLOT(onChange()));

    m_confFileSettings = new SettingConfFile(ui->tabConfig);
    ui->tabConfigLayout->addWidget(m_confFileSettings);
    connect(m_confFileSettings, SIGNAL(changed()), this, SLOT(onChange()));

    m_server = new SettingServer(ui->tabServer);
    ui->tabServerLayout->addWidget(m_server);
    connect(m_server, SIGNAL(changed()), this, SLOT(onChange()));
    m_apply = ui->m_apply;
}

void SettingDialog::onApply() {
    QWidget *w = ui->tabOptions->currentWidget();
    AbstractSettings* tmp = dynamic_cast<AbstractSettings*>(w);
    if (tmp->isChanged()) {
        tmp->applySettings();
        emit settingsChanged();
    }

    m_apply->setEnabled(false);
}

void SettingDialog::onDefault() {
    QWidget *w = ui->tabOptions->currentWidget();
    AbstractSettings* tmp = dynamic_cast<AbstractSettings*>(w);
    tmp->setDefaults();
}
void SettingDialog::onChange() {
//     if(!m_apply->isEnabled()){
    m_apply->setEnabled(true);
//     }
}
