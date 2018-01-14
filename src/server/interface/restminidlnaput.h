/*
    KMiniDLNA
    https://sourceforge.net/projects/kminidlna/
    
    Copyright (C) 2012  Tomáš Poledný saljacky(at)gmail(dot)com

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


#ifndef RESTMINIDLNAPUT_H
#define RESTMINIDLNAPUT_H

#include "restresource.h"
#include "../../core/minidlna_process.h"


class RESTMiniDLNAPUT : public RESTResource {

    public:
        RESTMiniDLNAPUT(MiniDLNAProcess* process, QString address = "/minidlna-set-state.xml");
        virtual ~RESTMiniDLNAPUT(); 
    protected:
        virtual bool processResource(const QDomDocument& resource);
};

#endif // RESTMINIDLNAPUT_H
