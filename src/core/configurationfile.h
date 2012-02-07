/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) 2011  Tomáš Poledný <email>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/


#ifndef CONFIGURATIONFILE_H
#define CONFIGURATIONFILE_H
#include <QList>
#include <QStandardItemModel>
#include <QMap>
#include <QMultiHash>

#include "model/mediafolder.h"




/**
 * This class is used for load media dir and save configuration minidlna to configuration file
 *
 */
class ConfigurationFile: public QObject {
        Q_OBJECT
    public:
        ConfigurationFile(QString path, QObject* parent = 0);
        virtual ~ConfigurationFile();
        bool reload();
        bool createFile();
        QList< MediaFolder* >& mediaFolders();
	QStringList albumArtNames();
	void saveAlbumArtNames(QStringList list);
        bool saveMediaDirectoryModel(QStandardItemModel* model);
        bool saveOptions();
        inline QString path() {
            return m_path;
        };
	bool isWritable();

        //!!!!!!!!!!!!COPY FROM MINIDLNA SOURCE CODE options.h!!!!!!!!!
        enum ConfigOptions {
            INVALID = 0,
            UPNPIFNAME = 1,			/* ext_ifname */
            UPNPLISTENING_IP,		/* listening_ip */
            UPNPPORT,			/* port */
            UPNPPRESENTATIONURL,		/* presentation_url */
            UPNPNOTIFY_INTERVAL,		/* notify_interval */
            UPNPSYSTEM_UPTIME,		/* system_uptime */
            UPNPUUID,			/* uuid */
            UPNPSERIAL,			/* serial */
            UPNPMODEL_NAME,			/* model_name */
            UPNPMODEL_NUMBER,		/* model_number */
            UPNPFRIENDLYNAME,		/* how the system should show up to DLNA clients */
            UPNPMEDIADIR,			/* directory to search for UPnP-A/V content */
            UPNPALBUMART_NAMES,		/* list of '/'-delimited file names to check for album art */
            UPNPINOTIFY,			/* enable inotify on the media directories */
            UPNPDBDIR,			/* base directory to store the database and album art cache */
            UPNPLOGDIR,			/* base directory to store the log file */
            ENABLE_TIVO,			/* enable support for streaming images and music to TiVo */
            ENABLE_DLNA_STRICT		/* strictly adhere to DLNA specs */
        };
        /**
         * Which configuration file will be used
         */
        enum ConfFile {
            DEFAULT = -1,
            GLOBAL = 0,
            USER = 1
        };
        static const QString DEFAULT_LOCAL_CONF_FILE_NAME;
        static const QString PATH_TO_DEFAULT_LOCAL_FILE;

    private:
        bool loadData();
        void loadAndSetDirectory();
        void parseLineAndAddOption(QString line);
        QString m_path;
        QList<MediaFolder *> m_mediaFolders;
        QMap<ConfigOptions, QString> m_optionsID;
        bool m_optionsIDLoaded;
        void loadOptionsID();
        void processOptionsData();
        QMultiHash<ConfigOptions, QString> m_optionsValue;
        static QString pathToDefaultLocalConfigFile();
};

#endif // CONFIGURATION_H
