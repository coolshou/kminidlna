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
#include "settingsminidlna.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <KLocalizedString>
#include <KGlobal>
#include <KConfigGroup>
#include <KConfig>
#include <QDebug>
#include <KFileDialog>
#include <QGroupBox>
#include <KUrl>


SettingsMiniDLNA::SettingsMiniDLNA(QWidget* parent, Qt::WindowFlags f): QWidget(parent, f)
{
    initGUI();
}

SettingsMiniDLNA::~SettingsMiniDLNA()
{

}
void SettingsMiniDLNA::initGUI()
{
  
    QVBoxLayout* central = new QVBoxLayout(this);
    QGroupBox* groupmini = new QGroupBox(i18n("minidlna"), this);
    central->addWidget(groupmini);
    QVBoxLayout* vl = new QVBoxLayout(groupmini);
    
    QHBoxLayout* hl =  new QHBoxLayout();
    
    hl->addWidget(new QLabel(i18n("minidlna path:"),groupmini));
    
    m_minidlnaPath = new QLineEdit("/usr/sbin/minidlna", groupmini);
    hl->addWidget(m_minidlnaPath);
    
    m_browsePath = new QToolButton(groupmini);
    m_browsePath->setIcon(KIcon("document-open"));
    connect(m_browsePath, SIGNAL(pressed()), this, SLOT(onBrowsePath()));
    hl->addWidget(m_browsePath);
    
    vl->addLayout(hl);

    loadSettings();
}


void SettingsMiniDLNA::applySettings()
{
    KConfigGroup config = KGlobal::config()->group( "minidlna" );
    config.writeEntry("path", m_minidlnaPath->text());
    config.sync();
}

void SettingsMiniDLNA::loadSettings()
{
    KConfigGroup config = KGlobal::config()->group( "minidlna" );
    m_minidlnaPath->setText(config.readEntry("path", "/usr/sbin/minidlna"));
}

void SettingsMiniDLNA::onBrowsePath()
{
    KUrl url = KFileDialog::getOpenUrl(KUrl(m_minidlnaPath->text()), "minidlna");
    m_minidlnaPath->setText(url.path());
    
}



