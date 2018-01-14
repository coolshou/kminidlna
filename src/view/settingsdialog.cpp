/*
qminidlna
http://gitorious.org/qminidlna/pages/Home

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

#include "settingsdialog.h"
#include "settingsgeneral.h"
//#include <KDE/KLocalizedString>
#include <QPushButton>
//#include <KPushButton>
#include <QDebug>
#include <QMessageBox>
//#include <KMessageBox>

SettingsDialog::SettingsDialog(QWidget* parent, Qt::WindowFlags flags): QDialog(parent, flags), m_wasNo(false) {
    initGUI();
}

SettingsDialog::~SettingsDialog() {

}

void SettingsDialog::onApply() {
    /*
    KPageWidgetItem* w = currentPage();
    AbstractSettings* tmp = dynamic_cast<AbstractSettings*>(w->widget());
    if (tmp->isChanged()) {
        tmp->applySettings();
        emit settingsChanged();
    }
    */
    m_apply->setEnabled(false);
}

void SettingsDialog::onDefault() {
    /*
    KPageWidgetItem* w = currentPage();
    AbstractSettings* tmp = dynamic_cast<AbstractSettings*>(w->widget());
    tmp->setDefaults();
    */
}

void SettingsDialog::initGUI() {
    setWindowModality(Qt::WindowModal);
    /*
    setButtons(QDialog::Ok | QDialog::Cancel | QDialog::Apply );
    m_apply = button(QDialog::Apply);
    setFaceType(KPageDialog::List);
    */
    setWindowTitle(tr("Configure qminidlna"));
    setMinimumSize(450, 250);

    m_general = new SettingsGeneral(this);
    connect(m_general, SIGNAL(changed()),
            this, SLOT(onChange()));
    /*
    KPageWidgetItem* generalPage = new KPageWidgetItem(m_general, tr("General"));
    generalPage->setObjectName("gen");
    generalPage->setIcon(QIcon(":/images/ikona.png"));
    addPage(generalPage);
*/
    m_minidlna = new SettingsMiniDLNA(this);
    connect(m_minidlna, SIGNAL(changed()),
            this, SLOT(onChange()));
    /*
    KPageWidgetItem* minidlnaPage = new KPageWidgetItem(m_minidlna, tr("MiniDLNA"));
    minidlnaPage->setObjectName("MiniDLNA");
    minidlnaPage->setIcon(QIcon("configure"));
    addPage(minidlnaPage);
*/
    m_mediaFolders = new SettingsMediaFolder(this);
    connect(m_mediaFolders, SIGNAL(changed()),
            this, SLOT(onChange()));
    /*
    KPageWidgetItem* mediaFoldersPage = new KPageWidgetItem(m_mediaFolders, tr("Media folders"));
    mediaFoldersPage->setObjectName("mediafolder");
    mediaFoldersPage->setIcon(QIcon("folder-video"));
    addPage(mediaFoldersPage);
*/
    m_confFileSettings = new SettingsConfFile(this);
    connect(m_confFileSettings, SIGNAL(changed()),
            this, SLOT(onChange()));
  /*
    KPageWidgetItem* confFilePage = new KPageWidgetItem(m_confFileSettings, tr("MiniDLNA configuration file"));
    confFilePage->setObjectName("conffile");
    confFilePage->setIcon(QIcon("text-plain"));
    addPage(confFilePage);
*/
    m_server = new SettingsServer(this);
    connect(m_server, SIGNAL(changed()),
            this, SLOT(onChange()));
    /*
    KPageWidgetItem* serverPage = new KPageWidgetItem(m_server, tr("Server interface"));
    serverPage->setObjectName("server");
    serverPage->setIcon(QIcon("network-server"));
    addPage(serverPage);
*/
    //Buttons clicked
    connect(this, SIGNAL(applyClicked()),
            this, SLOT(onApply()));
    connect(this, SIGNAL(defaultClicked()),
            this, SLOT(onDefault()));
    connect(this, SIGNAL(okClicked()),
            this, SLOT(onApply()));

    connect(this, SIGNAL(currentPageChanged(KPageWidgetItem*, KPageWidgetItem*)),
            this, SLOT(curentPageChanged(KPageWidgetItem*, KPageWidgetItem*)));
    m_apply->setEnabled(false);
}

/*
void SettingsDialog::curentPageChanged(KPageWidgetItem* current, KPageWidgetItem* before) {
    if (m_wasNo) {
        m_wasNo = false;
        return;
    }
    AbstractSettings* tmp = dynamic_cast<AbstractSettings*>(before->widget());

    if (tmp->isChanged()) {
        setCurrentPage(before);
        int ret = QMessageBox::warningYesNoCancel(this, tr("Settings was changed.\nDo you want apply changes?"), tr("Setting was changed"));
        switch (ret) {
        case QMessageBox::Yes:
            onApply();
            setCurrentPage(current);
            break;
        case QMessageBox::No:
            tmp->loadSettings();
            m_apply->setEnabled(false);
            m_wasNo = true;
            setCurrentPage(current);
            break;
        }
    }
}
*/
void SettingsDialog::onChange() {
//     if(!m_apply->isEnabled()){
    m_apply->setEnabled(true);
//     }
}





