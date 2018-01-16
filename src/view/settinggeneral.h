#ifndef SETTINGGENERAL_H
#define SETTINGGENERAL_H

#include <QWidget>
#include <QCheckBox>
#include "abstractsettings.h"

namespace Ui {
class SettingGeneral;
}

class SettingGeneral : public AbstractSettings
{
    Q_OBJECT

public:
    explicit SettingGeneral(QWidget *parent = 0, Qt::WindowFlags f = 0);
    ~SettingGeneral();
    void applySettings();
    void setDefaults();
    void loadSettings();
private:
    void initGUI();
    Ui::SettingGeneral *ui;
    //general
    QCheckBox* m_closeToTray;
    QCheckBox* m_runOnStart;
    QCheckBox* m_remRun;

};

#endif // SETTINGGENERAL_H
