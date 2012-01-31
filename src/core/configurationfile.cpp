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
// #include "minidlna_process.h"

ConfigurationFile::ConfigurationFile(QString path, QObject* parent) : QObject(parent), m_path(path) {
    if (loadData()) {
        qDebug() << "ConfigurationFile: Conf file is not loaded";
    }


    //DBG
    m_mediaFolders << new MediaFolder("/home/", MediaFolder::VIDEO, this);
    m_mediaFolders << new MediaFolder("/media/sda1", MediaFolder::AUDIO, this);

    //EndDBG

}

ConfigurationFile::~ConfigurationFile() {
}

/**
 * Function copy predefined configuration file
 */
bool ConfigurationFile::createFile(QString path) {
    this->m_path = path;
    QFile conf(path);
    if (conf.exists()) {
        qDebug() << "Configuration: creatFile> File exists";
        return false;
    }

    //TODO copy predefined file
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
            parseLine(line);
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
    return loadData();
}

/**
 * !!!!!!!!!!!!Copy from minidlna source options.c!!!!!!!!!!!!
 */
static const struct {
    enum upnpconfigoptions id;
    const char * name;
} optionids[] = {
    { UPNPIFNAME, "network_interface" },
    { UPNPLISTENING_IP, "listening_ip" },
    { UPNPPORT, "port" },
    { UPNPPRESENTATIONURL, "presentation_url" },
    { UPNPNOTIFY_INTERVAL, "notify_interval" },
    { UPNPSYSTEM_UPTIME, "system_uptime" },
    { UPNPUUID, "uuid"},
    { UPNPSERIAL, "serial"},
    { UPNPMODEL_NAME, "model_name"},
    { UPNPMODEL_NUMBER, "model_number"},
    { UPNPFRIENDLYNAME, "friendly_name"},
    { UPNPMEDIADIR, "media_dir"},
    { UPNPALBUMART_NAMES, "album_art_names"},
    { UPNPINOTIFY, "inotify" },
    { UPNPDBDIR, "db_dir" },
    { UPNPLOGDIR, "log_dir" },
    { ENABLE_TIVO, "enable_tivo" },
    { ENABLE_DLNA_STRICT, "strict_dlna" }
};

void ConfigurationFile::parseLine(QString line) {

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



}

QList<MediaFolder *>& ConfigurationFile::mediaFolders(){
    return m_mediaFolders;
}

bool ConfigurationFile::saveMediaDirectoryModel(QStandardItemModel* model)
{
    qDebug() << "model saved";
    //TODO save model
    return false;
}




