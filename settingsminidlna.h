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


#ifndef SETTINGSMINIDLNA_H
#define SETTINGSMINIDLNA_H

#include <QWidget>
#include <QCheckBox>
#include <QLineEdit>
#include <QToolButton>

class QToolButton;
class SettingsMiniDLNA : public QWidget
{
  Q_OBJECT
public:
    explicit SettingsMiniDLNA(QWidget* parent = 0, Qt::WindowFlags f = 0);
    virtual ~SettingsMiniDLNA();
    void applySettings();
private:
    void initGUI();
    QLineEdit* m_minidlnaPath;
    QToolButton* m_browsePath;
    QLineEdit* m_pidFilePath;
    QToolButton* m_pidbrowsePath;
    QCheckBox* m_loadFile;
    void loadSettings();
private slots:
    void onBrowsePath();
public slots:
    void onPidBrowsePath();
};

#endif // SETTINGSMINIDLNA_H
