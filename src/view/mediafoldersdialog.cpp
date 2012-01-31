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

MediaFoldersDialog::MediaFoldersDialog(QWidget* parent, Qt::WindowFlags f): KDialog(parent, f) {
    m_model = new QStandardItemModel(0, 2, this);
    loadModel();
    initGUI();
}

MediaFoldersDialog::~MediaFoldersDialog() {
    delete m_model;
}

void MediaFoldersDialog::initGUI() {
    setCaption(i18n("Media Folders"));
    QVBoxLayout* mainlayout = new QVBoxLayout(mainWidget());

    m_tableView = new QTableView(mainWidget());
    m_tableView->setModel(m_model);
    m_tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    m_tableView->resizeRowsToContents();
    m_tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_tableView->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    m_tableView->verticalHeader()->setVisible(false);

    mainlayout->addWidget(m_tableView);
    //Controll part
    QWidget* controllWidget = new QWidget(mainWidget());
    QHBoxLayout* controllLayout = new QHBoxLayout(controllWidget);

    QPushButton* btnEdit = new QPushButton(i18n("Edit"), controllWidget);
    connect(btnEdit, SIGNAL(clicked(bool)),
            this, SLOT(onEditButtonClicked()));

    controllLayout->addSpacerItem(new QSpacerItem(80, 10, QSizePolicy::Expanding));

    m_add = new QToolButton(controllWidget);
    m_add->setIcon(KIcon("list-add"));
    controllLayout->addWidget(m_add);
    connect(m_add, SIGNAL(clicked(bool)),
            this, SLOT(onAddButtonClicked()));

    m_remove = new QToolButton(controllWidget);
    m_remove->setIcon(KIcon("list-remove"));
    controllLayout->addWidget(m_remove);
    connect(m_remove, SIGNAL(clicked(bool)),
            this, SLOT(onRemoveButtonClicked()));


    mainlayout->addWidget(controllWidget);
    
    connect(this, SIGNAL(okClicked()),
	    this, SLOT(onOKClicked()));
}

void MediaFoldersDialog::loadModel() {

    m_model->clear();
    QStringList headerLabels;
    headerLabels << i18n("Folder") << i18n("Media type");
    m_model->setHorizontalHeaderLabels(headerLabels);

    QList<MediaFolder *>& mediaFolders = MiniDLNAProcess::getInstance()->configFile()->mediaFolders();
    QList<MediaFolder *>::const_iterator i;

    for (i = mediaFolders.begin(); i != mediaFolders.end(); ++i) {
        QList<QStandardItem *>* tmp = (*i)->row();
        m_model->appendRow(*tmp);
        delete tmp;
    }
    m_changedModel = false;
}

void MediaFoldersDialog::onAddButtonClicked() {
    FolderEditDialog* dlg = new FolderEditDialog(this);
    if (dlg->exec() == QDialog::Accepted) {
        m_model->appendRow(*(dlg->mediaFolder().row()));
    }
    delete dlg;
}

void MediaFoldersDialog::onRemoveButtonClicked() {
    int row = m_tableView->currentIndex().row();
    if (row >= 0 && row < m_model->rowCount()) {
	m_model->removeRow(row);
	m_changedModel = true;
    }
}

void MediaFoldersDialog::onEditButtonClicked() {
    int row = m_tableView->currentIndex().row();
    if (row >= 0 && row < m_model->rowCount()) {
        MediaFolder::MediaType type = *(MediaFolder::MediaType *) m_model->item(row, 1)->data().data();
        MediaFolder folder(m_model->item(row, 0)->data().toString(), type);
        FolderEditDialog* dlg = new FolderEditDialog(folder, this);
        if (dlg->exec() == QDialog::Accepted) {
            MediaFolder& tmp = dlg->mediaFolder();
            if (!(tmp == folder)) {
                m_model->item(row, 0)->setText(tmp.folder());
                m_model->item(row, 0)->setData(tmp.folder());

                m_model->item(row, 1)->setText(tmp.mediaTypeToString());
                m_model->item(row, 1)->setData(tmp.mediaType());
                
		m_changedModel = true;
            }
        }
        delete dlg;
    }
}


bool MediaFoldersDialog::isModelChanged()
{
    return m_changedModel;
}

void MediaFoldersDialog::onOKClicked()
{
    MiniDLNAProcess::getInstance()->configFile()->saveMediaDirectoryModel(m_model);
}




