#ifndef ABSTRACTSETTINGS_H
#define ABSTRACTSETTINGS_H
#include <QWidget>

class AbstractSettings: public QWidget
{
  Q_OBJECT
public:
    explicit AbstractSettings(QWidget* parent = 0, Qt::WindowFlags f = 0);
    virtual ~AbstractSettings();
    virtual void applySettings() = 0;
    virtual void setDefaults() = 0;
};

#endif // ABSTRACTSETTINGS_H