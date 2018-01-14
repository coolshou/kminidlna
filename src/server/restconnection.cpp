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


#include "restconnection.h"
#include <QDebug>
#include <QPointer>

RESTConnection::RESTConnection(QTcpSocket* socket, RESTServer* server, QObject* parent):
    QThread(parent), m_server(server), m_socket(socket), m_request(0) {
//     m_recievedData.clear();
}

void RESTConnection::run() {
    connect(m_socket, SIGNAL(disconnected()), this, SLOT(connectionClosed()));
    connect(m_socket, SIGNAL(readyRead()), this, SLOT(receiveHeader()));
    exec();
}



RESTConnection::~RESTConnection() {
    m_socket->deleteLater();
    if(m_request != 0) {
        delete m_request;
    }
}

void RESTConnection::connectionClosed() {
    m_socket->close();
    exit();
}

void RESTConnection::send404NotFound() {
//     qDebug() << "RESTServer: File not found";
    if(m_socket->isWritable()) {
        QByteArray nof = "HTTP/1.1 404 Not Found\r\n"
                         "Content-Type: text/html; charset=\"utf-8\"\r\n"
                         "\r\n";
        m_socket->write(nof);
//         QFileInfo infonof(m_notFoundFileHtmlPath);
//         if(infonof.isFile() && infonof.isReadable()) {
//             QFile nof404(m_notFoundFileHtmlPath);
//             if(nof404.open(QIODevice::ReadOnly)) {
//                 m_socket->write(nof404.readAll());
//             }
//         } else {
        QByteArray html = "<html>\n"
                          "<head><title>404 Not Found</title></head>\n"
                          "\t<body><h1>Not Found</h1>The requested document was not found on this server.</body>\n"
                          "</html>\n";
        m_socket->write(html);
//         }
        m_socket->flush();
    }
}
void RESTConnection::sendNoContent() {
    sendMSG("204 No Content");
}

void RESTConnection::sendMSG(const QString msg) {
    QByteArray send = "HTTP/1.1 " + msg.toUtf8() + "\r\n"
                      "\r\n";
    if(m_socket->isWritable()) {
        m_socket->write(send);
        m_socket->flush();
    } else {
        qDebug() << "RESTConnection: cannot send msg: " << msg << " socket " << m_socket->socketDescriptor() << "is not writable";
    }
}

/**
 * close socket and call exit on thread
 */
void RESTConnection::closeConnection() {
    m_socket->close();
    exit();
}


void RESTConnection::receiveHeader() {
    QMutexLocker l(&lock);
    if(m_request == 0) {
        m_request = new ServerRequest(this);
    } else if(m_request->endHeader()) {
        receiveData();
        return;
    }

    //First line
    if(!m_request->isSetFirstLine()) {
        m_line += m_socket->readLine();
        if(m_socket->atEnd()) {
            return;
        } else {
            if(!m_request->setFirstLine(m_line)) {
                //TODO check if is true
                closeConnection();
            }
            m_line.clear();
        }
    }

    while(m_socket->canReadLine() && !m_request->endHeader()) {
        QByteArray tmp;
        if(m_line.isEmpty()) {
            tmp = m_socket->readLine(MAXLENGTH_DATA); //TODO create new const
        } else {
            tmp = m_line;
            tmp += m_socket->readLine(MAXLENGTH_DATA); //TODO create new const
            m_line.clear();
        }
        if(m_socket->atEnd()) {
            if(!tmp.endsWith("\r\n")) {
                return;
            }
        }
        ServerRequest::HeaderLine headerLine = m_request->insertRawHeaderLine(tmp);
        //TODO if headerLine is BAD close connection

        if(m_request->numberOfHeadersLine() > MAXHEADERLINES) {
            closeConnection();
            return;
        }
    }

    if(m_request->endHeader()) {
        processHeader();
    }
}

void RESTConnection::processHeader() {
    if(m_request->isAuthorized()) {
        switch(m_request->method()) {
        case ServerRequest::GET:
            processGETAndSendReply();
            break;
        case ServerRequest::PUT:
            receiveData();
            break;
        case ServerRequest::POST:
            //TODO send not implemented
            break;
        case ServerRequest::NONE:
            //TODO send not implemented
            break;
        }
    } else {
        QByteArray ok = "HTTP/1.1 401 Unauthorized\r\n"
                        "WWW-Authenticate: Basic realm=\"authentication is needed\"\r\n"
                        "\r\n";
        m_socket->write(ok);
        m_socket->flush();
        endTransfer();
    }
}


void RESTConnection::receiveData() {
    if(m_socket->bytesAvailable() > 0) {
        if(m_request->method() == ServerRequest::PUT) {
            if(m_socket->bytesAvailable() < (MAXLENGTH_DATA - m_request->lengthOfContent())) {
                QByteArray con = m_socket->readAll();
                if(m_request->lengthOfContent() == 0) {
                    m_request->setContent(con);
                }
            }

            processPUTAndSendReply();
        } else {
            //is not put method error
            delete m_request;
            m_request = 0;
            closeConnection();
        }
    }
}

void RESTConnection::processPUTAndSendReply() {
    QString address = m_request->path();

    //TODO check if file is xml
    RESTResource* res = m_server->resource(address);

    if(res == 0 || !res->hasPUTMethod()) {
        sendNoContent(); //FIXME send right code
    } else {
        QDomDocument doc;

        if(doc.setContent(* (m_request->content()))) {
            if(res->setResource(doc)) {
                sendMSG("200 Ok");
            } else {
                sendNoContent(); //FIXME send right code error resource not setted
            }
            m_socket->flush();
        }
    }
    endTransfer();
}

void RESTConnection::processGETAndSendReply() {
    QString address = m_request->path();

    RESTResource* res = m_server->resource(address);
    if(res == 0 || !res->hasGETMethod()) {
        send404NotFound();
    } else if(m_socket->isWritable()) {
        QByteArray ok = "HTTP/1.1 200 Ok\r\n"
                        "Content-Type: text/xml; charset=\"utf-8\"\r\n"
                        "\r\n";
        m_socket->write(ok);
        m_socket->write(res->resource()->toByteArray());
        m_socket->flush();
    }
    endTransfer();
}

/**
 * delete m_request and close connection
 *
 * TODO implement keep-alive
 */
void RESTConnection::endTransfer() {
    closeConnection();
    delete m_request;
    m_request = 0;
}

