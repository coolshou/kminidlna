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


#ifndef CONFIGURATION_H
#define CONFIGURATION_H
#include <QLinkedList>
#include "minidlna_process.h"


/**
 * !!!!!!!!!!!!COPY FROM MINIDLNA SOURCE CODE options.h!!!!!!!!!
 */
/* enum of option available in the miniupnpd.conf */
enum upnpconfigoptions {
	UPNP_INVALID = 0,
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
 * This class is used for load media dir and save configuration minidlna to configuration file
 * 
 */
class Configuration
{

public:
    Configuration(QString path = MiniDLNA::CONFFILE_PATH);
    virtual ~Configuration();
    bool reload();
    bool creatFile(QString path);
    
    
private:
    bool loadData();
    void parseLine(QString line);
    
    QString path;
    
    
};

#endif // CONFIGURATION_H
