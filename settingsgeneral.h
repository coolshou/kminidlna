/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) 2011  Tomáš Poledný <email>

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


#ifndef SETTINGSGENERAL_H
#define SETTINGSGENERAL_H

#include <QWidget>
#include <QCheckBox>


class SettingsGeneral : public QWidget
{
    Q_OBJECT

public:
    explicit SettingsGeneral(QWidget* parent = 0, Qt::WindowFlags f = 0);
    virtual ~SettingsGeneral();
    void applySettings();
private:
    void initGUI();
    QCheckBox* m_closeToTray;
    QCheckBox* m_runOnStart;
    QCheckBox* m_remRun;
    void loadSettings();
};

#endif // SETTINGSGENERAL_H
