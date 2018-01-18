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
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QToolButton>
#include <QSpacerItem>
#include <QDebug>
#include <QFileDialog>
//#include <KFileDialog>
#include <QPointer>
#include <QStandardPaths>
//#include <KStandardDirs>

FolderEditDialog::FolderEditDialog(QWidget* parent, Qt::WindowFlags flags): QDialog(parent, flags)
{
    initGUI();
    setWindowTitle(tr("Add media directory"));
    //setCaption(tr("Add media directory"));
    QDir dir("/");
    dir.setPath("~/");
    qDebug() << dir.absolutePath();
    m_ledFolderPath->setText("~/");
}


FolderEditDialog::FolderEditDialog(const MediaFolder& mediaFolder, QWidget* parent, Qt::WindowFlags flags): QDialog(parent, flags) {
    initGUI();
    setWindowTitle(tr("Edit media directory"));
    //setCaption(tr("Edit media directory"));
    m_ledFolderPath->setText(mediaFolder.folder());
    setMediaType(mediaFolder.mediaType());
}

FolderEditDialog::~FolderEditDialog() {
}


void FolderEditDialog::initGUI() {
    QVBoxLayout* verticalLayout = new QVBoxLayout();

    QHBoxLayout* horizontalLayout = new QHBoxLayout();
    QLabel* label = new QLabel(tr("Folder Path:"));


    horizontalLayout->addWidget(label);

    m_ledFolderPath = new QLineEdit();
    horizontalLayout->addWidget(m_ledFolderPath);

    QToolButton* tlbFindFolder = new QToolButton();
    tlbFindFolder->setText("...");
    connect(tlbFindFolder, SIGNAL(clicked(bool)),
            SLOT(onFindFolderClicked()));
    horizontalLayout->addWidget(tlbFindFolder);

    verticalLayout->addLayout(horizontalLayout);

    QHBoxLayout* horizontalLayout_2 = new QHBoxLayout();

    QLabel* label_2 = new QLabel(tr("Media Type"));

    horizontalLayout_2->addWidget(label_2);

    m_cbxMediaType = new QComboBox();

    m_cbxMediaType->clear();
    m_cbxMediaType->insertItems(0, QStringList()
                                << tr("None")
                                << tr("Video")
                                << tr("Audio")
                                << tr("Images")
                               );
    horizontalLayout_2->addWidget(m_cbxMediaType);

    QSpacerItem* horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    horizontalLayout_2->addItem(horizontalSpacer);


    verticalLayout->addLayout(horizontalLayout_2);

    QSpacerItem* verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    verticalLayout->addItem(verticalSpacer);
}

void FolderEditDialog::onFindFolderClicked() {
    //TODO getExistingDirectory
    /*
    QString path = QFileDialog::getExistingDirectory(this->parentWidget(), tr("Media Folder"), QUrl(m_ledFolderPath->text()),QFileDialog::ShowDirsOnly);
    if (!path.isEmpty()) {
        m_ledFolderPath->setText(path);
    }
    */
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




