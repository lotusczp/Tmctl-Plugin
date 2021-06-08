#ifndef TMCTLPLUGIN_H
#define TMCTLPLUGIN_H

#include "tmctlplugin_global.h"

#include "LTransPluginFactory.h"

class TMCTLPLUGINSHARED_EXPORT TmctlPlugin : public LTransPluginFactory
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "Lobster.LTransPluginFactory" FILE "TmctlPlugin.json")
    Q_INTERFACES(LTransPluginFactory)

public:
    TmctlPlugin() {}
    ~TmctlPlugin() Q_DECL_OVERRIDE {}

    //! create the transmission instance
    virtual LTransmission* createTransInstance() Q_DECL_OVERRIDE;
};

#endif // TMCTLPLUGIN_H
