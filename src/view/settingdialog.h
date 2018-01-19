#ifndef SETTINGDIALOG_H
#define SETTINGDIALOG_H

#include <QDialog>
#include <QCheckBox>
#include "settinggeneral.h"
#include "settingminidlna.h"
#include "settingmediafolder.h"
#include "settingconffile.h"
#include "settingserver.h"

namespace Ui {
class SettingDialog;
}

class SettingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingDialog(QWidget *parent = 0, Qt::WindowFlags flags = 0);
    ~SettingDialog();

public slots:
    void onChange();

private slots:
    void onApply();
    void onDefault();

private:
    bool m_wasNo;
    Ui::SettingDialog *ui;
    SettingGeneral* m_general;
    SettingMiniDLNA* m_minidlna;
    SettingMediaFolder* m_mediaFolders;
    SettingConfFile* m_confFileSettings;
    SettingServer* m_server;
    QPushButton*  m_apply;


    void initGUI();
signals:
    void settingsChanged();

};

#endif // SETTINGDIALOG_H
