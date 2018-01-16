#include "settingconffile.h"
#include "ui_settingconffile.h"

#include "../core/minidlna_process.h"
#include "../core/configurationfile.h"
#include <QInputDialog>

SettingConfFile::SettingConfFile(QWidget *parent, Qt::WindowFlags f) :
    AbstractSettings(parent, f),
    m_albumArtNamesModel(0),
    m_actualConfFile(0),
    ui(new Ui::SettingConfFile)
{
    ui->setupUi(this);
    initGUI();
    loadSettings();
    connect(MiniDLNAProcess::getInstance(), SIGNAL(configurationFileChanged()),
            this, SLOT(onConfigFileChanged()));
}

SettingConfFile::~SettingConfFile()
{
    delete ui;
    if (m_albumArtNamesModel != 0) {
        delete m_albumArtNamesModel;
    }
}

void SettingConfFile::applySettings() {

    QStringList list = m_albumArtNamesModel->stringList();
    if (m_actualConfFile != 0) {
        m_actualConfFile->saveAlbumArtNames(list);
    } else {
        qDebug() << "SettingsConfFile: Not exist actual conf file object!";
    }
    AbstractSettings::applySettings();

}

void SettingConfFile::loadSettings() {
    loadModel();
}

void SettingConfFile::setDefaults() {

}

void SettingConfFile::initGUI() {
    m_add = ui->m_add;
    connect(m_add, SIGNAL(clicked(bool)), this, SLOT(onAddButtonClicked()));
    m_remove = ui->m_remove;
    connect(m_remove, SIGNAL(clicked(bool)), this, SLOT(onRemoveButtonClicked()));
    connect(m_remove, SIGNAL(clicked(bool)), this, SLOT(someChanged()));

}

void SettingConfFile::loadModel() {
    m_actualConfFile = MiniDLNAProcess::getInstance()->configFile();
    m_albumArtNamesList = m_actualConfFile->albumArtNames();
    if (m_albumArtNamesModel != 0) {
        disconnect(m_albumArtNamesModel);
        delete m_albumArtNamesModel;
    }
    m_albumArtNamesModel = new QStringListModel(m_albumArtNamesList);
    connect(m_albumArtNamesModel, SIGNAL(dataChanged(QModelIndex, QModelIndex)),
            this, SLOT(someChanged()));

    m_albumArtNamesControllWidget = ui->m_albumArtNamesControllWidget;
    m_albumArtNamesView = ui->m_albumArtNamesView;
    m_albumArtNamesView->setModel(m_albumArtNamesModel);
    if (m_actualConfFile->isWritable()) {
        m_albumArtNamesControllWidget->setEnabled(true);
        m_albumArtNamesView->setEditTriggers(QAbstractItemView::DoubleClicked);
    } else {
        m_albumArtNamesControllWidget->setEnabled(false);
        m_albumArtNamesView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    }
    m_changed = false;

}

void SettingConfFile::onAddButtonClicked() {
    bool ok;
    QString text = QInputDialog::getText(this, tr("Add new album art name"),
                                         tr("New album art name:"), QLineEdit::Normal,
                                         tr("albumartname.jpg"), &ok);
    if (ok && !text.isEmpty()) {
        int lastItemIdx = m_albumArtNamesModel->rowCount();
        m_albumArtNamesModel->insertRow(lastItemIdx);
        m_albumArtNamesModel->setData(m_albumArtNamesModel->index(lastItemIdx), text);
        someChanged();
    }
}

void SettingConfFile::onRemoveButtonClicked() {
    int row = m_albumArtNamesView->currentIndex().row();
    if (row >= 0 && row <= m_albumArtNamesModel->rowCount()) {
        m_albumArtNamesModel->removeRow(row);
    }
}

void SettingConfFile::onConfigFileChanged() {
    loadModel();
}


