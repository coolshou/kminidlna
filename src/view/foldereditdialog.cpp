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


#include "foldereditdialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QToolButton>
#include <QSpacerItem>
#include <QDebug>
#include <KFileDialog>

FolderEditDialog::FolderEditDialog(QWidget* parent, Qt::WFlags flags): KDialog(parent, flags)
{
    initGUI();
    setCaption(i18n("Add media directory"));
    m_ledFolderPath->setText("~/");
}


FolderEditDialog::FolderEditDialog(const MediaFolder& mediaFolder, QWidget* parent, Qt::WFlags flags): KDialog(parent, flags) {
    initGUI();
    setCaption(i18n("Edit media directory"));
    m_ledFolderPath->setText(mediaFolder.folder());
    setMediaType(mediaFolder.mediaType());
}

FolderEditDialog::~FolderEditDialog() {
}


void FolderEditDialog::initGUI() {
    QVBoxLayout* verticalLayout = new QVBoxLayout(mainWidget());

    QHBoxLayout* horizontalLayout = new QHBoxLayout();
    QLabel* label = new QLabel(i18n("Folder Path:"), mainWidget());


    horizontalLayout->addWidget(label);

    m_ledFolderPath = new QLineEdit(mainWidget());
    horizontalLayout->addWidget(m_ledFolderPath);

    QToolButton* tlbFindFolder = new QToolButton(mainWidget());
    connect(tlbFindFolder, SIGNAL(clicked(bool)),
            SLOT(onFindFolderClicked()));
    horizontalLayout->addWidget(tlbFindFolder);

    verticalLayout->addLayout(horizontalLayout);

    QHBoxLayout* horizontalLayout_2 = new QHBoxLayout();

    QLabel* label_2 = new QLabel(i18n("Media Type"), mainWidget());

    horizontalLayout_2->addWidget(label_2);

    m_cbxMediaType = new QComboBox(mainWidget());

    m_cbxMediaType->clear();
    m_cbxMediaType->insertItems(0, QStringList()
                                << i18n("None")
                                << i18n("Video")
                                << i18n("Audio")
                                << i18n("Images")
                               );
    horizontalLayout_2->addWidget(m_cbxMediaType);

    QSpacerItem* horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    horizontalLayout_2->addItem(horizontalSpacer);


    verticalLayout->addLayout(horizontalLayout_2);

    QSpacerItem* verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    verticalLayout->addItem(verticalSpacer);
}

void FolderEditDialog::onFindFolderClicked() {
    QString path = KFileDialog::getExistingDirectory(KUrl(m_ledFolderPath->text()), this, i18n("Media Folder"));
    if (!path.isEmpty()) {
        m_ledFolderPath->setText(path);
    }
}

void FolderEditDialog::setMediaType(MediaFolder::MediaType mediaType) {
    m_cbxMediaType->setCurrentIndex(mediaType);
}

MediaFolder& FolderEditDialog::mediaFolder() {
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
    MediaFolder* folder = new MediaFolder(m_ledFolderPath->text(), mediaType, this);
    return *folder;
}




