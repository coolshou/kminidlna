#include <KAboutData>
#include <KApplication>
#include <KCmdLineArgs>

#include "KminiDLNA.h"
#include <kicon.h>

int main(int argc, char** argv)
{    
  
    Q_INIT_RESOURCE(resource);
    KAboutData aboutData("kminidlna",
			  0,
			 ki18n("KminiDLNA"),
			 "0.1",
			 ki18n("Frotnend for minidlna"),
			 KAboutData::License_GPL_V2);
    
    aboutData.addAuthor(ki18n("Saljack"));
    aboutData.setProgramLogo(qVariantFromValue(QImage(":/images/ikona.png")));
    KCmdLineArgs::init(argc, argv, &aboutData);
    KApplication app;
    app.setWindowIcon(QIcon(":/images/ikona.png"));
    KminiDLNA *mainWindow = new KminiDLNA();
    mainWindow->show();
    return app.exec();
}
