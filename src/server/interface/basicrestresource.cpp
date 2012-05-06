/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) 2012  TomÃ¡Å¡ PolednÃ½ <email>

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


#include "basicrestresource.h"

BasicRESTResource::BasicRESTResource(QString address, QObject* parent):RESTResource(address, parent)
{

}

BasicRESTResource::~BasicRESTResource()
{

}


QDomDocument* BasicRESTResource::resource()
{
    return m_document;
}

/**
 * @param xml without xml header
 */
bool BasicRESTResource::setXML(QByteArray xml)
{
    xml = "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n" + xml;
    m_document = new QDomDocument();

    return m_document->setContent(xml);

}




