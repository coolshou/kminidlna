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


#ifndef RESTMINIDLNAMEDIAFOLDER_H
#define RESTMINIDLNAMEDIAFOLDER_H

#include "restresource.h"
#include "../../core/model/mediafolder.h"
#include <QDomDocument>


class RESTMediaFolder : public RESTResource
{

public:
    explicit RESTMediaFolder(QObject* parent = 0, QString address = "/mediafolders.xml" );
    virtual QDomDocument* resource();
    virtual ~RESTMediaFolder();
private:
    void appendFolder(MediaFolder* folder, QDomDocument* doc);
};

#endif // RESTMINIDLNAMEDIAFOLDER_H
