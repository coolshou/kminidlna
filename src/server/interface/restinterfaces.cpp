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


#include "restinterfaces.h"
#include <QDebug>

RESTInterfaces::RESTInterfaces()
{
}

RESTInterfaces::~RESTInterfaces()
{

}


void RESTInterfaces::addResource(RESTResource* res)
{
    if (!m_resources.contains(res->address())) {
        m_resources[res->address()] = res;
    }
}

bool RESTInterfaces::removeResource(RESTResource* res)
{
    if (m_resources.contains(res->address())) {
        if (m_resources.remove(res->address()) > 0) {
            return true;
        }

    }
    return false;
}

QStringList RESTInterfaces::adresses()
{
    QStringList list(m_resources.keys());
    return list;
}

bool RESTInterfaces::hasResourceOnAddress(QString address)
{
//   qDebug() << "RESTInterfaces::hasResourceOnAddress:" << address;
    int count = m_resources.contains(address);
    if (count == 1) {
        return true;
    }else if(count > 1){
      qDebug() << "RESTInterfaces: On address \"" << address << " are more resources";
      return true;
    }
    return false;
}

RESTResource* RESTInterfaces::resourceOnAddress(QString address)
{
    if (hasResourceOnAddress(address))
        return m_resources[address];
    return 0;
}