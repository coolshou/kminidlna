#ifndef SETTINGSERVER_H
#define SETTINGSERVER_H

#include <QWidget>
#include "abstractsettings.h"

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
    QByteArray m_passwordHashed;

};

#endif // SETTINGSERVER_H
