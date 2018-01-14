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


#include "restminidlnaput.h"
#include "../../core/minidlna_process.h"

RESTMiniDLNAPUT::RESTMiniDLNAPUT(MiniDLNAProcess* process, QString address): RESTResource(address, process) {
    //set implemented method
    setPUTMethod(true);
    setGETMethod(false);

    loadSchema(":/schema/set_state.xsd");
}

RESTMiniDLNAPUT::~RESTMiniDLNAPUT() {

}

bool RESTMiniDLNAPUT::processResource(const QDomDocument& resource) {
    QDomText state = resource.documentElement().firstChildElement("run").firstChild().toText();
    if(state.data() == "start") {
        MiniDLNAProcess::getInstance()->minidlnaStart();
    } else if(state.data() == "stop") {
        MiniDLNAProcess::getInstance()->minidlnaKill();
    }
    return true;
}


