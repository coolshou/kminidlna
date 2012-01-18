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

RESTInterfaces::RESTInterfaces()
{
}

RESTInterfaces::~RESTInterfaces()
{

}

void RESTInterfaces::addResource(RESTresource* res)
{
    if(!m_resource.contains(res)){
      m_resource.append(res);
      m_addresses << res->addres();
    }
}
bool RESTInterfaces::removeResource(RESTresource* res)
{
    if(m_resource.contains(res)){
	m_addresses.removeOne(res->addres());
	return m_resource.removeOne(res);
    }
    return false;
}



