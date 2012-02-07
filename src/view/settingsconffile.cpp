/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) 2012  TomÃ¡Å¡ PolednÃ½ <email>

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


#include "settingsconffile.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <KLocalizedString>
#include "../core/minidlna_process.h"
#include "../core/configurationfile.h"
#include <QSpacerItem>
#include <KDialog>
#include <QLineEdit>
#include <QLabel>

SettingsConfFile::SettingsConfFile(QWidget* parent, Qt::WindowFlags f): AbstractSettings(parent, f), m_albumArtNamesModel(0), m_actualConfFile(0) {
    initGUI();
    loadSettings();
    connect(MiniDLNAProcess::getInstance(), SIGNAL(configurationFileChanged()),
            this, SLOT(onConfigFileChanged()));
}

SettingsConfFile::~SettingsConfFile() {

}


void SettingsConfFile::applySettings() {

    QStringList list = m_albumArtNamesModel->stringList();
    if (m_actualConfFile != 0) {
        m_actualConfFile->saveAlbumArtNames(list);
    } else {
        qDebug() << "SettingsConfFile: Not exist actual conf file object!";
    }
    AbstractSettings::applySettings();

}

void SettingsConfFile::loadSettings() {
    loadModel();
}

void SettingsConfFile::setDefaults() {

}

void SettingsConfFile::initGUI() {
    QVBoxLayout* central = new QVBoxLayout(this);

    QGroupBox* albumArtNamesGroup = new QGroupBox(i18n("Album art filenames"), this);
    central->addWidget(albumArtNamesGroup);

    QVBoxLayout* albumArtNamesGroupLayout = new QVBoxLayout(albumArtNamesGroup);

    m_albumArtNamesView = new QListView(albumArtNamesGroup);
    albumArtNamesGroupLayout->addWidget(m_albumArtNamesView);
    m_albumArtNamesView->setEditTriggers(QAbstractItemView::DoubleClicked);

    m_albumArtNamesControllWidget = new QWidget(albumArtNamesGroup);

    QHBoxLayout* controllWidgetLayout = new QHBoxLayout(m_albumArtNamesControllWidget);
    controllWidgetLayout->addSpacerItem(new QSpacerItem(80, 10, QSizePolicy::Expanding));

    m_add = new QToolButton(m_albumArtNamesControllWidget);
    m_add->setIcon(KIcon("list-add"));
    controllWidgetLayout->addWidget(m_add);
    connect(m_add, SIGNAL(clicked(bool)),
            this, SLOT(onAddButtonClicked()));



    m_remove = new QToolButton(m_albumArtNamesControllWidget);
    m_remove->setIcon(KIcon("list-remove"));
    controllWidgetLayout->addWidget(m_remove);
    connect(m_remove, SIGNAL(clicked(bool)),
            this, SLOT(onRemoveButtonClicked()));
    connect(m_remove, SIGNAL(clicked(bool)),
            this, SLOT(someChanged()));
    albumArtNamesGroupLayout->addWidget(m_albumArtNamesControllWidget);    
}

void SettingsConfFile::loadModel() {
    m_actualConfFile = MiniDLNAProcess::getInstance()->configFile();
    m_albumArtNamesList = m_actualConfFile->albumArtNames();
    if (m_albumArtNamesModel != 0) {
        disconnect(m_albumArtNamesModel);
        delete m_albumArtNamesModel;
    }
    m_albumArtNamesModel = new QStringListModel(m_albumArtNamesList);


    connect(m_albumArtNamesModel, SIGNAL(dataChanged(QModelIndex, QModelIndex)),
            this, SLOT(someChanged()));
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

void SettingsConfFile::onAddButtonClicked() {
    KDialog* dlg = new KDialog(this);
    dlg->setCaption(i18n("Add new album art name"));
    dlg->setButtons(KDialog::Ok | KDialog::Cancel);
    QHBoxLayout* mainlayout = new QHBoxLayout(dlg->mainWidget());

    QLabel* lbl = new QLabel(i18n("<b>New album art name:</b>"), dlg->mainWidget());
    mainlayout->addWidget(lbl);

    QLineEdit* albumArtName = new QLineEdit(i18n("Type new name"), dlg->mainWidget());
    mainlayout->addWidget(albumArtName);

    if (dlg->exec() == QDialog::Accepted) {
        int lastItemIdx = m_albumArtNamesModel->rowCount();
        m_albumArtNamesModel->insertRow(lastItemIdx);
        m_albumArtNamesModel->setData(m_albumArtNamesModel->index(lastItemIdx), albumArtName->text());
        someChanged();
    }
    delete dlg;
}

void SettingsConfFile::onRemoveButtonClicked() {
    int row = m_albumArtNamesView->currentIndex().row();
    if (row >= 0 && row <= m_albumArtNamesModel->rowCount()) {
        m_albumArtNamesModel->removeRow(row);
    }
}

void SettingsConfFile::onConfigFileChanged() {
    loadModel();
}


