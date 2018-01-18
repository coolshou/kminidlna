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


#ifndef RESTCONNECTION_H
#define RESTCONNECTION_H

#include <QThread>
#include <QTcpSocket>
#include <QNetworkRequest>
#include <QByteArray>
#include <QMutex>
#include <QPointer>
#include "restserver.h"
#include "interface/restresource.h"
#include "serverrequest.h"


class RESTConnection : public QThread {
        Q_OBJECT
    public:
        explicit RESTConnection(QTcpSocket* socket, RESTServer* server, QObject* parent = 0);
        virtual ~RESTConnection();
        void endTransfer();

    protected:
        virtual void run();


    protected slots:
        void connectionClosed();
        void receiveData();
        void receiveHeader();

    private:
//         QString m_notFoundFileHtmlPath;
        static const int MAXLENGTH_DATA = 256;
        static const int MAXHEADERLINES = 40;

        RESTServer* m_server;
        QTcpSocket* m_socket;
        QByteArray m_recievedData;
        ServerRequest* m_request;
        QMutex lock;
        bool m_allRecieved;
        bool m_headerRecieved;
        QByteArray m_line;

        void sendNoContent();
        void sendMSG(const QString msg);
        void send404NotFound();
        void processGETAndSendReply();
        void processPUTAndSendReply();
        void closeConnection();
        void processHeader();
};

#endif // RESTCONNECTION_H
