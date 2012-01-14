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


#include "minidlna_configuration.h"

Configuration::Configuration(QString path) : path(path) {
    if (loadData()) {
        qDebug() << "Configuration: this> File not loaded will be use default";
        path = MiniDLNA::CONFFILE_PATH;
        if (!loadData()) {
            qDebug() << "Configuration: this> File not loaded. Please set path to configuration file";
        }
    }
}

Configuration::~Configuration() {

}

/**
 * Function copy predefined configuration file
 */
bool Configuration::creatFile(QString path) {
    this->path = path;
    QFile conf(path);
    if (conf.exists()) {
        qDebug() << "Configuration: creatFile> File exists";
        return false;
    }

    //TODO copy predefined file
    return true;
}


/**
 * @return - linked list with MediaDirItem
 */
QLinkedList< MediaDirItem* >& Configuration::getMediaDir(){
    return list_mediaDir;
}

/**
 * load data
 */
bool Configuration::loadData() {
    QFile conf(path);
    if (!conf.exists()) {
        return false;
    }
    if (conf.open(QIODevice::ReadOnly | QIODevice::Text)) {
        //TODO parse config file as minidlna
        QTextStream in(&conf);
        while (!in.atEnd()) {
            QString line  = in.readLine();
            parseLine(line);
        }
        conf.close();

    } else {
        qDebug() << "Configuration: loadData> File " << path << " is not readable";
        return false;
    }
    return true;
}

/**
 * load data again
 */
bool Configuration::reload() {
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

void Configuration::parseLine(QString line) {

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


