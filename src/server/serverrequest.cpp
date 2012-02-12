/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) 2011  TomÃ¡Å¡ PolednÃ½ <email>

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


#include "serverrequest.h"
#include <QDebug>
#include <QCryptographicHash>
#include "restserver.h"

ServerRequest::ServerRequest(): m_method(ServerRequest::NONE)
{

}

ServerRequest::ServerRequest(const ServerRequest& other)
{
}

ServerRequest::~ServerRequest()
{
    qDeleteAll<QHash <QString, QStringList* >::iterator>(m_header.begin(), m_header.end());
}

/**
 *
 * @param key of header
 * @return QStringList of line or empty QStringList
 */
const QStringList* ServerRequest::headerLine(const QString& key) const
{
    return m_header.value(key);
}

/**
 *
 * @return true if method is recognized false if method is not recognized
 */
bool ServerRequest::parseFirstLine()
{

//     if (m_firstLine->isEmpty()) {
    if (m_firstLine[0] == "GET") {
        setMethod(GET);
    } else if (m_firstLine[0] == "PUT") {
        setMethod(PUT);
    } else if (m_firstLine[0] == "POST") {
        setMethod(POST);
    } else {
        setMethod(NONE);
        return false;
    }

    return true;
//     }
//     return false;
}

/**
 *
 * TODO make better authorization 
 */
bool ServerRequest::isAuthorized()
{
    if (RESTServer::login.isEmpty() || RESTServer::password.isEmpty()) {
        return false;
    }
    if (!m_header.isEmpty()) {
        if (m_header.contains("Authorization")) {
            QStringList* auth = m_header.value("Authorization");//get line with authorization

            if (auth->length() > 2) {
                if (auth->at(1) == "Basic") {
                    QList<QByteArray> loginPass = QByteArray::fromBase64(auth->at(2).toUtf8()).split(':');
                    if (loginPass.count() == 2) {
                        if (loginPass[0].toLower() == RESTServer::login.toLower()
                                && RESTServer::password == QCryptographicHash::hash(loginPass[1], QCryptographicHash::Md5)) {
			    //TODO save password for quick authorization
                            return true;
                        }
                    }
                }

            }
        }
    }
    return false;
}

bool ServerRequest::insertRawHeaderLine(QString line)
{
    if (line == "\r\n") {
        return false;
    }
    QStringList* ln = new QStringList(line.split(QRegExp("[ \r\n][ \r\n]*")));
    if (!ln->isEmpty()) {
        m_header.insert((*ln)[0].remove(":"), ln); //remove :
        return true;
    }
    return false;
}

bool ServerRequest::setFirstLine(QString firstLine)
{
    m_firstLine = firstLine.split(QRegExp("[ \r\n][ \r\n]*"));
    return parseFirstLine();
}

/**
 * @return address
 */
QString ServerRequest::path() const
{
    if (m_firstLine.length()>2) {
        return m_firstLine[1];
    }
    return "";
}

bool ServerRequest::arePassesEquel(const QByteArray& plainPass, const QByteArray& hashPass)
{
    QByteArray hashed = QCryptographicHash::hash(plainPass, QCryptographicHash::Md5);
    if (hashed == hashPass) {
        return true;
    }
    return false;
}





