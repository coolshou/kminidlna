#ifndef SETTINGSERVER_H
#define SETTINGSERVER_H

#include <QWidget>
#include "abstractsettings.h"
#include <QSpinBox>
#include <QLineEdit>
#include <QPushButton>
#include <QCheckBox>
#include <QRadioButton>

namespace Ui {
class SettingServer;
}

class SettingServer : public AbstractSettings
{
    Q_OBJECT

public:
    explicit SettingServer(QWidget *parent = 0, Qt::WindowFlags f = 0);
    ~SettingServer();
    void applySettings();
    void setDefaults();
    void loadSettings();

public slots:
    void onPasswordClicked(bool);
    void onGenerateCertificateClicked(bool);
    void certificateChanged(bool);
    void onTbtPKeyClicked(bool);
    void onTbtCertClicked(bool);

private:
    void initGUI();
    Ui::SettingServer *ui;
    QSpinBox* m_port;
    QLineEdit* m_username;
    QByteArray m_passwordHashed;
    QPushButton* m_btnControll;
    QCheckBox* m_onStartRun;
    QPushButton* m_btnPassword;
    QRadioButton* m_rbtGenerated;
    QRadioButton* m_rbtCustom;
    QWidget* m_customCertificate;
    QLineEdit* m_leCustomPKey;
    QLineEdit* m_leCustomCertificate;

};

#endif // SETTINGSERVER_H
