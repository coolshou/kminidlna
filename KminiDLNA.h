#ifndef KminiDLNA_H
#define KminiDLNA_H

#include <KMainWindow>
#include <KSystemTrayIcon>

class KminiDLNA : public KMainWindow
{
Q_OBJECT
public:
    KminiDLNA();
    virtual ~KminiDLNA();
private:
    void initGUI();
    KSystemTrayIcon *systemtray; 
};

#endif // KminiDLNA_H
