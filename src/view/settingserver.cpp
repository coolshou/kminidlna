#include "settingserver.h"
#include "ui_settingserver.h"
#include "generatecertificatedialog.h"
#include "../server/restserver.h"
#include "../server/certificategenerator.h"

#include <QMessageBox>
#include <QFileDialog>
#include <QStandardPaths>
#include <QSettings>

SettingServer::SettingServer(QWidget *parent, Qt::WindowFlags f ) :
    AbstractSettings(parent, f),
    ui(new Ui::SettingServer)
{
    ui->setupUi(this);
    initGUI();
    loadSettings();
}

SettingServer::~SettingServer()
{
    delete ui;
}
void SettingServer::initGUI()
{
    connect(ui->m_port, SIGNAL(valueChanged(int)), this, SLOT(someChanged()));
    connect(ui->m_onStartRun, SIGNAL(stateChanged(int)), this, SLOT(someChanged()));
    //LOGIN GROUP
    //Username
    connect(ui->m_username, SIGNAL(textChanged(QString)), this, SLOT(someChanged()));
    //Password
    connect(ui->m_btnPassword, SIGNAL(clicked(bool)), this, SLOT(onPasswordClicked(bool)));

    //Certificate
    connect(ui->m_rbtGenerated, SIGNAL(toggled(bool)), SLOT(certificateChanged(bool)));
    connect(ui->m_rbtCustom, SIGNAL(toggled(bool)), SLOT(certificateChanged(bool)));
    connect(ui->m_leCustomPKey, SIGNAL(textChanged(QString)), SLOT(someChanged()));
    connect(ui->tbtPKey, SIGNAL(clicked(bool)),  SLOT(onTbtPKeyClicked(bool)));
    connect(ui->m_leCustomCertificate, SIGNAL(textChanged(QString)), SLOT(someChanged()));
    connect(ui->tbtCert, SIGNAL(clicked(bool)), SLOT(onTbtCertClicked(bool)));
    connect(ui->btnGenerateCertificate, SIGNAL(clicked(bool)), SLOT(onGenerateCertificateClicked(bool)));

}

void SettingServer::applySettings()
{
    AbstractSettings::applySettings();
    QSettings config;
    config.beginGroup("server");
    config.setValue("port", ui->m_port->value());
    config.setValue("run_server_on_start", ui->m_onStartRun->isChecked());
    config.setValue("username", ui->m_username->text());
    config.setValue("password", m_passwordHashed);
    config.setValue("custom_cert", ui->m_rbtCustom->isChecked());
    config.setValue("pkey_path", ui->m_leCustomPKey->text());
    config.setValue("cert_path", ui->m_leCustomCertificate->text());
    config.sync();
    config.endGroup();
}

void SettingServer::setDefaults()
{
    ui->m_port->setValue(RESTServer::DEFAULT_PORT);
    ui->m_onStartRun->setChecked(false);
    ui->m_username->setText(QByteArray());
    m_passwordHashed.clear();
}

void SettingServer::loadSettings()
{
    QSettings config;
    config.beginGroup("server");
    ui->m_port->setValue(config.value("port", 8080).toInt());
    ui->m_onStartRun->setChecked(config.value("run_server_on_start", false).toBool());
    ui->m_username->setText(config.value("username", QByteArray()).toByteArray());
    m_passwordHashed = config.value("password", QByteArray()).toByteArray();
    bool custom = config.value("custom_cert", false).toBool();

    if(custom){
      ui->m_customCertificate->setEnabled(true);
      ui->m_rbtCustom->setChecked(true);
    }
    ui->m_leCustomPKey->setText(config.value("pkey_path", QString()).toString());
    ui->m_leCustomCertificate->setText(config.value("cert_path", QString()).toString());
    config.endGroup();

    m_changed = false;
}


void SettingServer::onPasswordClicked(bool )
{
//     if (clicked) {
    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Information);
    if(!m_passwordHashed.isEmpty()) {
        //TODO PasswordDialog
        /*
        KPasswordDialog dlg(this);
        dlg.setPrompt(tr("Enter actual password"));
        if(dlg.exec()) {
            if(!ServerRequest::arePassesEquel(dlg.password().toLower().toUtf8(), m_passwordHashed)) {
                msgBox.setText("Password is incorect!");
                msgBox.exec();
                return;
            }
        } else {
            return;
        }
        */
    }
    /*TODO KNewPasswordDialog
    KNewPasswordDialog dlgNew(this);
    dlgNew.setPrompt(tr("Enter a password"));
    if(dlgNew.exec()) {
        m_passwordHashed = QCryptographicHash::hash(dlgNew.password().toLower().toUtf8(), QCryptographicHash::Md5);
        someChanged();
        msgBox.setText("Password was changed.");
        msgBox.exec();
    }
    */
}

void SettingServer::certificateChanged(bool)
{
    if(ui->m_rbtCustom->isChecked()) {
        ui->m_customCertificate->setEnabled(true);
    } else {
        ui->m_customCertificate->setEnabled(false);
    }
    someChanged();
}

void SettingServer::onTbtPKeyClicked(bool)
{
    QString path = QFileDialog::getOpenFileName(this,tr("Private key file"),ui->m_leCustomPKey->text(),"All (*.*)");
    if(!path.isEmpty()) {
        ui->m_leCustomPKey->setText(path);
    }
}

void SettingServer::onTbtCertClicked(bool)
{
    QString path = QFileDialog::getOpenFileName(this,tr("Certificate X509 file"),ui->m_leCustomCertificate->text(),"All (*.*)");
    if(!path.isEmpty()) {
        ui->m_leCustomCertificate->setText(path);
    }
}


void SettingServer::onGenerateCertificateClicked(bool)
{
    GenerateCertificateDialog* dlg = new GenerateCertificateDialog(this);
    if(dlg->exec()) {
        X509Value value;
        value.commonName = (unsigned char*) dlg->commonName().toLatin1().data();
        value.countryName = (unsigned char*) dlg->country().toLatin1().data();
        QString dirs = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation);
        CertificateGenerator::CreatedCertificteError err = CertificateGenerator::createCertificate(value, dirs + "/qminidlna/");
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
