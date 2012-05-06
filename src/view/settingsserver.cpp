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
#include "generatecertificatedialog.h"

#include <KLocalizedString>
#include <KGlobal>
#include <KConfigGroup>
#include <KConfig>
#include <QDebug>
#include "../server/restserver.h"
#include "../server/certificategenerator.h"
#include <KPasswordDialog>
#include <KMessageBox>
#include <KStandardDirs>
#include <knewpassworddialog.h>
#include <QMessageBox>
#include <QToolButton>
#include <KFileDialog>

SettingsServer::SettingsServer(QWidget* parent, Qt::WindowFlags f): AbstractSettings(parent, f) {
    initGUI();
    loadSettings();
}

SettingsServer::~SettingsServer() {

}

void SettingsServer::initGUI() {
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

    //Certificate
    QGroupBox* groupCertificate = new QGroupBox(i18n("SSL Certificate"), this);
    QVBoxLayout* llCertificate = new QVBoxLayout();

    m_rbtGenerated = new QRadioButton(i18n("Use default generated certificate"), this);
    m_rbtGenerated->setChecked(true);
    connect(m_rbtGenerated, SIGNAL(toggled(bool)),
            SLOT(certificateChanged(bool)));
    llCertificate->addWidget(m_rbtGenerated);

    m_rbtCustom = new QRadioButton(i18n("Use custom certificate"), this);
    connect(m_rbtCustom, SIGNAL(toggled(bool)), SLOT(certificateChanged(bool)));
    llCertificate->addWidget(m_rbtCustom);

    m_customCertificate = new QWidget(this);
    m_customCertificate->setEnabled(false);
    QGridLayout* glCustomCert = new QGridLayout(m_customCertificate);
    llCertificate->addWidget(m_customCertificate);

    glCustomCert->addWidget(new QLabel("Private key path:", m_customCertificate), 0, 0);
    m_leCustomPKey = new QLineEdit(m_customCertificate);
    connect(m_leCustomPKey, SIGNAL(textChanged(QString)),
            SLOT(someChanged()));
    glCustomCert->addWidget(m_leCustomPKey, 0, 1);
    QToolButton* tbtPKey =  new QToolButton(m_customCertificate);
    tbtPKey->setIcon(KIcon("document-open"));
    connect(tbtPKey, SIGNAL(clicked(bool)),
            SLOT(onTbtPKeyClicked(bool)));
    glCustomCert->addWidget(tbtPKey, 0, 2);

    glCustomCert->addWidget(new QLabel("Certificate path:", m_customCertificate), 1, 0);
    m_leCustomCertificate = new QLineEdit(m_customCertificate);
    connect(m_leCustomCertificate, SIGNAL(textChanged(QString)),
            SLOT(someChanged()));
    glCustomCert->addWidget(m_leCustomCertificate, 1, 1);
    QToolButton* tbtCert =  new QToolButton(m_customCertificate);
    tbtCert->setIcon(KIcon("document-open"));
    connect(tbtCert, SIGNAL(clicked(bool)),
            SLOT(onTbtCertClicked(bool)));
    glCustomCert->addWidget(tbtCert, 1, 2);

    groupCertificate->setLayout(llCertificate);
    central->addWidget(groupCertificate);


    QPushButton* btnGenerateCertificate = new QPushButton(i18n("Generate certificate"), groupCertificate);
    connect(btnGenerateCertificate, SIGNAL(clicked(bool)), SLOT(onGenerateCertificateClicked(bool)));
    llCertificate->addWidget(btnGenerateCertificate);

    //SPACER
    llCertificate->addSpacerItem(new QSpacerItem(40, 200));
}

void SettingsServer::applySettings() {
    AbstractSettings::applySettings();
    KConfigGroup config = KGlobal::config()->group("server");
    config.writeEntry("port", m_port->value());
    config.writeEntry("run_server_on_start", m_onStartRun->isChecked());
    config.writeEntry("username", m_username->text());
    config.writeEntry("password", m_passwordHashed);
    config.writeEntry("custom_cert", m_rbtCustom->isChecked());
    config.writeEntry("pkey_path", m_leCustomPKey->text());
    config.writeEntry("cert_path", m_leCustomCertificate->text());
    config.sync();
}

void SettingsServer::setDefaults() {
    m_port->setValue(RESTServer::DEFAULT_PORT);
    m_onStartRun->setChecked(false);
    m_username->setText(QByteArray());
    m_passwordHashed.clear();
}

void SettingsServer::loadSettings() {
    KConfigGroup config = KGlobal::config()->group("server");
    m_port->setValue(config.readEntry("port", 8080));
    m_onStartRun->setChecked(config.readEntry("run_server_on_start", false));
    m_username->setText(config.readEntry("username", QByteArray()));
    m_passwordHashed = config.readEntry("password", QByteArray());
    bool custom = config.readEntry("custom_cert", false);
    if(custom){
      m_customCertificate->setEnabled(true);
      m_rbtCustom->setChecked(true);
    }
    m_leCustomPKey->setText(config.readEntry("pkey_path", QString()));
    m_leCustomCertificate->setText(config.readEntry("cert_path", QString()));
    m_changed = false;
}


void SettingsServer::onPasswordClicked(bool ) {
//     if (clicked) {
    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Information);
    if(!m_passwordHashed.isEmpty()) {
        KPasswordDialog dlg(this);
        dlg.setPrompt(i18n("Enter actual password"));
        if(dlg.exec()) {
            if(!ServerRequest::arePassesEquel(dlg.password().toLower().toUtf8(), m_passwordHashed)) {
                msgBox.setText("Password is incorect!");
                msgBox.exec();
                return;
            }
        } else {
            return;
        }
    }

    KNewPasswordDialog dlgNew(this);
    dlgNew.setPrompt(i18n("Enter a password"));
    if(dlgNew.exec()) {
        m_passwordHashed = QCryptographicHash::hash(dlgNew.password().toLower().toUtf8(), QCryptographicHash::Md5);
        someChanged();
        msgBox.setText("Password was changed.");
        msgBox.exec();
    }
}

void SettingsServer::certificateChanged(bool) {
    if(m_rbtCustom->isChecked()) {
        m_customCertificate->setEnabled(true);
    } else {
        m_customCertificate->setEnabled(false);
    }
    someChanged();
}

void SettingsServer::onTbtPKeyClicked(bool) {
    QString path = KFileDialog::getOpenFileName(KUrl(m_leCustomPKey->text()), "", this, i18n("Private key file") + "/");
    if(!path.isEmpty()) {
        m_leCustomPKey->setText(path);
    }
}

void SettingsServer::onTbtCertClicked(bool) {
    QString path = KFileDialog::getOpenFileName(KUrl(m_leCustomCertificate->text()), "", this, i18n("Certificate X509 file") + "/");
    if(!path.isEmpty()) {
        m_leCustomCertificate->setText(path);
    }
}


void SettingsServer::onGenerateCertificateClicked(bool) {
    GenerateCertificateDialog* dlg = new GenerateCertificateDialog(this);
    if(dlg->exec()) {
        X509Value value;
        value.commonName = (unsigned char*) dlg->commonName().toAscii().data();
        value.countryName = (unsigned char*) dlg->country().toAscii().data();
        KStandardDirs dirs;
        CertificateGenerator::CreatedCertificteError err = CertificateGenerator::createCertificate(value, dirs.saveLocation("data") + "kminidlna/");
        QMessageBox msgBox;

        switch(err) {
        case CertificateGenerator::NoError:
            msgBox.setIcon(QMessageBox::Information);
            msgBox.setText("Certificate was generated");
	    someChanged();
            break;
        case CertificateGenerator::FileError:
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.setText("Certificate was not generated. File create problem.");
            break;
        case CertificateGenerator::GenerateError:
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.setText("Certificate was generated. ");
            break;
        }
        msgBox.exec();
    }
}

