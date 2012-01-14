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


#ifndef SETTINGSMINIDLNA_H
#define SETTINGSMINIDLNA_H

#include <QWidget>
#include <QCheckBox>
#include <QLineEdit>
#include <QToolButton>
#include <QLabel>

class QToolButton;
class SettingsMiniDLNA : public QWidget
{
    Q_OBJECT
public:
    explicit SettingsMiniDLNA(QWidget* parent = 0, Qt::WindowFlags f = 0);
    virtual ~SettingsMiniDLNA();
    void applySettings();
    void setDefaults();
private:
    void initGUI();
    QLineEdit* m_minidlnaPath;
    QToolButton* m_browsePath;
    QLineEdit* m_pidFilePath;
    QToolButton* m_pidbrowsePath;
    QLineEdit* m_confFilePath;
    QToolButton* m_browseConfFile;
    QLabel* m_lblConfFile;
    QCheckBox* m_loadFile;
    QCheckBox* m_checkDefaultPath;
    void loadSettings();
    
private slots:
    void onBrowsePath();
    void onBrowseConfFile();
public slots:
    void onPidBrowsePath();
    void checkedDefautlPath(int);
};

#endif // SETTINGSMINIDLNA_H
