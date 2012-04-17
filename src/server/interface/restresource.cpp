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
#include <QFile>

RESTresource::RESTresource(QString address, QObject* parent)
    : QObject(parent), m_document(0), m_address(address), m_schema(0), m_schemaLoaded(false), m_PUT(false), m_GET(true) {

}

RESTresource::~RESTresource() {
    if(m_document != 0) {
        delete m_document;
    }
    if(m_schema != 0) {
        delete m_schema;
    }
}

QString RESTresource::address() {
    return m_address;
}

void RESTresource::setAddress(QString address) {
    m_address = address;
}

bool RESTresource::isValidResource(QDomDocument* resource) {
    if(m_schema == 0 || !m_schema->isValid()) {
        return false;
    }

    QXmlSchemaValidator validator(*m_schema);
    return validator.validate(resource->toByteArray());
}

bool RESTresource::operator==(const RESTresource& other) {
    if(other.m_address == m_address && other.m_document == m_document) {
        return true;
    }
    return false;
}

bool RESTresource::loadSchema(QString path) {
    if(m_schema != 0) {
        delete m_schema;
    }
    m_schema = new QXmlSchema();
    QFile file(path);
    m_schemaLoaded = false;
    if(file.open(QIODevice::ReadOnly)) {
        m_schemaLoaded = m_schema->load(&file);
    }

    if(file.isOpen()) {
        file.close();
    }

    if(!m_schemaLoaded) {
        delete m_schema;
        m_schema = 0;
    }

    return m_schemaLoaded;
}

bool RESTresource::isXmlSchemaSetted() {
    return m_schemaLoaded;
}

QXmlSchema* RESTresource::schema() {
    return m_schema;
}

//HTTP Method part
/**
 * @return false (not implemented http method)
 */
bool RESTresource::hasDELETEMethod() {
    return false;
}

/**
 * @return false (not implemented http method)
 */
bool RESTresource::hasPOSTMethod() {
    return false;
}

/**
 * @return true if has http method other false. Default is true.
 */
bool RESTresource::hasGETMethod() {
    return m_GET;
}

/**
 * @return true if has http method other false. Default is false.
 */
bool RESTresource::hasPUTMethod() {
    return m_PUT;
}

/**
 * default is true
 */
void RESTresource::setGETMethod(bool get) {
    m_GET = get;
}

/**
 * default is false
 */
void RESTresource::setPUTMethod(bool put) {
    m_PUT = put;
}









