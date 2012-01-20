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
    m_port->setMaximum(Server::MAX_NUMBER_OF_PORT);
    m_port->setValue(Server::DEFAULT_PORT);
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
    QHBoxLayout* llPassword = new QHBoxLayout();
    llPassword->addWidget(new QLabel(i18n("Password:"), groupLogin));
    m_password = new QLineEdit(groupLogin);
    m_password->setEchoMode(QLineEdit::Password);
    connect(m_password, SIGNAL(textChanged(QString)),
            this, SLOT(someChanged()));
    llPassword->addWidget(m_password);
    llLogin->addLayout(llPassword);

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
    config.writeEntry("password", m_password->text().toUtf8().toBase64());
    config.sync();
}

void SettingsServer::setDefaults()
{
    m_port->setValue(Server::DEFAULT_PORT);
    m_onStartRun->setChecked(false);
    m_username->setText(Server::DEFAULT_PASSWORD);
    m_password->setText(Server::DEFAULT_PASSWORD);
}

void SettingsServer::loadSettings()
{
    KConfigGroup config = KGlobal::config()->group("server");
    m_port->setValue(config.readEntry("port", 8080));
    m_onStartRun->setChecked(config.readEntry("run_server_on_start", false));
    m_username->setText(config.readEntry("username", Server::DEFAULT_PASSWORD));
    QString decodedpassword = Server::DEFAULT_PASSWORD;
    m_password->setText(QByteArray::fromBase64(config.readEntry("password", decodedpassword.toUtf8().toBase64())));
    m_changed = false;
}



