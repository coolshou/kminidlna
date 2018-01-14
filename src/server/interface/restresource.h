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
#include <QObject>
#include <QDomDocument>
#include <QXmlSchema>
#include <QXmlSchemaValidator>

class RESTResource: public QObject
{
        Q_OBJECT
    public:
        explicit RESTResource(QString address, QObject* parent = 0);
//         RESTResource(const RESTResource& other);
        virtual ~RESTResource();
        bool operator== (const RESTResource& other);
        virtual QString address();
        virtual void setAddress(QString address);
        virtual QDomDocument* resource();
        virtual bool setResource(const QDomDocument& resource);
        virtual bool isValidResource(const QDomDocument& resource);
        virtual bool isXmlSchemaSetted();
        virtual QXmlSchema* schema();
        virtual bool loadSchema(QString path);
        //Method
        virtual bool hasPUTMethod();
        virtual bool hasGETMethod();
        virtual void setPUTMethod(bool put);
        virtual void setGETMethod(bool get);

        virtual bool hasDELETEMethod();
        virtual bool hasPOSTMethod();

    protected:
        virtual bool processResource(const QDomDocument& resource);
        QDomDocument* m_document;
        QString m_address;
        QXmlSchema* m_schema;
        bool m_schemaLoaded;
        bool m_PUT;
        bool m_GET;

};

#endif // RESTRESOURCE_H
