/*
http://github.com/coolshou/qminidlna

Copyright (C) 2018 jimmy

 */
#ifndef SETTINGCONFFILE_H
#define SETTINGCONFFILE_H

#include "abstractsettings.h"

#include <QListView>
#include <QStringListModel>
#include <QToolButton>
#include "../core/configurationfile.h"
#include "mediafolderswidget.h"

namespace Ui {
class SettingConfFile;
}

class SettingConfFile : public AbstractSettings
{
    Q_OBJECT

public:
    explicit SettingConfFile(QWidget *parent = 0, Qt::WindowFlags f = 0);
    ~SettingConfFile();
    void applySettings();
    void setDefaults();
    void loadSettings();

    public slots:
        void onAddButtonClicked();
        void onRemoveButtonClicked();
        void onConfigFileChanged();

private:
    void initGUI();
    void loadModel();
    QStringListModel* m_albumArtNamesModel;
    ConfigurationFile* m_actualConfFile;
    Ui::SettingConfFile *ui;
    QListView* m_albumArtNamesView;
    QStringList m_albumArtNamesList;
    QWidget* m_albumArtNamesControllWidget;
    QToolButton* m_add;
    QToolButton* m_remove;
    MediaFoldersWidget* m_mediaFoldersWidget;

};

#endif // SETTINGCONFFILE_H
