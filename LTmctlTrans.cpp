#include "LTmctlTrans.h"

// Static class data
const QString LTmctlTrans::s_strProtocolName = "Tmctl";
const QString LTmctlTrans::s_strSwVersion = "1.0";

/// \note don't use the reference as the iterator argument in case the iterator changed unexpectedly inside this function
static bool parsePoint(LObixObjIter a_rConfigIter, LTmctlData::Properties &a_rProperties);

LTmctlTrans::LTmctlTrans()
{
    m_bIsDeviceReady = false;

    m_pDataProc = new LDataProcessing(m_mapAllData, m_mapCaredData, m_mutex);
    m_pDataProcThread = new QThread;
    /// \note This will move ALL the slots of LDataProcessing processed in the sub-thread
    m_pDataProc->moveToThread(m_pDataProcThread);

    m_pSettingsWidget = new LTmctlSettingsWidget;

    m_pSettingsRelier = new LTmctlSettingsRelier;

    m_pToolWidget = new LTmctlToolWidget;

    // Connect processing slots
    connect(m_pDataProc, &LDataProcessing::sendMultiVar, this, &LTmctlTrans::receiveMultiVar);
    connect(m_pDataProc, &LDataProcessing::targetConnected, this, &LTmctlTrans::receiveDeviceReady);
    connect(m_pDataProc, &LDataProcessing::targetConnected, m_pSettingsWidget, &LTmctlSettingsWidget::receiveDeviceConnected);
    connect(m_pSettingsWidget, &LTmctlSettingsWidget::tryConnect, this, &LTmctlTrans::handleTryConnect);
    connect(m_pSettingsWidget, &LTmctlSettingsWidget::tryDisconnect, m_pDataProc, &LDataProcessing::disconnectTarget);
    connect(m_pSettingsWidget, &LTmctlSettingsWidget::sendScanInterval, m_pDataProc, &LDataProcessing::setScanInterval);

    // Start data processing thread
    m_pDataProcThread->start();
}

LTmctlTrans::~LTmctlTrans()
{
    // Make sure the thread quits
    m_pDataProcThread->quit();
    m_pDataProcThread->wait();
    delete m_pDataProcThread;

    // we only need delete this map, other pointer containers all point to memory managed by this one
    qDeleteAll(m_mapAllData);

    m_mapAllData.clear();

    delete m_pDataProc;

    if(m_pSettingsWidget != NULL) {
        delete m_pSettingsWidget, m_pSettingsWidget = NULL;
    }
    if(m_pSettingsRelier != NULL) {
        delete m_pSettingsRelier, m_pSettingsRelier = NULL;
    }
    if (m_pToolWidget != NULL) {
        delete m_pToolWidget, m_pToolWidget = NULL;
    }
}

void LTmctlTrans::configurate(const LObixObjIter &a_rConfigIter)
{
    // Clear old stuff
    qDeleteAll(m_mapAllData);
    m_mapAllData.clear();

    // Parse the configuration file and add points
    m_bErrorFlag = false;
    parseConfigTree(a_rConfigIter);
}

bool LTmctlTrans::getDataList(LDataNameList &a_rDataNameList) const
{
    if(m_bErrorFlag) {
        return false;
    }
    else {
        a_rDataNameList = m_mapAllData.keys();
        m_pDataProc->setVariableKeys(a_rDataNameList);
        return true;
    }
}

void LTmctlTrans::resetCaredDataList(const LDataNameList &a_rCaredDataNameList)
{
    m_mutex.lock();

    // clear the old list
    m_mapCaredData.clear();

    // build a new one
    LDataNameListIter i(a_rCaredDataNameList);
    QString strName;
    while (i.hasNext()) {
        strName = i.next();
        if(m_mapAllData.contains(strName)) {
            m_mapCaredData[strName] = m_mapAllData[strName];
        }
    }

    m_mutex.unlock();
}

void LTmctlTrans::start()
{
    if(m_bIsDeviceReady) {
        m_pDataProc->startProcessing(true);

    }
}

void LTmctlTrans::stop()
{
    m_pDataProc->startProcessing(false);

}

void LTmctlTrans::setChannelPrefix(const QString &a_rPrefix)
{
    m_strPrefix = a_rPrefix;
}

const QString &LTmctlTrans::getProtocolName() const
{
    return s_strProtocolName;
}

const QString &LTmctlTrans::getSwVersion() const
{
    return s_strSwVersion;
}

QWidget *LTmctlTrans::getSettingsWidget()
{
    return m_pSettingsWidget;
}

LSettingsRelier *LTmctlTrans::getSettingsRelier()
{
    return m_pSettingsRelier;
}

QWidget *LTmctlTrans::getToolWidget()
{
    return m_pToolWidget;
}

void LTmctlTrans::setDataValue(QString a_strDataName, LDataValue a_dataValue)
{
    // Do nothing
}

void LTmctlTrans::setUniqueResource(const LUniqueResource &a_rUniqueResource)
{
    m_pDataProc->setTmctlWrapper(a_rUniqueResource.pTmctl);
}

void LTmctlTrans::pullAttributes()
{
    LAttributesMap mapAttri;

    QMapIterator<QString, LTmctlData*> it(m_mapCaredData);
    while(it.hasNext()) {
        it.next();
        QString strName = it.key();
        // This point is in config file
        LAttributes attri;
        attri.setUnit(it.value()->getProperties().unit);
        attri.setWritable(false);
        mapAttri.insert(m_strPrefix+"_"+strName, attri);
    }
    // Send map
    emit notifyVarAttributes(mapAttri);
}

void LTmctlTrans::pullDataValue(QString a_strDataName)
{
    // Do nothing
}

void LTmctlTrans::receiveMultiVar(LDataValueMap a_mapDataValue)
{
    LDataValueMap valueMap;
    QMapIterator<QString, LDataValue> it(a_mapDataValue);
    while(it.hasNext()) {
        it.next();
        valueMap.insert(m_strPrefix+"_"+it.key(), it.value());
    }

    if(!valueMap.isEmpty()) {
        emit notifyMultiVar(valueMap);
    }
}

void LTmctlTrans::receiveDeviceReady()
{
    m_bIsDeviceReady = true;
}

void LTmctlTrans::parseConfigTree(const LObixObjIter &a_rConfigIter)
{
    LObixObjIter configIter = a_rConfigIter;
    if ((configIter.getValue().getType() == eObj) && (configIter.getValue().getProperty("is") == "Tmctl Plugin Config")) {
        configIter.moveToFirstChild();
    }
    else {
        m_bErrorFlag = true;
        return;
    }

    // Parse Subscribe string
    if((configIter.getValue().getType() == eStr) && (configIter.getValue().getProperty("name") == "Subscribe")) {
        m_strSubscribeMsg = configIter.getValue().getProperty("val");
    }
    else {
        m_bErrorFlag = true;
        return;
    }

    configIter.moveToNextSibling();

    // Parse points
    if((configIter.getValue().getType() == eList) && (configIter.getValue().getProperty("is") == "Points")) {
        configIter.moveToFirstChild();
        bool bQuit = false;
        do {
            LTmctlData::Properties properties;
            if(parsePoint(configIter, properties) ) {
                if(!m_mapAllData.contains(properties.name) ) {
                    m_mapAllData[properties.name] = new LTmctlData(properties);
                }
            }
            if(configIter.hasSibling()) {
                configIter.moveToNextSibling();
            }
            else {
                bQuit = true;
            }
        }while(!bQuit);
        configIter.moveToParent();
    }
    else {
        m_bErrorFlag = true;
        return;
    }
}

void LTmctlTrans::handleTryConnect(QString a_strIpAddress)
{
    m_pDataProc->connectTarget(a_strIpAddress, m_strSubscribeMsg);
}

bool parsePoint(LObixObjIter a_rConfigIter, LTmctlData::Properties &a_rProperties)
{
    LTmctlData::Properties properties;
    if((a_rConfigIter.getValue().getType() == eObj) && (a_rConfigIter.getValue().getProperty("is") == "Point")) {
        a_rConfigIter.moveToFirstChild();
        if((a_rConfigIter.getValue().getType() == eStr) && (a_rConfigIter.getValue().getProperty("name") == "Name")) {
            properties.name = a_rConfigIter.getValue().getProperty("val");
            a_rConfigIter.moveToNextSibling();
        }
        else return false;

        if((a_rConfigIter.getValue().getType() == eStr) && (a_rConfigIter.getValue().getProperty("name") == "Unit")) {
            properties.unit = a_rConfigIter.getValue().getProperty("val");
            a_rConfigIter.moveToNextSibling();
        }
        else return false;

        // Parse success
        a_rProperties = properties;
        return true;
    }

    return false;
}

