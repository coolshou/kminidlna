#ifndef SETTINGMINIDLNA_H
#define SETTINGMINIDLNA_H

#include <QWidget>
#include <QLineEdit>
#include <QLabel>
#include <QCheckBox>
#include <QSpinBox>
#include <QRadioButton>
#include "abstractsettings.h"
#include "../core/configurationfile.h"

namespace Ui {
class SettingMiniDLNA;
}

class SettingMiniDLNA : public AbstractSettings
{
    Q_OBJECT

public:
    explicit SettingMiniDLNA(QWidget *parent = 0, Qt::WindowFlags f = 0);
    ~SettingMiniDLNA();
    void applySettings();
    void setDefaults();
    void loadSettings();
public slots:
    void onPidBrowsePath();
    void onToogledUserConfPath(bool toogled);

private:
    void initGUI();
    Ui::SettingMiniDLNA *ui;
    QLineEdit* m_minidlnaPath;
    QToolButton* m_browsePath;
    QLineEdit* m_pidFilePath;
    QToolButton* m_pidbrowsePath;
    QLineEdit* m_confFilePath;
    QToolButton* m_browseConfFile;
    QLabel* m_lblConfFile;
    QCheckBox* m_loadFile;
    QSpinBox* m_port;
    QRadioButton* m_radioGlobalConfPath;
    QRadioButton* m_radioDefaultConfPath;
    QRadioButton* m_radioUserConfPath;
    ConfigurationFile::ConfFile usedConfFile();
    void setUsedConfFile(ConfigurationFile::ConfFile use);
    bool m_fileOptionsChanged;
    static const int DEFAULTPORT = 8200;
    private slots:
        void onBrowsePath();
        void onBrowseConfFile();
	void fileOptionsChanged();

};

#endif // SETTINGMINIDLNA_H
