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


#include "mediadirmodel.h"
#include <klocalizedstring.h>
#include <QSize>
#include <QFile>

QVariant MediaDirModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() >= list.size())
        return QVariant();

    if (role == Qt::DisplayRole) {
        //colomun
        if (index.column() == 0) {

            switch ((list[index.row()])->getMediaType()) {
            case 0:
                return QVariant("Video");
            case 1:
                return QVariant("Audio");
            case 2:
                return QVariant("Picture");
            }
            return QVariant();
        }
        return (list[index.row()])->getPath();
    }
    else
        return QVariant();
}

int MediaDirModel::rowCount(const QModelIndex& parent) const
{
    return list.length();
}

MediaDirModel::MediaDirModel(QObject* parent): QAbstractTableModel(parent)
{
    loadData();
}

MediaDirModel::MediaDirModel(const MediaDirModel& other)
{

}

void MediaDirModel::loadData()
{
    list << new MediaDirItem(MediaDirItem::Video, "/ahoj") << new MediaDirItem(MediaDirItem::Audio, "/ahoj/vid") << new MediaDirItem(MediaDirItem::Picture, "/ahoj/pic");
    QFile* conf = new QFile("/etc/minidlna.conf");
    
    //TODO exceptions controll
    if (conf->exists()) {
        if (conf->open(QIODevice::ReadOnly)) {
            while (!conf->atEnd()) {
		parseLine(conf->readLine());
            }
        }
    }
    conf->close();
    /**
     * media_dir=V,/home/saljack/Stahuj/ZZZDownload
     * media_dir=A,/home/saljack/Hudba
     */
}

void MediaDirModel::parseLine(QString line)
{
  if(line.contains("media_dir=")){
    qDebug() << line;
  }
}



/**
 *
 * Insert columns
 */
// bool MediaDirModel::insertColumns(int column, int count, const QModelIndex& parent)
// {
//     return QAbstractItemModel::insertColumns(column, count, parent);
// }
//
// /**
//  * Insert rows
//  */
// bool MediaDirModel::insertRows(int row, int count, const QModelIndex& parent)
// {
//     return QAbstractItemModel::insertRows(row, count, parent);
// }



// bool MediaDirModel::setData(const QModelIndex& index, const QVariant& value, int role)
// {
//     return QAbstractItemModel::setData(index, value, role);
// }

QVariant MediaDirModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal ) {
        if (role == Qt::DisplayRole) {
            switch (section) {
            case 0:
                return QVariant("Media Type");
            case 1:
                return QVariant("Path");
            }
        } else if (role == Qt::SizeHintRole) {//Tohle je dulezite jinak se vubec header nezobrazi!!
            return QVariant(QSize(10,25));
        }
    }

    return QVariant();
}

int MediaDirModel::columnCount(const QModelIndex& parent) const
{
    return 2;
}





/**
 *
 * MediaDirItem Class
 */

/**
 *
 *
 */
MediaDirItem::MediaDirItem(MediaDirItem::MediaType mediaType, QString path): mediaType(mediaType), path(path)
{
    //Do nothing
}

MediaDirItem::~MediaDirItem()
{

}

QString MediaDirItem::getPath()
{
    return path;
}

void MediaDirItem::setPath(QString path)
{
    this->path = path;
}

void MediaDirItem::setMediaType(MediaDirItem::MediaType type)
{
    this->mediaType = type;
}

MediaDirItem::MediaType MediaDirItem::getMediaType()
{
    return mediaType;
}


