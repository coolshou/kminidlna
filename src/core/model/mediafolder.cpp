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

MediaFolder::MediaFolder(QString folder, MediaFolder::MediaType mediaType, QObject* parent): QObject(parent), m_lineNumber(-1)
{
    m_folder = new QStandardItem(folder);
    m_mediaType = new QStandardItem();
    setMediaType(mediaType);
    m_row.append(m_folder);
    m_row.append(m_mediaType);
}

MediaFolder::MediaFolder(QString line, int lineNumber, QObject* parent): QObject(parent), m_lineNumber(lineNumber)
{

}

MediaFolder::~MediaFolder()
{

}


bool MediaFolder::operator==(const MediaFolder& other) const
{
    return false;
}

QString MediaFolder::folder()
{
    return m_folder->data().toString();
}

MediaFolder::MediaType MediaFolder::mediaType()
{
    return *(MediaType *) m_mediaType->data().data();
}

QList< QStandardItem* >& MediaFolder::row()
{
    return m_row;
}

void MediaFolder::setFolder(QString path)
{
    m_folder->setText(path);
}

void MediaFolder::setMediaType(MediaFolder::MediaType mediaType)
{
    m_mediaType->setData(mediaType);

    switch (mediaType) {
    case VIDEO:
        m_mediaType->setText(i18n("Video"));
        break;
    case AUDIO:
        m_mediaType->setText(i18n("Audio"));
        break;
    case IMAGES:
        m_mediaType->setText(i18n("Images"));
        break;
    default:
        m_mediaType->setText(i18n(""));
    }
}
