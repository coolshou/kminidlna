/*
http://github.com/coolshou/qminidlna

Copyright (C) 2018 jimmy

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


#ifndef SERVERREQUEST_H
#define SERVERREQUEST_H
#include <QHash>
#include <QString>
#include <QStringList>
#include <QByteArray>
#include <QObject>


/**
 * request incoming from client
 */
class ServerRequest : public QObject
{
Q_OBJECT
    public:
        ServerRequest(QObject* parent = 0);
        virtual ~ServerRequest();

        /**
        * enum with HTTP method
        */
        enum RequestMethod {
            GET = 0,
            PUT,
            POST,
            NONE = 10
        };

        enum HeaderLine {
            BADLINE = -1,
            HEADEREND = 0,
            NORMAL = 1
        };

        bool isAuthorized();
        HeaderLine insertRawHeaderLine(QString line);
        QString path() const;
        const QStringList* headerLine(const QString& key) const;
        bool setFirstLine(QString firstLine);
        static bool arePassesEquel(const QByteArray& plainPass, const QByteArray& hashPass);
        void appendToContent(const QByteArray& con);
        QString firstLine();



        /**
        * inline part
        */

        inline RequestMethod method() const {
            return m_method;
        };

        inline void setMethod(RequestMethod method) {
            m_method = method;
        };

        inline bool hasHeaderLine(const QString& key) const {
            return m_header.contains(key);
        };

        inline const QByteArray* content() const {
            return &m_content;
        };
        inline void setContent(const QByteArray& content) {
            m_contentLength += content.length();
            m_content = content;
        };

        /**
         *
         * @return number of headers line without first line of header
         */
        inline int numberOfHeadersLine() {
            return m_numOfHeaderLine;
        }

        inline int lengthOfContent() {
            return m_contentLength;
        }

        inline bool isSetFirstLine() {
            return m_settedFirstLine;
        }

        inline bool endHeader() {
            return m_endHeader;
        }

    private:
        QStringList m_firstLine;
        RequestMethod m_method;
        QHash<QString, QStringList*> m_header;
        QByteArray m_content;
        bool parseFirstLine();
        int m_numOfHeaderLine;
        int m_contentLength;
        bool m_endHeader;
        bool m_settedFirstLine;
};



#endif // SERVERREQUEST_H
