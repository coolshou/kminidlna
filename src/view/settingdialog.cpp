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

    m_general = new SettingGeneral(this);
    connect(m_general, SIGNAL(changed()), this, SLOT(onChange()));

    m_minidlna = new SettingMiniDLNA(this);
    connect(m_minidlna, SIGNAL(changed()),this, SLOT(onChange()));

    m_mediaFolders = new SettingMediaFolder(this);
    connect(m_mediaFolders, SIGNAL(changed()), this, SLOT(onChange()));

    m_confFileSettings = new SettingConfFile(this);
    connect(m_confFileSettings, SIGNAL(changed()), this, SLOT(onChange()));

    m_server = new SettingServer(this);
    connect(m_server, SIGNAL(changed()), this, SLOT(onChange()));
}

void SettingDialog::onApply() {
    /*
    KPageWidgetItem* w = currentPage();
    AbstractSettings* tmp = dynamic_cast<AbstractSettings*>(w->widget());
    if (tmp->isChanged()) {
        tmp->applySettings();
        emit settingsChanged();
    }
    */
    m_apply->setEnabled(false);
}

void SettingDialog::onDefault() {
    /*
    KPageWidgetItem* w = currentPage();
    AbstractSettings* tmp = dynamic_cast<AbstractSettings*>(w->widget());
    tmp->setDefaults();
    */
}
void SettingDialog::onChange() {
//     if(!m_apply->isEnabled()){
    m_apply->setEnabled(true);
//     }
}
