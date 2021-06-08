#ifndef LTMCTLTRANS_H
#define LTMCTLTRANS_H

#include <QThread>
#include <QMutex>

#include "LCommon.h"
#include "LTransmission.h"
#include "LUniqueResource.h"

#include "LDataProcessing.h"
#include "LTmctlData.h"
#include "LTmctlSettingsWidget.h"
#include "LTmctlSettingsRelier.h"
#include "LTmctlToolWidget.h"

class LTmctlTrans : public LTransmission
{
    Q_OBJECT
public:
    LTmctlTrans();
    ~LTmctlTrans() Q_DECL_OVERRIDE;

    //! Configurate the transmission, like port, baudrate, etc...
    virtual void configurate(const LObixObjIter &a_rConfigIter) Q_DECL_OVERRIDE;

    //! return false if some error occurs when parsing the configuration tree,
    //! else return true and set the a_rWholeDataList the whole data list included in the configuration tree
    virtual bool getDataList(LDataNameList &a_rDataNameList) const Q_DECL_OVERRIDE;

    //! Clear the current data map container and fill in new data
    //! The data is user cared variables.
    virtual void resetCaredDataList(const LDataNameList &a_rCaredDataNameList) Q_DECL_OVERRIDE;

    //! Start data processing and transmission
    virtual void start() Q_DECL_OVERRIDE;

    //! Stop data processing and transmission
    virtual void stop() Q_DECL_OVERRIDE;

    //! Set the prefix of channel name to all the cared data value uploaded
    //! So that the main program can distinguish datas with the same name from different channels but using the same transmission plugin
    virtual void setChannelPrefix(const QString &a_rPrefix) Q_DECL_OVERRIDE;

    //! Get the protocol name
    virtual const QString &getProtocolName() const Q_DECL_OVERRIDE;

    //! Get the SW version number
    virtual const QString &getSwVersion() const Q_DECL_OVERRIDE;

    //! widget for configuration of this plugin
    virtual QWidget* getSettingsWidget() Q_DECL_OVERRIDE;

    //! settings relier of this plugin
    virtual LSettingsRelier* getSettingsRelier() Q_DECL_OVERRIDE;

    //! tool widget of this plugin
    virtual QWidget* getToolWidget() Q_DECL_OVERRIDE;

    //! set value by lobster main program
    virtual void setDataValue(QString a_strDataName, LDataValue a_dataValue) Q_DECL_OVERRIDE;

    //! set the unique resource access interface
    virtual void setUniqueResource(const LUniqueResource& a_rUniqueResource) Q_DECL_OVERRIDE;

    //! Force to pull attributes from this plugin
    virtual void pullAttributes() Q_DECL_OVERRIDE;

    //! Force to pull value of a certain point from this plugin
    virtual void pullDataValue(QString a_strDataName) Q_DECL_OVERRIDE;


protected:
    //! Receive variables from data processing
    void receiveMultiVar(LDataValueMap a_mapDataValue);

    //! Receive device connected
    void receiveDeviceReady();

    void parseConfigTree(const LObixObjIter &a_rConfigIter);

    void handleTryConnect(QString a_strIpAddress);


protected:  // Protected const class data
    static const QString        s_strProtocolName;      //!< transmission name
    static const QString        s_strSwVersion;         //!< transmission plugin version number

    bool                        m_bErrorFlag;           //!< whether any wrong while parsing the config
    QString                     m_strPrefix;            //!< channel prefix

    QThread                     *m_pDataProcThread;     //!< Data processing sub-thread
    QMutex                      m_mutex;                //!< mutex for m_dataProcThread and main thread
    LDataProcessing             *m_pDataProc;           //!< Data processing slots object

    bool                        m_bErrorReported;       //!< Flag of whether the error is reported

    bool                        m_bIsDeviceReady;

    LTmctlDataMap               m_mapAllData;           //!< all the data information in the config tree
    LTmctlDataMap               m_mapCaredData;         //!< Map of user interested data source

    LTmctlSettingsWidget        *m_pSettingsWidget;     //!< settings widget
    LTmctlSettingsRelier        *m_pSettingsRelier;     //!< settings relier
    LTmctlToolWidget            *m_pToolWidget;         //!< tool widget
    QString                     m_strSubscribeMsg;
};

#endif // LTMCTLTRANS_H
