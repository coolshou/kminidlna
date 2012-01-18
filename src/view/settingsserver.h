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


#ifndef SETTINGSSERVER_H
#define SETTINGSSERVER_H

#include <QWidget>
#include <KPageWidgetItem>
#include <QSpinBox>
#include <QLabel>
#include <QPushButton>
#include <QCheckBox>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLineEdit>
#include "abstractsettings.h"

class SettingsServer : public AbstractSettings
{
    Q_OBJECT
public:
    explicit SettingsServer(QWidget* parent = 0, Qt::WindowFlags f = 0);
    virtual ~SettingsServer();
    void applySettings();
    void setDefaults();
private:
    void initGUI();
    void loadSettings();
    QSpinBox* m_port;
    QLineEdit* m_username;
    QLineEdit* m_password;
    QPushButton* m_btnControll;
    QCheckBox* m_onStartRun;

};


#endif // SETTINGSSERVER_H
