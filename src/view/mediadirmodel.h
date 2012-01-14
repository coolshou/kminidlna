/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) 2011  Tomáš Poledný <email>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/


#ifndef MEDIADIRMODEL_H
#define MEDIADIRMODEL_H

#include <QAbstractItemModel>
#include <QDebug>

class MediaDirItem{
public:
    enum MediaType {
        Video,
        Audio,
        Picture
    };
    MediaDirItem(MediaType mediaType, QString path);
    ~MediaDirItem();

    void setMediaType(MediaType type);
    MediaType getMediaType();
    void setPath(QString path);
    QString getPath();
    
private:
    MediaType mediaType;
    QString path;
protected:
    


};

class MediaDirModel : public QAbstractTableModel
{
public:
    virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
    virtual int rowCount(const QModelIndex& parent = QModelIndex()) const;
    MediaDirModel(QObject* parent = 0);
    MediaDirModel(const MediaDirModel& other);
//     virtual bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole);
//     virtual bool insertColumns(int column, int count, const QModelIndex& parent = QModelIndex());
//     virtual bool insertRows(int row, int count, const QModelIndex& parent = QModelIndex());
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    virtual int columnCount(const QModelIndex& parent = QModelIndex()) const;
//     virtual QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const;
//     virtual QModelIndex parent(const QModelIndex& child) const;

private:
    QList<MediaDirItem*> list;
    void loadData();
    void parseLine(QString line);
};



#endif // MEDIADIRMODEL_H

