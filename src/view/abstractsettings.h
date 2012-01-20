#ifndef ABSTRACTSETTINGS_H
#define ABSTRACTSETTINGS_H
#include <QWidget>

class AbstractSettings: public QWidget
{
    Q_OBJECT
public:
    AbstractSettings(QWidget* parent = 0, Qt::WindowFlags f = 0);
    virtual ~AbstractSettings();
    virtual void applySettings();
    virtual void setDefaults() = 0;
    virtual void loadSettings() = 0;
    inline bool isChanged() {
        return m_changed;
    };
public slots:
    void someChanged();
protected:
    bool m_changed;
signals:
    void changed();
};

#endif // ABSTRACTSETTINGS_H
