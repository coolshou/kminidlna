/*
qminidlna
http://github.com/coolshou/qminidlna

Copyright (C) 2018 jimmy

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


#ifndef MINIDLNAPROCESS_H
#define MINIDLNAPROCESS_H
#include <QProcess>

#include "pidthread.h"
#include "../server/interface/restinterfaces.h"
#include "../server/interface/restminidlna.h"
#include "configurationfile.h"


/**
 * @patern singleton
 */
class MiniDLNAProcess: public QObject, public RESTInterfaces {
        Q_OBJECT

    public:
        static MiniDLNAProcess* getInstance();

        void minidlnaStart();
        void minidlnaKill();
        bool minidlnaStatus();
        void loadConfiguration();
        ConfigurationFile* configFile();
        void configFileChanged();

        //Static
        static const QString MINIDLNA_PATH;
        static const QString PIDFILE_PATH;
        static const QString GLOBALCONFFILE_PATH;
        static const int DEFAULTPORT;

    protected:
        MiniDLNAProcess();
        MiniDLNAProcess(const MiniDLNAProcess &); // hide copy constructor
        MiniDLNAProcess& operator=(const MiniDLNAProcess &); // hide assign op
        virtual ~MiniDLNAProcess();

    private:
        void loadSettings();
        void loadConfigFile();
        QProcess* minidlna;
        QString minidlnas;
        QStringList arg;
        QString pathPidFile;
        PidThread* t_pid;
        QString m_confFilePath;

        bool m_fullRescanFile;
        int m_port;
        ConfigurationFile::ConfFile m_usedConfFile;
        void setArg();
        void loadResource();
        void createConfigFile(QString path);
        bool m_isConfigFileOptionsChanged;
        ConfigurationFile* m_confFile;

    public slots:
        void onPidFile(bool found);

    signals:
        void minidlnaStatus(QProcess::ProcessState state);
        void configurationFileChanged();

};

// namespace MiniDLNA {
//
// }

#endif // MINIDLNAPROCESS_H
