#ifndef LTMCTLSETTINGSRELIER_H
#define LTMCTLSETTINGSRELIER_H

#include <QObject>
#include "LSettingsRelier.h"

class LTmctlSettingsRelier : public QObject, public LSettingsRelier
{
    Q_OBJECT
public:
    LTmctlSettingsRelier() {}
    virtual ~LTmctlSettingsRelier() Q_DECL_OVERRIDE {}

    virtual void parseFromSetting(LObixTreeIter a_obixIter) Q_DECL_OVERRIDE;

    virtual void convertToSetting() Q_DECL_OVERRIDE;

public slots:
    void receiveServerInfo(QString a_strServer, QString a_strPort);

signals:
    void sendServerInfo(QString a_strServer, QString a_strPort);

private:
};

#endif // LTMCTLSETTINGSRELIER_H
