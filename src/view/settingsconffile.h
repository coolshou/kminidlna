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


#ifndef SETTINGSCONFFILE_H
#define SETTINGSCONFFILE_H

#include "abstractsettings.h"

#include <QListView>
#include <QStringListModel>
#include <QToolButton>
#include "../core/configurationfile.h"
#include "mediafolderswidget.h"


class SettingsConfFile : public AbstractSettings {
        Q_OBJECT

    public:
        SettingsConfFile(QWidget* parent = 0, Qt::WindowFlags f = 0);
        virtual void loadSettings();
        virtual void setDefaults();
        virtual ~SettingsConfFile();
        virtual void applySettings();
    private:
        void initGUI();
        void loadModel();
        QListView* m_albumArtNamesView;
        QStringListModel* m_albumArtNamesModel;
        QWidget* m_albumArtNamesControllWidget;
        QToolButton* m_add;
        QToolButton* m_remove;
        QStringList m_albumArtNamesList;
	ConfigurationFile* m_actualConfFile;
	MediaFoldersWidget* m_mediaFoldersWidget;
    public slots:
        void onAddButtonClicked();
        void onRemoveButtonClicked();
        void onConfigFileChanged();
};

#endif // SETTINGSCONFFILE_H
