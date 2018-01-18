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


#ifndef MEDIAFOLDERS_H
#define MEDIAFOLDERS_H

#include "../core/model/mediafolder.h"
#include "../core/configurationfile.h"

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QTableView>
#include <QDialog>
//#include <KDialog>
#include <QToolButton>

namespace Ui {
class MediaFoldersWidget;
}

class MediaFoldersWidget : public QWidget {
        Q_OBJECT
    public:
        explicit MediaFoldersWidget(QWidget* parent = 0, Qt::WindowFlags f = 0);
        virtual ~MediaFoldersWidget();
        void addLine(QString line);
        void loadModel();
        bool isModelChanged();
        void setFileIsNotWritable(bool writable);
    private:
        void initGUI();
        ConfigurationFile* m_actualConfFile;
        Ui::MediaFoldersWidget *ui;
        QStandardItemModel* m_model;
        QTableView* m_tableView;
        ConfigurationFile* m_configFile;
        QToolButton* m_add;
        QToolButton* m_remove;
        bool m_changedModel;
        QList<MediaFolder *> m_createdMediaFolders;
        QWidget* m_controllWidget;
        QLabel* m_lblInfo;
        QString m_pathToConfig;

    public slots:
        void onAddButtonClicked();
        void onRemoveButtonClicked();
        void onEditButtonClicked();
        void saveModel();
        void reload();
    signals:
        void modelChanged();

};

#endif // MEDIAFOLDERS_H
