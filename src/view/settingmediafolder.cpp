#include "settingmediafolder.h"
#include "ui_settingmediafolder.h"
#include "../core/minidlna_process.h"

SettingMediaFolder::SettingMediaFolder(QWidget *parent, Qt::WindowFlags f) :
    AbstractSettings(parent, f),
    ui(new Ui::SettingMediaFolder)
{
    ui->setupUi(this);
    initGUI();
}

SettingMediaFolder::~SettingMediaFolder()
{
    delete ui;
}

void SettingMediaFolder::loadSettings() {

}

void SettingMediaFolder::setDefaults() {

}

void SettingMediaFolder::applySettings() {
    AbstractSettings::applySettings();
    m_mediaFoldersWidget->saveModel();
}

void SettingMediaFolder::initGUI() {
    m_mediaFoldersWidget = new MediaFoldersWidget(ui->mediaFolderGroup);

    ui->mediaFolderGroupLayout->addWidget(m_mediaFoldersWidget);
    connect(m_mediaFoldersWidget, SIGNAL(modelChanged()),
            this, SLOT(someChanged()));
    connect(MiniDLNAProcess::getInstance(), SIGNAL(configurationFileChanged()),
            m_mediaFoldersWidget, SLOT(reload()));
}


