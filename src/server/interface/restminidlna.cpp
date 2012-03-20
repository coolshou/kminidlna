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


#include "restminidlna.h"
#include <QXmlSchema>
#include <QDebug>

RESTMiniDLNA::RESTMiniDLNA(QObject* process, QString address): RESTresource(address, process) {
    setPUTMethod(true);
    //XML
    QByteArray xmlskeleton = "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>"
                             "<minidlna><state>notrunning</state></minidlna>";
    m_document = new QDomDocument();
    if (m_document->setContent(xmlskeleton)) {
        m_stateText =  m_document->documentElement().firstChildElement("state").firstChild().toText();
    }
    //End XML

    loadSchema(":/schema/state.xsd");

    if (process != 0) {
        connect(process, SIGNAL(minidlnaStatus(QProcess::ProcessState)), this, SLOT(onMiniDLNAState(QProcess::ProcessState)));
    }
}

RESTMiniDLNA::~RESTMiniDLNA() {

}

QDomDocument* RESTMiniDLNA::resource() {
    return m_document;
}

bool RESTMiniDLNA::setResource(QDomDocument* resource) {
    if (isValidResource(resource)) {
        QDomText state = resource->documentElement().firstChildElement("state").firstChild().toText();
        if (state.data() == "running") {
            MiniDLNAProcess::getInstance()->minidlnaStart();
        } else if (state.data() == "notrunning") {
            MiniDLNAProcess::getInstance()->minidlnaKill();
        }
        return true;
    }
    return false;
}

void RESTMiniDLNA::onMiniDLNAState(QProcess::ProcessState state) {
    switch (state) {
    case QProcess::Starting:
        m_stateText.setData("starting");
        break;
    case QProcess::Running:
        m_stateText.setData("running");
        break;
    default:
        m_stateText.setData("notrunning");

    }
}


