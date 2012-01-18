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


#ifndef RESTRESOURCE_H
#define RESTRESOURCE_H
#include <QString>
#include <QDomDocument>
#include <QXmlSchema>
#include <QXmlSchemaValidator>

class RESTresource
{

public:
    explicit RESTresource();
    RESTresource(const RESTresource& other);
    virtual ~RESTresource();
    bool operator== (const RESTresource& other);
    virtual QString addres();
    virtual void setAddres(QString addres);
    virtual QDomDocument* resource() = 0;
    virtual bool setResource(QDomDocument* resource) = 0;
    virtual bool isValidResource(QDomDocument* resource);
protected:
    QDomDocument* m_document;
    QString m_addres;
    QXmlSchema m_schema;
};

#endif // RESTRESOURCE_H
