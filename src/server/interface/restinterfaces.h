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


#ifndef RESTINTERFACES_H
#define RESTINTERFACES_H
#include <QList>
#include <QString>
#include "restresource.h"
#include <QStringList>

class RESTInterfaces
{

public:
    RESTInterfaces();
    virtual ~RESTInterfaces();
    QStringList aresses();
    void addResource(RESTresource* res);
    bool removeResource(RESTresource* res);
protected:
    QList<RESTresource* > m_resource;
    QStringList m_addresses;
};

#endif // RESTINTERFACES_H
