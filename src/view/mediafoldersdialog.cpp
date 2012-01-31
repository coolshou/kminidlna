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

MediaFoldersDialog::MediaFoldersDialog(QWidget* parent, Qt::WindowFlags f): KDialog(parent, f)
{
    loadModel();
    initGUI();
}

MediaFoldersDialog::~MediaFoldersDialog()
{

}

void MediaFoldersDialog::initGUI()
{
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
}

void MediaFoldersDialog::loadModel()
{

    m_model = new QStandardItemModel(0, 2, this);
    QStringList headerLabels;
    headerLabels << "Folder" << "Media type";
    m_model->setHorizontalHeaderLabels(headerLabels);

    //DBG
//     MediaFolder* m = new MediaFolder("/home/", MediaFolder::VIDEO, this);
    m_rows << new MediaFolder("/home/", MediaFolder::VIDEO, this);
    m_rows << new MediaFolder("/media/sda1", MediaFolder::AUDIO, this);

    //EndDBG

    QList<MediaFolder *>::const_iterator i;
    for (i = m_rows.begin(); i != m_rows.end(); ++i) {
        m_model->appendRow((*i)->row());
    }


}

void MediaFoldersDialog::addLine(QString line)
{

}




