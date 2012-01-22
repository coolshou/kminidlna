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


#include "mediafolders.h"

MediaFolders::MediaFolders(QWidget* parent, Qt::WindowFlags f): QWidget(parent, f)
{
    loadModel();
    initGUI();
}

MediaFolders::~MediaFolders()
{

}

void MediaFolders::initGUI()
{
    QVBoxLayout* mainlayout = new QVBoxLayout(this);
    setLayout(mainlayout);
    
    m_tableView = new QTableView(this);
    m_tableView->setModel(m_model);
    mainlayout->addWidget(m_tableView);
//     layoutRun->addWidget(new QPixmap (QIcon(":/images/ikona.png")));

}

void MediaFolders::loadModel()
{
    m_model = new QStandardItemModel(0, 4, this);
    m_model->appendColumn();
}

void MediaFolders::addLine(QString line)
{

}



