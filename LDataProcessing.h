#ifndef LDATAPROCESSING_H
#define LDATAPROCESSING_H

#include <QObject>
#include <QMutex>
#include "LCommon.h"
#include "LTmctlData.h"
#include "LUniqueResource.h"

class LDataProcessing : public QObject
{
    Q_OBJECT
public:
    LDataProcessing(LTmctlDataMap& a_rAllDataMap,
                    LTmctlDataMap& a_rCaredDataMap,
                    QMutex& a_rMutex);
    ~LDataProcessing();

    void setTmctlWrapper(LTmctlWrapper *a_pTmctlWrapper) {m_pTmctlWrapper = a_pTmctlWrapper;}

    void startProcessing(bool a_bStart);

    void setScanInterval(int a_iScanInterval) {m_iScanInterval = a_iScanInterval;}

    void setConnected(bool a_bIsConnected) {m_bIsConnected = a_bIsConnected;}

    void connectTarget(QString a_strIpAddress, QString a_strSubscribeMsg);

    void disconnectTarget();

    void setVariableKeys(const QList<QString>& a_rVarKeys) {m_listVarKeys = a_rVarKeys;}

signals:
    void sendMultiVar(LDataValueMap a_mapDataValue);

    void targetConnected();

public slots:
    void handleScan();

private:
    QList<QString>      m_listVarKeys;
    QMutex              &m_mutex;               //!< mutex for this sub-thread and main thread

    LTmctlDataMap       &m_mapAllData;          //!< All the data information in the config tree
    LTmctlDataMap       &m_mapCaredData;        //!< Map of user interested data source

    QTimer              *m_pTimer;
    int                 m_iScanInterval;
    bool                m_bIsConnected;
    int                 m_iId;
    LTmctlWrapper       *m_pTmctlWrapper;
};

#endif // LDATAPROCESSING_H
