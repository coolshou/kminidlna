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


#ifndef FOLDEREDITDIALOG_H
#define FOLDEREDITDIALOG_H

#include <Qt>
#include <QDialog>
#include <QLineEdit>
#include <QComboBox>
#include "../core/model/mediafolder.h"

namespace Ui {
class FolderEditDialog;
}

class FolderEditDialog : public QDialog
{
    Q_OBJECT
public:
    explicit FolderEditDialog(QWidget* parent = 0, Qt::WindowFlags flags = 0);
    FolderEditDialog(const MediaFolder& mediaFolder, QWidget* parent = 0, Qt::WindowFlags flags = 0);
    virtual ~FolderEditDialog();
    MediaFolder* mediaFolder();
private:
    void initGUI();
    Ui::FolderEditDialog *ui;
    QLineEdit* m_ledFolderPath;
    QComboBox* m_cbxMediaType;
    void setMediaType(MediaFolder::MediaType mediaType);

private slots:
    void onFindFolderClicked();
};

#endif // FOLDEREDITDIALOG_H
