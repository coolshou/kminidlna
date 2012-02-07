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


#include "mediafoldersdialog.h"
#include <QTableWidgetItem>
#include <QSizePolicy>
#include <QAbstractItemView>
#include <QHeaderView>
#include <KLocalizedString>
#include "../core/minidlna_process.h"
#include "foldereditdialog.h"
#include <QPushButton>


MediaFoldersDialog::MediaFoldersDialog(QWidget* parent, Qt::WindowFlags f): QWidget(parent, f), m_actualConfFile(0) {
    m_model = new QStandardItemModel(0, 2, this);
    initGUI();
    loadModel();
}

MediaFoldersDialog::~MediaFoldersDialog() {
    delete m_model;
}

void MediaFoldersDialog::initGUI() {
//     setCaption(i18n("Media Folders"));
    QVBoxLayout* mainlayout = new QVBoxLayout(this);

    m_lblInfo = new QLabel(this);
    mainlayout->addWidget(m_lblInfo);

    m_tableView = new QTableView(this);
    m_tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    m_tableView->resizeRowsToContents();
    m_tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_tableView->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    m_tableView->verticalHeader()->setVisible(false);

    mainlayout->addWidget(m_tableView);
    //Controll part
    m_controllWidget = new QWidget(this);
    QHBoxLayout* controllLayout = new QHBoxLayout(m_controllWidget);

    QPushButton* btnEdit = new QPushButton(i18n("Edit"), m_controllWidget);
    connect(btnEdit, SIGNAL(clicked(bool)),
            this, SLOT(onEditButtonClicked()));

    controllLayout->addSpacerItem(new QSpacerItem(80, 10, QSizePolicy::Expanding));

    m_add = new QToolButton(m_controllWidget);
    m_add->setIcon(KIcon("list-add"));
    controllLayout->addWidget(m_add);
    connect(m_add, SIGNAL(clicked(bool)),
            this, SLOT(onAddButtonClicked()));

    m_remove = new QToolButton(m_controllWidget);
    m_remove->setIcon(KIcon("list-remove"));
    controllLayout->addWidget(m_remove);
    connect(m_remove, SIGNAL(clicked(bool)),
            this, SLOT(onRemoveButtonClicked()));

    mainlayout->addWidget(m_controllWidget);
}

void MediaFoldersDialog::loadModel() {
    m_model->clear();
    QStringList headerLabels;
    headerLabels << i18n("Folder") << i18n("Media type");
    m_model->setHorizontalHeaderLabels(headerLabels);
    m_tableView->setModel(m_model);

    m_actualConfFile = MiniDLNAProcess::getInstance()->configFile();
    QList<MediaFolder *>& mediaFolders = m_actualConfFile->mediaFolders();
    m_pathToConfig = m_actualConfFile->path();
    QList<MediaFolder *>::const_iterator i;

    for (i = mediaFolders.begin(); i != mediaFolders.end(); ++i) {
        QList<QStandardItem *>* tmp = (*i)->row();
        m_model->appendRow(*tmp);
        delete tmp;
    }

    setFileIsNotWritable(m_actualConfFile->isWritable());
    m_changedModel = false;
}

void MediaFoldersDialog::onAddButtonClicked() {
    FolderEditDialog* dlg = new FolderEditDialog(this);
    if (dlg->exec() == QDialog::Accepted) {
        m_model->appendRow(*(dlg->mediaFolder()->row()));
	emit modelChanged();
    }
    delete dlg;
}

void MediaFoldersDialog::onRemoveButtonClicked() {
    int row = m_tableView->currentIndex().row();
    if (row >= 0 && row < m_model->rowCount()) {
        m_model->removeRow(row);
        m_changedModel = true;
        emit modelChanged();
    }
}

void MediaFoldersDialog::onEditButtonClicked() {
    int row = m_tableView->currentIndex().row();
    if (row >= 0 && row < m_model->rowCount()) {
        MediaFolder::MediaType type = *(MediaFolder::MediaType *) m_model->item(row, 1)->data().data();
        MediaFolder folder(m_model->item(row, 0)->data().toString(), type);
        FolderEditDialog* dlg = new FolderEditDialog(folder, this);
        if (dlg->exec() == QDialog::Accepted) {
            MediaFolder* tmp = dlg->mediaFolder();
            if (!(*tmp == folder)) {
                m_model->item(row, 0)->setText(tmp->folder());
                m_model->item(row, 0)->setData(tmp->folder());

                m_model->item(row, 1)->setText(tmp->mediaTypeToString());
                m_model->item(row, 1)->setData(tmp->mediaType());

                m_changedModel = true;
                emit modelChanged();
            }
        }
        delete dlg;
    }
}


bool MediaFoldersDialog::isModelChanged() {
    return m_changedModel;
}

void MediaFoldersDialog::onApplyClicked() {
    if (m_actualConfFile != 0) {
        if (m_actualConfFile->isWritable()) {
            m_actualConfFile->saveMediaDirectoryModel(m_model);
        }
    }
}

void MediaFoldersDialog::setFileIsNotWritable(bool writable) {

    m_controllWidget->setEnabled(writable);
    if (!writable) {
        m_lblInfo->setText("<b>" + i18n("Config file: ") + "</b>" + m_pathToConfig +
                           " <br/><b>" + i18n("Configuration file is not writable!") + "</b");
    } else {
        m_lblInfo->setText("<b>" + i18n("Config file: ") + "</b>" + m_pathToConfig);
    }
}

void MediaFoldersDialog::reload() {
    loadModel();
}






