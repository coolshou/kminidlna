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


#ifndef MEDIAFOLDERS_H
#define MEDIAFOLDERS_H

#include "../core/model/mediafolder.h"

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QTableView>
#include <KDialog>


class MediaFoldersDialog : public KDialog
{
Q_OBJECT
public:
    explicit MediaFoldersDialog(QWidget* parent = 0, Qt::WindowFlags f = 0);
    virtual ~MediaFoldersDialog();
    void addLine(QString line);
    void loadModel();
private:
    void initGUI();
    QStandardItemModel* m_model;
    QTableView* m_tableView;
    QList<MediaFolder *> m_rows;
    
};

#endif // MEDIAFOLDERS_H
