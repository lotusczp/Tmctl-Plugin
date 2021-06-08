#include "LDataProcessing.h"
#include <QDebug>

LDataProcessing::LDataProcessing(LTmctlDataMap &a_rAllDataMap, LTmctlDataMap &a_rCaredDataMap, QMutex &a_rMutex) :
    m_mutex(a_rMutex),
    m_mapAllData(a_rAllDataMap),
    m_mapCaredData(a_rCaredDataMap)
{
    m_pTimer = new QTimer;
    connect(m_pTimer, &QTimer::timeout, this, &LDataProcessing::handleScan);

    m_iScanInterval = 200;
    m_bIsConnected = false;
    m_iId = -1;
    m_pTmctlWrapper = nullptr;
}

LDataProcessing::~LDataProcessing()
{
    delete m_pTimer, m_pTimer = nullptr;
}

void LDataProcessing::startProcessing(bool a_bStart)
{
    if(a_bStart) {
        m_pTimer->start(m_iScanInterval);
    }
    else {
        m_pTimer->stop();
    }
}

void LDataProcessing::connectTarget(QString a_strIpAddress, QString a_strSubscribeMsg)
{
    if(!m_bIsConnected && m_pTmctlWrapper != nullptr) {
        // TM_CTL_VXI11 = 8
        int iInitResult = m_pTmctlWrapper->initialize(8, a_strIpAddress.toLatin1().data(), &m_iId);
        if(iInitResult == 0) {
            #if 1
            //strSubscribeMsg = ":NUMERIC:NORMAL:NUMBER 12;:NUMERIC:NORMAL:ITEM1 SPE;ITEM2 TORQ;ITEM3 U,2,TOT;ITEM4 I,2,TOT;ITEM5 P,SIGMA,TOT;ITEM6 PM;ITEM7 ETA2;ITEM8 ETA1;ITEM9 P,1,TOT;ITEM10 U,1,TOT;ITEM11 I,1,TOT;ITEM12 ETA3";
            //strSubscribeMsg = ":NUMERIC:NORMAL:NUMBER 12;:NUMERIC:NORMAL:ITEM1 SPE;ITEM2 TORQ;ITEM3 U,2,TOT;ITEM4 I,2,TOT;ITEM5 P,SIGMA,TOT;ITEM6 PM;ITEM7 ETA2;ITEM8 ETA1;ITEM9 P,1,TOT;ITEM10 UDC;ITEM11 IDC;ITEM12 ETA3";
            int iSendResult = m_pTmctlWrapper->send(m_iId, a_strSubscribeMsg.toLatin1().data());
            if (iSendResult == 0) {
                QString strFormatMsg = ":NUMERIC:FORMAT FLOAT";
                int iFormatResult = m_pTmctlWrapper->send(m_iId, strFormatMsg.toLatin1().data());
                if(iFormatResult == 0) {
                    m_bIsConnected = true;
                    emit targetConnected();
                }
            }
        #endif
        }
    }
}

void LDataProcessing::disconnectTarget()
{
    // No communication with device?
    m_bIsConnected = false;
}

void LDataProcessing::handleScan()
{
    if(m_bIsConnected) {
        QString msg;
        int rtn=0;
        msg = ":NUMERIC:NORMAL:VALUE?";
        rtn = m_pTmctlWrapper->sendByLength(m_iId, msg.toLatin1().data(), msg.length());
        if (rtn != 0) {
            return;
        };

        char* buf;
        int maxLength;
        int realLength;
        //----------------------#receive values by Float#
        //###FLOAT:TmcReceiveBlock()###
        rtn = m_pTmctlWrapper->receiveBlockHeader(m_iId, &maxLength);
        if(maxLength < 1) {
            return;
        }
        maxLength ++;//see tmctl's help
        if ((buf = new char[maxLength]) == NULL) {
            return;
        }

        int isEnd = 0;
        char bytes[sizeof(float)];
        float floatBuf;
        while (isEnd == 0) {
            memset(buf, 0, maxLength);
            rtn = m_pTmctlWrapper->receiveBlockData(m_iId, buf, maxLength, &realLength, &isEnd);
            if (rtn != 0) {
                delete[] buf;
                return;
            }
            LDataValueMap valueMap;
            //converse the char format and copy as float.
            for (int n=0; n<realLength/4; n++) {
                bytes[3] = buf[n * 4];
                bytes[2] = buf[n * 4 + 1];
                bytes[1] = buf[n * 4 + 2];
                bytes[0] = buf[n * 4 + 3];
                memcpy(&floatBuf, bytes, sizeof(float));
                //! \todo   So rude! Need check the invalid number
                if(floatBuf < 100000000.0){ // Error value
                    if(n < m_listVarKeys.size()) {
                        LDataValue dataValue;
                        dataValue.set(floatBuf);
                        valueMap[m_listVarKeys[n]] = dataValue;
                    }
                }
            }
            emit sendMultiVar(valueMap);
        }
        delete[] buf;
    }
}
