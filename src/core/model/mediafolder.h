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


#ifndef MEDIAFOLDER_H
#define MEDIAFOLDER_H
#include <QStandardItemModel>
#include <QList>
//#include <KLocalizedString>


class MediaFolder: public QObject {
  Q_OBJECT
    public:
        enum MediaType {NONE, VIDEO, AUDIO, IMAGES};
        MediaFolder(QString folder, MediaType mediaType = NONE, QObject* parent= 0);
        MediaFolder(QString line, int lineNumber = -1, QObject* parent= 0);
        explicit MediaFolder(QObject* parent= 0);
        MediaFolder(const MediaFolder& other);

        virtual ~MediaFolder();
        virtual bool operator==(const MediaFolder& other) const;
        QList< QStandardItem* >* row();
        void setMediaType(MediaType mediaType);
        void setFolder(QString path);
        MediaType mediaType() const;
        QString folder() const;
        QStandardItem* standardItemMediaType();
        QStandardItem* standardItemFolder();
	QString mediaTypeToString();
	bool isValid();
	QString line();

    private:
        MediaType m_mediaType;
        QString m_folder;
        int m_lineNumber;
	bool m_valid;
};

#endif // MEDIAFOLDER_H
