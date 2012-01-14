/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) 2011  Tomáš Poledný <email>

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


#include "mediadirwidget.h"
#include <QVBoxLayout>
#include "mediadirmodel.h"
#include <QHeaderView>



MediaDirWidget::MediaDirWidget(QWidget* parent, Qt::WFlags flags): KDialog(parent, flags)
{
    initGUI();
}

MediaDirWidget::~MediaDirWidget()
{

}

void MediaDirWidget::initGUI()
{
    setWindowTitle("Media Dir");
    main = new QWidget(this);
    QVBoxLayout* vbl = new QVBoxLayout();
    main->setLayout(vbl);
    setMainWidget(main);


    //QListView
    m_qlistview = new QTableView();


    //TODO model
    MediaDirModel* model = new MediaDirModel(m_qlistview);
    m_qlistview->setModel(model);
    
//     m_qlistview->setHorizontalHeader(new QHeaderView(Qt::Horizontal));
    m_qlistview->verticalHeader()->hide();
    m_qlistview->setShowGrid(false);
    m_qlistview->setSelectionMode(QAbstractItemView::SingleSelection);
    
   

    vbl->addWidget(m_qlistview);

}

void MediaDirWidget::loadMediaDirs()
{

}



