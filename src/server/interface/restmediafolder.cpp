/*
http://github.com/coolshou/qminidlna

Copyright (C) 2018 jimmy

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


#include "restmediafolder.h"
#include "../../core/minidlna_process.h"
#include <QList>
#include <QDebug>

RESTMediaFolder::RESTMediaFolder(QObject* parent, QString address): RESTResource(address, parent)
{
    m_document = new QDomDocument();
}

QDomDocument* RESTMediaFolder::resource()
{
    m_document->clear();
    QByteArray xmlskeleton = "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>"
                             "<mediafolders/>";
    m_document->setContent(xmlskeleton);

    QList<MediaFolder*> folders = MiniDLNAProcess::getInstance()->configFile()->mediaFolders();

    for (QList<MediaFolder*>::const_iterator it = folders.begin(); it != folders.end(); ++it) {
        appendFolder((*it), m_document);
    }
    return m_document;
}

void RESTMediaFolder::appendFolder(MediaFolder* folder, QDomDocument* doc) {
    QDomElement fold = doc->createElement("mediafolder");
    doc->documentElement().appendChild(fold);

    //Path
    QDomElement pathElement = doc->createElement("path");
    fold.appendChild(pathElement);
    QDomText pathText = doc->createTextNode(folder->folder());
    pathElement.appendChild(pathText);

    //Media type
    if (folder->mediaType() != MediaFolder::NONE) {
        QDomElement typeElement = doc->createElement("mediatype");
        fold.appendChild(typeElement);
        QString str;
        switch (folder->mediaType()) {
        case MediaFolder::AUDIO:
            str = "audio";
            break;
        case MediaFolder::VIDEO:
            str = "video";
            break;
        case MediaFolder::IMAGES:
            str = "images";
            break;
        case MediaFolder::NONE:
            str = "none";
            break;
        }
        QDomText typeText = doc->createTextNode(str);
        typeElement.appendChild(typeText);
    }
}

RESTMediaFolder::~RESTMediaFolder()
{

}


