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

#include "settingsdialog.h"
#include "settingsgeneral.h"
#include <KDE/KLocalizedString>
#include <KPushButton>
#include <QDebug>
#include <KMessageBox>

SettingsDialog::SettingsDialog(QWidget* parent, Qt::WFlags flags): KPageDialog(parent, flags) {
    initGUI();
}

SettingsDialog::~SettingsDialog() {

}

void SettingsDialog::onApply() {
    KPageWidgetItem* w = currentPage();
    AbstractSettings* tmp = dynamic_cast<AbstractSettings*>(w->widget());
    if (tmp->isChanged()) {
        tmp->applySettings();
    }
    m_apply->setEnabled(false);
}

void SettingsDialog::onDefault() {
    KPageWidgetItem* w = currentPage();
    AbstractSettings* tmp = dynamic_cast<AbstractSettings*>(w->widget());
    tmp->setDefaults();
}

void SettingsDialog::initGUI() {
    setWindowModality(Qt::WindowModal);
    setButtons(KDialog::Ok | KDialog::Cancel | KDialog::Apply /*| KDialog::Default*/);
    m_apply = button(KDialog::Apply);
    setFaceType(KPageDialog::List);
    setWindowTitle(i18n("Configure KminiDLNA"));
    setMinimumSize(450, 250);

    m_general = new SettingsGeneral(this);
    connect(m_general, SIGNAL(changed()),
            this, SLOT(onChange()));
    KPageWidgetItem* generalPage = new KPageWidgetItem(m_general, i18n("General"));
    generalPage->setObjectName("gen");
    generalPage->setIcon(KIcon(":/images/ikona.png"));
    addPage(generalPage);

    m_minidlna = new SettingsMiniDLNA(this);
    connect(m_minidlna, SIGNAL(changed()),
            this, SLOT(onChange()));
    KPageWidgetItem* minidlnaPage = new KPageWidgetItem(m_minidlna, i18n("MiniDLNA"));
    minidlnaPage->setObjectName("MiniDLNA");
    minidlnaPage->setIcon(KIcon("configure"));
    addPage(minidlnaPage);

    m_confFileSettings = new SettingsConfFile(this);
    connect(m_confFileSettings, SIGNAL(changed()),
            this, SLOT(onChange()));
    KPageWidgetItem* confFilePage = new KPageWidgetItem(m_confFileSettings, i18n("MiniDLNA configuration file"));
    confFilePage->setObjectName("conffile");
    confFilePage->setIcon(KIcon("text-plain"));
    addPage(confFilePage);

    m_server = new SettingsServer(this);
    connect(m_server, SIGNAL(changed()),
            this, SLOT(onChange()));
    KPageWidgetItem* serverPage = new KPageWidgetItem(m_server, i18n("Server interface"));
    serverPage->setObjectName("server");
    serverPage->setIcon(KIcon("applications-internet"));
    addPage(serverPage);

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

void SettingsDialog::curentPageChanged(KPageWidgetItem* current, KPageWidgetItem* before) {
    AbstractSettings* tmp = dynamic_cast<AbstractSettings*>(before->widget());
    if (tmp->isChanged()) {

        int ret = KMessageBox::warningYesNoCancel(this, i18n("Settings was changed.\nDo you want apply changes?"), i18n("Setting was changed"));
        switch (ret) {
        case KMessageBox::Yes:
            onApply();
            break;
        case KMessageBox::No:
            tmp->loadSettings();
            m_apply->setEnabled(false);
            break;
        default:
            setCurrentPage(before);
        }
    }
}

void SettingsDialog::onChange() {
//     if(!m_apply->isEnabled()){
    m_apply->setEnabled(true);
//     }
}



