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

#include "settingsserver.h"

#include <KLocalizedString>
#include <KGlobal>
#include <KConfigGroup>
#include <KConfig>
#include <QDebug>
#include "../server/restserver.h"
#include <KPasswordDialog>
#include <knewpassworddialog.h>
#include <QMessageBox>

SettingsServer::SettingsServer(QWidget* parent, Qt::WindowFlags f): AbstractSettings(parent, f)
{
    initGUI();
    loadSettings();
}

SettingsServer::~SettingsServer()
{

}

void SettingsServer::initGUI()
{
    QVBoxLayout *central = new QVBoxLayout(this);

    QGroupBox *group = new QGroupBox(i18n("Server interface"), this);
    QVBoxLayout *llGeneral = new QVBoxLayout(group);
    central->addWidget(group);
//     group->setLayout(llGeneral);

    //PORT
    QHBoxLayout* llPort = new QHBoxLayout();
    llPort->addWidget(new QLabel(i18n("Port:"), group));

    m_port = new QSpinBox(group);
    m_port->setMinimum(1);
    m_port->setMaximum(RESTServer::MAX_NUMBER_OF_PORT);
    m_port->setValue(RESTServer::DEFAULT_PORT);
    connect(m_port, SIGNAL(valueChanged(int)),
            this, SLOT(someChanged()));
    llPort->addWidget(m_port);

    llGeneral->addLayout(llPort);

    m_onStartRun = new QCheckBox(i18n("Run on start"), group);
    connect(m_onStartRun, SIGNAL(stateChanged(int)),
            this, SLOT(someChanged()));
    llGeneral->addWidget(m_onStartRun);

    //LOGIN GROUP
    QGroupBox* groupLogin = new QGroupBox(i18n("Login"), this);
    QVBoxLayout* llLogin = new QVBoxLayout(groupLogin);
    groupLogin->setLayout(llLogin);
    central->addWidget(groupLogin);

    //Username
    QHBoxLayout* llUsername = new QHBoxLayout();
    llUsername->addWidget(new QLabel(i18n("Username:"), groupLogin));
    m_username = new QLineEdit(groupLogin);
    connect(m_username, SIGNAL(textChanged(QString)),
            this, SLOT(someChanged()));
    llUsername->addWidget(m_username);
    llLogin->addLayout(llUsername);
    //Password
    m_btnPassword = new QPushButton(i18n("Change password"), this);
    connect(m_btnPassword, SIGNAL(clicked(bool)),
            this, SLOT(onPasswordClicked(bool)));
    llLogin->addWidget(m_btnPassword);


    //SPACER
    llLogin->addSpacerItem(new QSpacerItem(40, 200));
}

void SettingsServer::applySettings()
{
    AbstractSettings::applySettings();
    KConfigGroup config = KGlobal::config()->group("server");
    config.writeEntry ( "port", m_port->value() );
    config.writeEntry("run_server_on_start", m_onStartRun->isChecked());
    config.writeEntry("username", m_username->text());
    config.writeEntry("password", m_passwordHashed);
    config.sync();
}

void SettingsServer::setDefaults()
{
    m_port->setValue(RESTServer::DEFAULT_PORT);
    m_onStartRun->setChecked(false);
    m_username->setText(QByteArray());
    m_passwordHashed.clear();
}

void SettingsServer::loadSettings()
{
    KConfigGroup config = KGlobal::config()->group("server");
    m_port->setValue(config.readEntry("port", 8080));
    m_onStartRun->setChecked(config.readEntry("run_server_on_start", false));
    m_username->setText(config.readEntry("username", QByteArray()));
    m_passwordHashed = config.readEntry("password", QByteArray());
    m_changed = false;
}


void SettingsServer::onPasswordClicked(bool clicked)
{
//     if (clicked) {
    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Information);
    if (!m_passwordHashed.isEmpty()) {
        KPasswordDialog dlg(this);
        dlg.setPrompt(i18n("Enter actual password"));
        if (dlg.exec()) {
            if (!ServerRequest::arePassesEquel(dlg.password().toLower().toUtf8(), m_passwordHashed)) {
                msgBox.setText("Password is incorect!");
                msgBox.exec();
                return;
            }
        } else {
            return;
        }
    }

    KNewPasswordDialog dlgNew(this);
    dlgNew.setPrompt( i18n( "Enter a password" ) );
    if (dlgNew.exec()) {
        m_passwordHashed = QCryptographicHash::hash(dlgNew.password().toLower().toUtf8(), QCryptographicHash::Md5);
        someChanged();
        msgBox.setText("Password was changed.");
        msgBox.exec();
    }
}


