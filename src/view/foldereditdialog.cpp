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


#include "foldereditdialog.h"
#include "ui_foldereditdialog.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDebug>
#include <QFileDialog>
#include <QPointer>
#include <QStandardPaths>


FolderEditDialog::FolderEditDialog(QWidget* parent, Qt::WindowFlags flags):
    QDialog(parent, flags),
    ui(new Ui::FolderEditDialog)
{
    ui->setupUi(this);
    initGUI();
    setWindowTitle(tr("Add media directory"));
    QDir dir("/");
    dir.setPath("~/");
    qDebug() << dir.absolutePath();
    m_ledFolderPath->setText("~/");
}


FolderEditDialog::FolderEditDialog(const MediaFolder& mediaFolder, QWidget* parent, Qt::WindowFlags flags):
    QDialog(parent, flags),
    ui(new Ui::FolderEditDialog)
{
    ui->setupUi(this);
    initGUI();
    setWindowTitle(tr("Edit media directory"));
    m_ledFolderPath->setText(mediaFolder.folder());
    setMediaType(mediaFolder.mediaType());
}

FolderEditDialog::~FolderEditDialog() {
}


void FolderEditDialog::initGUI() {
    m_ledFolderPath = ui->m_ledFolderPath;
    connect(ui->tlbFindFolder, SIGNAL(clicked(bool)), SLOT(onFindFolderClicked()));
    m_cbxMediaType = ui->m_cbxMediaType;
}

void FolderEditDialog::onFindFolderClicked() {
    QString path = QFileDialog::getExistingDirectory(this->parentWidget(), tr("Media Folder"), m_ledFolderPath->text(),QFileDialog::ShowDirsOnly);
    if (!path.isEmpty()) {
        m_ledFolderPath->setText(path);
    }

}

void FolderEditDialog::setMediaType(MediaFolder::MediaType mediaType) {
    m_cbxMediaType->setCurrentIndex(mediaType);
}

MediaFolder* FolderEditDialog::mediaFolder() {
    MediaFolder::MediaType mediaType;
    switch (m_cbxMediaType->currentIndex()) {
    case 1:
        mediaType = MediaFolder::VIDEO;
        break;
    case 2:
        mediaType = MediaFolder::AUDIO;
        break;
    case 3:
        mediaType = MediaFolder::IMAGES;
        break;
    default:
        mediaType = MediaFolder::NONE;
    }
    QPointer<MediaFolder> folder = new MediaFolder(m_ledFolderPath->text(), mediaType);
    return folder;
}




