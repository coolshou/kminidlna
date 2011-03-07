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


SettingsMiniDLNA::SettingsMiniDLNA ( QWidget* parent, Qt::WindowFlags f ) : QWidget ( parent, f ), d_minidlnaPath("/usr/sbin/minidlna"), d_pidFilePath("/tmp/")
{
    initGUI();
    loadSettings();
}

SettingsMiniDLNA::~SettingsMiniDLNA()
{

}

/**
 * init settings minidlna
 */
void SettingsMiniDLNA::initGUI()
{

    QVBoxLayout* central = new QVBoxLayout ( this );
    QGroupBox* groupmini = new QGroupBox ( i18n ( "minidlna" ), this );
    central->addWidget ( groupmini );

    QVBoxLayout* vl = new QVBoxLayout ( groupmini );
    QHBoxLayout* hl0 =  new QHBoxLayout();

    //PATH to minidlna
    hl0->addWidget ( new QLabel ( i18n ( "minidlna path:" ),groupmini ) );

    m_minidlnaPath = new QLineEdit ( "", groupmini );
    hl0->addWidget ( m_minidlnaPath );

    m_browsePath = new QToolButton ( groupmini );
    m_browsePath->setIcon ( KIcon ( "document-open" ) );
    connect ( m_browsePath, SIGNAL ( pressed() ), this, SLOT ( onBrowsePath() ) );
    hl0->addWidget ( m_browsePath );

    vl->addLayout ( hl0 );

    //PATH pid file
    QHBoxLayout* hl1 = new QHBoxLayout();
    hl1->addWidget ( new QLabel ( i18n ( "Path to pid file:" ), groupmini ) );

    m_pidFilePath = new QLineEdit ( "", groupmini );
    hl1->addWidget ( m_pidFilePath );

    m_pidbrowsePath = new QToolButton ( groupmini );
    m_pidbrowsePath->setIcon ( KIcon ( "document-open" ) );
    connect ( m_pidbrowsePath, SIGNAL ( pressed() ), this, SLOT ( onPidBrowsePath() ) );
    hl1->addWidget ( m_pidbrowsePath );

    vl->addLayout ( hl1 );

    m_loadFile = new QCheckBox(i18n("Scan file on start minidlna"),groupmini);
    vl->addWidget(m_loadFile);

    vl->addSpacerItem(new QSpacerItem(40, 200));
}


void SettingsMiniDLNA::applySettings()
{
    KConfigGroup config = KGlobal::config()->group ( "minidlna" );
    config.writeEntry ( "minidlnapath", m_minidlnaPath->text() );
    config.writeEntry("pidpath", m_pidFilePath->text());
    config.writeEntry("scanfile", m_loadFile->isChecked());
    config.sync();
}

void SettingsMiniDLNA::loadSettings()
{
    KConfigGroup config = KGlobal::config()->group ( "minidlna" );
    m_minidlnaPath->setText ( config.readEntry ( "minidlnapath", d_minidlnaPath ) );
    m_pidFilePath->setText(config.readEntry("pidpath", d_pidFilePath));
    m_loadFile->setChecked(config.readEntry("scanfile", false));
}

void SettingsMiniDLNA::onBrowsePath()
{
    KUrl url = KFileDialog::getOpenUrl ( KUrl ( m_minidlnaPath->text() ), "minidlna", this, i18n("minidlna path") );
    if (!url.isEmpty()) {
        m_minidlnaPath->setText ( url.path() );
    }
}

void SettingsMiniDLNA::onPidBrowsePath()
{
//     KUrl url = KFileDialog::getExistingDirectory(KUrl ( m_pidFilePath->text() ), this, i18n("Pid file directory"));
    QString path = KFileDialog::getExistingDirectory(KUrl ( m_pidFilePath->text()), this, i18n("Pid file directory") + "/" );
    if (!path.isEmpty()) {
        m_pidFilePath->setText (path);
    }
}

void SettingsMiniDLNA::setDefaults()
{
    m_minidlnaPath->setText(d_minidlnaPath);
    m_pidFilePath->setText(d_pidFilePath);
    m_loadFile->setChecked(true);    
}



