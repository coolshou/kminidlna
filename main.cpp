/*
KminiDLNA
http://gitorious.org/kminidlna/pages/Home

Copyright (C) 2011 Saljack <saljacky a gmail dot com>

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
version 2 as published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#include <KAboutData>
#include <KApplication>
#include <KCmdLineArgs>


#include "kminidlna.h"

int main(int argc, char** argv)
{    
  
    Q_INIT_RESOURCE(resource);
    KAboutData aboutData("kminidlna",
			  0,
			 ki18n("KminiDLNA"),
			 "0.1",
			 ki18n("Frotnend for minidlna"),
			 KAboutData::License_GPL_V2);
    
    aboutData.addAuthor(ki18n("Tomáš Poledný"), ki18n("Author"), QByteArray("saljacky@gmail.com"));
    aboutData.setProgramLogo(qVariantFromValue(QImage(":/images/ikona.png")));
    aboutData.setBugAddress("Author email");
    KCmdLineArgs::init(argc, argv, &aboutData);
    KApplication app;
    app.setWindowIcon(QIcon(":/images/ikona.png"));
    KminiDLNA *mainWindow = new KminiDLNA();
    mainWindow->show();
    return app.exec();
}
