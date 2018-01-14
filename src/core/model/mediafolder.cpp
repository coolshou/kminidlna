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

#include "mediafolder.h"
#include <QDebug>

MediaFolder::MediaFolder(QString folder, MediaFolder::MediaType mediaType, QObject* parent)
        :  QObject(parent), m_mediaType(mediaType), m_lineNumber(-1), m_valid(true)
{
    m_folder = folder;
}

MediaFolder::MediaFolder(QObject* parent)
        : QObject(parent), m_valid(false)
{
    m_folder = "";
    m_mediaType = NONE;
}

MediaFolder::MediaFolder(QString line, int lineNumber, QObject* parent)
        : QObject(parent), m_lineNumber(lineNumber), m_valid(false)
{
    switch (line[0].toLatin1()) {
    case 'V':
    case 'v':
        m_mediaType = VIDEO;
        break;
    case 'A':
    case 'a':
        m_mediaType = AUDIO;
        break;
    case 'P':
    case 'p':
        m_mediaType = IMAGES;
        break;
    case '/':
        m_mediaType = NONE;
        break;
        //case '~': //replace ~ by home
    default:
        m_valid = false;
    }

    if (!m_valid) {
        QString folder(line);
        //TODO check if it is valid path
        if (m_mediaType != NONE) {
            folder.remove(0,2);
        }
        m_folder = folder;
    }
}

MediaFolder::MediaFolder(const MediaFolder& other) {
    m_folder = other.m_folder;
    m_lineNumber = other.m_lineNumber;
    m_mediaType = other.m_mediaType;
}

MediaFolder::~MediaFolder() {

}


bool MediaFolder::operator==(const MediaFolder& other) const {
    if (other.m_folder == m_folder && other.m_mediaType == m_mediaType && other.m_lineNumber == m_lineNumber) {
        return true;
    }
    return false;
}

QString MediaFolder::folder() const {
    return m_folder;
}

MediaFolder::MediaType MediaFolder::mediaType() const {
    return m_mediaType;
}

/**
 * You must delete list after using it!!!
 */
QList< QStandardItem* >* MediaFolder::row() {
    QList<QStandardItem *>* ret = new QList<QStandardItem *>();
    ret->append(standardItemFolder());
    ret->append(standardItemMediaType());
    return ret;
}

void MediaFolder::setFolder(QString path) {
    m_folder = path;
}

void MediaFolder::setMediaType(MediaFolder::MediaType mediaType) {
    m_mediaType = mediaType;


}

/**
 * must be used in model for delete
 */
QStandardItem* MediaFolder::standardItemFolder() {
    QStandardItem* ret = new QStandardItem();
    ret->setData(m_folder);
    ret->setText(m_folder);
    return ret;
}


/**
 * must be used in model for delete
 */
QStandardItem* MediaFolder::standardItemMediaType() {
    QStandardItem* ret = new QStandardItem();
    ret->setData(m_mediaType);
    ret->setText(mediaTypeToString());
    return ret;
}

QString MediaFolder::mediaTypeToString() {
    QString ret;
    switch (m_mediaType) {
    case VIDEO:
        ret = tr("Video");
        break;
    case AUDIO:
        ret = tr("Audio");
        break;
    case IMAGES:
        ret = tr("Images");
        break;
    default:
        ret = tr("");
    }
    return ret;
}


bool MediaFolder::isValid()
{
    return m_valid;
}

QString MediaFolder::line()
{
    QString line = "";
    switch (m_mediaType) {
    case VIDEO:
        line += "V,";
        break;
    case AUDIO:
        line += "A,";
        break;
    case IMAGES:
        line += "P,";
        break;
    default:
      ;
    }
    line += m_folder;
    return line;
}




