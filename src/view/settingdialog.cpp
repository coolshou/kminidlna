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
    setWindowModality(Qt::WindowModal);

    setWindowTitle(tr("Configure qminidlna"));
    setMinimumSize(450, 250);

    m_general = new SettingGeneral(ui->tabGeneral);
    connect(m_general, SIGNAL(changed()), this, SLOT(onChange()));

    m_minidlna = new SettingMiniDLNA(ui->tabMiniDLNA);
    connect(m_minidlna, SIGNAL(changed()),this, SLOT(onChange()));

    m_mediaFolders = new SettingMediaFolder(ui->tabMediaFolder);
    connect(m_mediaFolders, SIGNAL(changed()), this, SLOT(onChange()));

    m_confFileSettings = new SettingConfFile(ui->tabConfig);
    connect(m_confFileSettings, SIGNAL(changed()), this, SLOT(onChange()));

    m_server = new SettingServer(ui->tabServer);
    connect(m_server, SIGNAL(changed()), this, SLOT(onChange()));
    m_apply = ui->m_apply;
}

void SettingDialog::onApply() {
    QWidget *w = ui->tabOptions->currentWidget();

    //KPageWidgetItem* w = currentPage();
    AbstractSettings* tmp = dynamic_cast<AbstractSettings*>(w);
    if (tmp->isChanged()) {
        tmp->applySettings();
        emit settingsChanged();
    }

    m_apply->setEnabled(false);
}

void SettingDialog::onDefault() {
    QWidget *w = ui->tabOptions->currentWidget();

    //KPageWidgetItem* w = currentPage();
    AbstractSettings* tmp = dynamic_cast<AbstractSettings*>(w);
    tmp->setDefaults();
}
void SettingDialog::onChange() {
//     if(!m_apply->isEnabled()){
    m_apply->setEnabled(true);
//     }
}
