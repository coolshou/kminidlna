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
    QVBoxLayout* vl = new QVBoxLayout(this);
    setLayout(vl);
    
    QHBoxLayout* hl =  new QHBoxLayout(this);
    
    hl->addWidget(new QLabel(i18n("minidlna path:"),this));
    
    m_minidlnaPath = new QLineEdit("/usr/sbin/minidlna", this);
    hl->addWidget(m_minidlnaPath);
    
    m_browsePath = new QPushButton(i18n("Browse..."), this);
    connect(m_browsePath, SIGNAL(pressed()), this, SLOT(onBrowsePath()));
    hl->addWidget(m_browsePath);
    
    vl->addLayout(hl);
    qDebug() << "minidlnasettings loads gui";

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
//     KUrl surl = KUrl(m_minidlnaPath->text());
    KUrl url = KFileDialog::getOpenUrl(KUrl(m_minidlnaPath->text()), "minidlna");
    m_minidlnaPath->setText(url.path());
    
}



