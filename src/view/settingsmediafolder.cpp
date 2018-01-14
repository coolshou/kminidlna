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


#include "settingsmediafolder.h"

#include <QGroupBox>
#include <QVBoxLayout>
#include "../core/minidlna_process.h"

SettingsMediaFolder::SettingsMediaFolder(QWidget* parent, Qt::WindowFlags f): AbstractSettings(parent, f) {
    initGUI();
}

SettingsMediaFolder::~SettingsMediaFolder() {

}

void SettingsMediaFolder::loadSettings() {

}

void SettingsMediaFolder::setDefaults() {

}

void SettingsMediaFolder::applySettings() {
    AbstractSettings::applySettings();
    m_mediaFoldersWidget->saveModel();
}

void SettingsMediaFolder::initGUI() {
    QVBoxLayout* central = new QVBoxLayout(this);

    QGroupBox* mediaFolderGroup = new QGroupBox(tr("Media folders"), this);
    central->addWidget(mediaFolderGroup);

    QVBoxLayout* mediaFolderGroupLayout = new QVBoxLayout(mediaFolderGroup);
    m_mediaFoldersWidget = new MediaFoldersWidget(mediaFolderGroup);
    mediaFolderGroupLayout->addWidget(m_mediaFoldersWidget);
    connect(m_mediaFoldersWidget, SIGNAL(modelChanged()),
            this, SLOT(someChanged()));
    connect(MiniDLNAProcess::getInstance(), SIGNAL(configurationFileChanged()),
            m_mediaFoldersWidget, SLOT(reload()));
}


