#ifndef LTMCTLDATA_H
#define LTMCTLDATA_H

#include <QString>
#include <QList>
#include <QMap>

class LTmctlData
{
public:
    struct Properties {
        // For simple access, variables are all public
        QString         unit;
        QString         name;

        Properties() : unit("NA") {}
    };

public:
    LTmctlData(const Properties& a_rProperties)
    {
        m_properties = a_rProperties;
    }

    const Properties& getProperties() const {return m_properties;}

private:
    Properties      m_properties;

};

typedef QList<LTmctlData*>                      LTmctlDataArray;
typedef QListIterator<LTmctlData*>              LTmctlDataArrayIter;
typedef QMap<QString, LTmctlData*>              LTmctlDataMap;
typedef QMapIterator<QString, LTmctlData*>      LTmctlDataMapIter;

#endif // LTMCTLDATA_H
