/*
KminiDLNA
http://gitorious.org/kminidlna/pages/Home

Copyright (C) 2011 Saljack <saljacky a gmail dot com>

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
version 2 as published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/


#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <kpagedialog.h>
#include "settingsgeneral.h"
#include "settingsminidlna.h"
#include "settingsserver.h"
#include "settingsconffile.h"
#include "settingsmediafolder.h"


class SettingsDialog : public KPageDialog {
        Q_OBJECT
    public:
        explicit SettingsDialog(QWidget* parent = 0, Qt::WFlags flags = 0);
        virtual ~SettingsDialog();
    private:
        SettingsGeneral* m_general;
        SettingsMiniDLNA* m_minidlna;
        SettingsConfFile* m_confFileSettings;
        SettingsMediaFolder* m_mediaFolders;
        SettingsServer* m_server;
        KPushButton*  m_apply;
        bool m_wasNo;

        void initGUI();
    private slots:
        void onApply();
        void onDefault();
    public slots:
        void curentPageChanged(KPageWidgetItem* current, KPageWidgetItem* before);
        void onChange();
    signals:
	void settingsChanged();
};

#endif // SETTINGSDIALOG_H
