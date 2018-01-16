#ifndef SETTINGMEDIAFOLDER_H
#define SETTINGMEDIAFOLDER_H

#include <QWidget>
#include "abstractsettings.h"
#include "mediafolderswidget.h"

namespace Ui {
class SettingMediaFolder;
}

class SettingMediaFolder : public AbstractSettings
{
    Q_OBJECT

public:
    explicit SettingMediaFolder(QWidget *parent = 0, Qt::WindowFlags f = 0);
    ~SettingMediaFolder();
    virtual void loadSettings();
    virtual void setDefaults();
    virtual void applySettings();

private:
    void initGUI();
    Ui::SettingMediaFolder *ui;
    MediaFoldersWidget* m_mediaFoldersWidget;
};

#endif // SETTINGMEDIAFOLDER_H
