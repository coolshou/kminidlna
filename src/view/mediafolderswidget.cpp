/*
http://github.com/coolshou/qminidlna

Copyright (C) 2018 jimmy

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


#include "mediafolderswidget.h"
#include "ui_mediafolderswidget.h"

#include <QTableWidgetItem>
#include <QSizePolicy>
#include <QAbstractItemView>
#include <QHeaderView>
//#include <KLocalizedString>
#include "../core/minidlna_process.h"
#include "foldereditdialog.h"
#include <QPushButton>


MediaFoldersWidget::MediaFoldersWidget(QWidget* parent, Qt::WindowFlags f):
    QWidget(parent, f),
    m_actualConfFile(0),
    ui(new Ui::MediaFoldersWidget)
{
    ui->setupUi(this);
    m_model = new QStandardItemModel(0, 2, this);
    initGUI();
    loadModel();
}

MediaFoldersWidget::~MediaFoldersWidget() {
    delete m_model;
}

void MediaFoldersWidget::initGUI() {
    setWindowTitle(tr("Media Folders"));
    m_lblInfo = ui->m_lblInfo;

    m_tableView = ui->m_tableView;
    m_tableView->resizeRowsToContents();
    //m_tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //m_tableView->horizontalHeader()->setResizeMode(QHeaderView::Stretch);

    //Controll part
    m_controllWidget = ui->m_controllWidget;

    QPushButton* btnEdit = ui->btnEdit;
    connect(btnEdit, SIGNAL(clicked(bool)), this, SLOT(onEditButtonClicked()));

    //m_add = ui->m_add;
    connect(ui->m_add, SIGNAL(clicked(bool)), this, SLOT(onAddButtonClicked()));

    //m_remove = ui->m_remove;
    connect(ui->m_remove, SIGNAL(clicked(bool)), this, SLOT(onRemoveButtonClicked()));
}

void MediaFoldersWidget::loadModel() {
    m_model->clear();
    QStringList headerLabels;
    headerLabels << tr("Folder") << tr("Media type");
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

void MediaFoldersWidget::onAddButtonClicked() {
    FolderEditDialog* dlg = new FolderEditDialog(this);
    if (dlg->exec() == QDialog::Accepted) {
        m_model->appendRow(*(dlg->mediaFolder()->row()));
        emit modelChanged();
    }
    delete dlg;
}

void MediaFoldersWidget::onRemoveButtonClicked() {
    int row = m_tableView->currentIndex().row();
    if (row >= 0 && row < m_model->rowCount()) {
        m_model->removeRow(row);
        m_changedModel = true;
        emit modelChanged();
    }
}

void MediaFoldersWidget::onEditButtonClicked() {
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


bool MediaFoldersWidget::isModelChanged() {
    return m_changedModel;
}

void MediaFoldersWidget::saveModel() {
    if (m_actualConfFile != 0) {
        if (m_actualConfFile->isWritable()) {
            m_actualConfFile->saveMediaDirectoryModel(m_model);
        }
    }
}

void MediaFoldersWidget::setFileIsNotWritable(bool writable) {

    m_controllWidget->setEnabled(writable);
    if (!writable) {
        m_lblInfo->setText("<b>" + tr("Config file: ") + "</b>" + m_pathToConfig +
                           " <br/><b>" + tr("Configuration file is not writable!") + "</b");
    } else {
        m_lblInfo->setText("<b>" + tr("Config file: ") + "</b>" + m_pathToConfig);
    }
}

void MediaFoldersWidget::reload() {
    loadModel();
}






