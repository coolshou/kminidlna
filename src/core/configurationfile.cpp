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


#include "configurationfile.h"
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QStandardPaths>
//#include <KStandardDirs>
#include <QApplication>
//#include <KApplication>
//#include <KAboutData>
#include <QDir>
#include "minidlna_process.h"

// bool ConfigurationFile::m_optionsIDLoaded = false;

// QMap<ConfigurationFile::ConfigOptions, QString> m_optionsID();
const QString ConfigurationFile::DEFAULT_LOCAL_CONF_FILE_NAME = "minidlna.conf";
const QString ConfigurationFile::PATH_TO_DEFAULT_LOCAL_FILE = pathToDefaultLocalConfigFile();


ConfigurationFile::ConfigurationFile(QString path, QObject* parent) : QObject(parent), m_path(path), m_optionsIDLoaded(false) {
    loadOptionsID();
    if (loadData()) {
        processOptionsData();
    } else {
        qDebug() << "ConfigurationFile: Conf file is not loaded";
    }
}

ConfigurationFile::~ConfigurationFile() {
    qDeleteAll<QList<MediaFolder *>::iterator>(m_mediaFolders.begin(), m_mediaFolders.end());
    m_mediaFolders.clear();
}

/**
 * Function copy predefined configuration file
 *
 * @return true if file is created and copied default structur other is returned false
 */
bool ConfigurationFile::createFile() {

    QFile conf(m_path);
    if (conf.exists()) {
        qDebug() << "Configuration: creatFile> File exists";
        return false;
    }

    //TODO copy predefined file
    conf.open(QIODevice::WriteOnly);
    conf.write("#Created by qminidlna\n");
    conf.close();
    return true;
}

/**
 * load data
 */
bool ConfigurationFile::loadData() {
    QFile conf(m_path);
    if (!conf.exists()) {
        return false;
    }
    if (conf.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&conf);
        while (!in.atEnd()) {
            QString line  = in.readLine();
            parseLineAndAddOption(line);
        }
        conf.close();

    } else {
        qDebug() << "Configuration: loadData() File \"" << m_path << "\" is not readable";
        return false;
    }
    return true;
}

/**
 * load data again
 */
bool ConfigurationFile::reload() {

    m_optionsValue.clear();
    qDeleteAll<QList<MediaFolder *>::iterator>(m_mediaFolders.begin(), m_mediaFolders.end());
    m_mediaFolders.clear();

    if (loadData()) {
        processOptionsData();
    }
    return false;
}

void ConfigurationFile::parseLineAndAddOption(QString line) {
    //Empty line
    if (line[0] == '\0') {
        return;
    }


    //Remove leads whitespace
    int leadsSpace;
    for (leadsSpace = 0; leadsSpace < line.length(); ++leadsSpace) {
        if (!line[leadsSpace].isSpace()) {
            break;
        }
    }

    if (leadsSpace > 0) {
        line.remove(0, leadsSpace);
    }

    //Comments or empty lines
    if (line[0] == '#' || line[0] == '\0') {
        return;
    }

    ConfigOptions option = INVALID;
    if (!m_optionsIDLoaded) {
        loadOptionsID();
    }

    QMap<ConfigOptions, QString>::const_iterator it;
    for (it = m_optionsID.constBegin(); it  != m_optionsID.constEnd(); ++it) {
        if (line.startsWith(it.value())) {
            option = it.key();
            break;
        }
    }
    if (option == INVALID) {
        return;
    }

    int positionOfValue = line.indexOf('=');
    if (positionOfValue < 0) {
        return;
    }
    ++positionOfValue;

    //remove leading space before value
    for (; positionOfValue < line.length(); ++positionOfValue) {
        if (!line[positionOfValue].isSpace()) {
            break;
        }
    }

    if (positionOfValue == line.length()) {
        return;
    }

    line.remove(0, positionOfValue);
    m_optionsValue.insert(option, line);
}

QList<MediaFolder *>& ConfigurationFile::mediaFolders() {
    return m_mediaFolders;
}

bool ConfigurationFile::saveMediaDirectoryModel(QStandardItemModel* model) {
    m_optionsValue.remove(UPNPMEDIADIR);
    qDeleteAll<QList<MediaFolder *>::iterator>(m_mediaFolders.begin(), m_mediaFolders.end());
    m_mediaFolders.clear();
    if (model->rowCount() > 0) {

        for (int row = 0; row < model->rowCount(); ++row) {
            MediaFolder fld(model->item(row, 0)->data().toString(),
                            * (MediaFolder::MediaType *) model->item(row, 1)->data().data());
            m_optionsValue.insert(UPNPMEDIADIR, fld.line());
        }
    }
    return saveOptions();
}

void ConfigurationFile::loadOptionsID() {

    if (!m_optionsIDLoaded) {
        m_optionsID.insert(UPNPIFNAME, "network_interface");
        m_optionsID.insert(UPNPLISTENING_IP, "listening_ip");
        m_optionsID.insert(UPNPPORT, "port");
        m_optionsID.insert(UPNPPRESENTATIONURL, "presentation_url");
        m_optionsID.insert(UPNPNOTIFY_INTERVAL, "notify_interval");
        m_optionsID.insert(UPNPSYSTEM_UPTIME, "system_uptime");
        m_optionsID.insert(UPNPUUID, "uuid");
        m_optionsID.insert(UPNPSERIAL, "serial");
        m_optionsID.insert(UPNPMODEL_NAME, "model_name");
        m_optionsID.insert(UPNPMODEL_NUMBER, "model_number");
        m_optionsID.insert(UPNPFRIENDLYNAME, "friendly_name");
        m_optionsID.insert(UPNPMEDIADIR, "media_dir");
        m_optionsID.insert(UPNPALBUMART_NAMES, "album_art_names");
        m_optionsID.insert(UPNPINOTIFY, "inotify");
        m_optionsID.insert(UPNPDBDIR, "db_dir");
        m_optionsID.insert(UPNPLOGDIR, "log_dir");
        m_optionsID.insert(ENABLE_TIVO, "enable_tivo");
        m_optionsID.insert(ENABLE_DLNA_STRICT, "strict_dlna");
        m_optionsIDLoaded = true;
    }
}

void ConfigurationFile::processOptionsData() {
    loadAndSetDirectory();

}


/**
 * not clear m_mediaFolders!
 */
void ConfigurationFile::loadAndSetDirectory() {
    QList<QString> directory = m_optionsValue.values(UPNPMEDIADIR);
    QList<QString>::const_iterator it;
    for (it = directory.constBegin(); it != directory.constEnd(); ++it) {
        MediaFolder* mf = new MediaFolder(*it, -1, this);
        m_mediaFolders.append(mf);
    }
}

/**
 * Save all options
 */
bool ConfigurationFile::saveOptions() {
    QFileInfo info(m_path);
    QDir dir = info.dir();
    if (!dir.exists()) {
        if (!dir.mkpath(m_path)) {
            qDebug() << tr("ConfigurationFile: Cannot create directory");
            return false;
        }
    }

    QFile confFile(m_path);
    if (confFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&confFile);

        QMultiHash<ConfigOptions, QString>::const_iterator it;
        for (it = m_optionsValue.constBegin(); it != m_optionsValue.constEnd(); ++it) {
            if (it.key() != INVALID) {
                out << m_optionsID[it.key()] << "=" << it.value();
                endl(out);
            }

        }
    } else {
        qDebug() << tr("ConfigurationFile: Cannot open conf file: ") << m_path;
        return false;
    }
    if (confFile.isOpen()) {
        confFile.close();
    }
    reload();
    return true;
}


QString ConfigurationFile::pathToDefaultLocalConfigFile() {
    QString stdir= QStandardPaths::writableLocation(QStandardPaths::ConfigLocation);
    //KStandardDirs stdir;
    //QString path = stdir.saveLocation("data") +
    QString path = stdir +
                   "qminidlna/" +
                   DEFAULT_LOCAL_CONF_FILE_NAME;
    return path;
}

bool ConfigurationFile::isWritable() {
    QFileInfo info(m_path);
    return info.isWritable();
}

QStringList ConfigurationFile::albumArtNames() {
    QStringList albumArtNamesList;
    if (m_optionsValue.contains(UPNPALBUMART_NAMES)) {
        QString value = m_optionsValue.value(UPNPALBUMART_NAMES);
        albumArtNamesList = value.split("/");
    }
    return albumArtNamesList;
}

void ConfigurationFile::saveAlbumArtNames(QStringList list) {
    if (m_optionsValue.contains(UPNPALBUMART_NAMES)) {
        m_optionsValue.remove(UPNPALBUMART_NAMES);
    }
    if (!list.isEmpty()) {
        m_optionsValue.insert(UPNPALBUMART_NAMES, list.join("/"));
    }
    saveOptions();
}

