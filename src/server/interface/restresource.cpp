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


#include "restresource.h"

RESTresource::RESTresource(): m_document(0)
{

}

RESTresource::~RESTresource()
{
    if (m_document != 0) {
        delete m_document;
    }
}

QString RESTresource::addres()
{
    return m_addres;
}

void RESTresource::setAddres(QString addres)
{
    m_addres = addres;
}

bool RESTresource::isValidResource(QDomDocument* resource)
{
    if (!m_schema.isValid()) {
        return false;
    }

    QXmlSchemaValidator validator(m_schema);

//     if(validator.val
    //FIXME
    return true;
}

bool RESTresource::operator==(const RESTresource& other)
{
    if (other.m_addres == m_addres && other.m_document == m_document) {
        return true;
    }
    return false;
}




