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
    void setDefaults();
private:
    void initGUI();
    QCheckBox* m_closeToTray;
    QCheckBox* m_runOnStart;
    QCheckBox* m_remRun;
    void loadSettings();
};

#endif // SETTINGSGENERAL_H
