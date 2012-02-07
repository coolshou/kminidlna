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

SettingsConfFile::SettingsConfFile(QWidget* parent, Qt::WindowFlags f): AbstractSettings(parent, f) {
    initGUI();
    loadModel();
}

SettingsConfFile::~SettingsConfFile() {

}


void SettingsConfFile::applySettings() {
    AbstractSettings::applySettings();

}

void SettingsConfFile::loadSettings() {

}

void SettingsConfFile::setDefaults() {

}

void SettingsConfFile::initGUI() {
    QVBoxLayout* central = new QVBoxLayout(this);

    QGroupBox* confFilegroup = new QGroupBox(i18n("Configuration file"), this);
    central->addWidget(confFilegroup);

    QVBoxLayout* confFileGroupLayout = new QVBoxLayout(confFilegroup);

    m_albumArtNamesView = new QListView(confFilegroup);
    confFileGroupLayout->addWidget(m_albumArtNamesView);

    m_albumArtNamesControllWidget = new QWidget(confFilegroup);

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

    confFileGroupLayout->addWidget(m_albumArtNamesControllWidget);
}

void SettingsConfFile::loadModel() {
    ConfigurationFile* confFile = MiniDLNAProcess::getInstance()->configFile();
    m_albumArtNamesList = confFile->albumArtNames();
    m_albumArtNamesModel = new QStringListModel(m_albumArtNamesList);
    connect(m_albumArtNamesModel, SIGNAL(dataChanged(QModelIndex, QModelIndex)),
            this, SLOT(someChanged()));
    m_albumArtNamesView->setModel(m_albumArtNamesModel);

}

void SettingsConfFile::onAddButtonClicked() {
//     KDialog* dlg = new KDialog(this);
//     dlg->setButtons(KDialog::Ok | KDialog::Cancel);
//     QLineEdit* albumArtName = new QLineEdit(i18n("Album art name"), dlg);
//     if (dlg->exec() == QDialog::Accepted) {
    m_albumArtNamesModel->insertRow(m_albumArtNamesModel->rowCount());
// 	QModelIndex last = m_albumArtNamesModel->index(m_albumArtNamesModel->rowCount());
// // 	m_albumArtNamesView->;
//     }
//     delete dlg;
}

void SettingsConfFile::onRemoveButtonClicked() {
    int row = m_albumArtNamesView->currentIndex().row();
    if (row >= 0 && row <= m_albumArtNamesModel->rowCount()) {
        m_albumArtNamesModel->removeRow(row);
    }
}

