#include "abstractsettings.h"

AbstractSettings::AbstractSettings(QWidget* parent, Qt::WindowFlags f): QWidget(parent, f), m_changed(false)
{

}

AbstractSettings::~AbstractSettings()
{

}

void AbstractSettings::applySettings()
{
    m_changed = false;
}


void AbstractSettings::someChanged()
{
    if (!m_changed) {
        m_changed = true;
        emit changed();
    }
}

