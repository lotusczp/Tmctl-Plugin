#include "TmctlPlugin.h"


#include "LTmctlTrans.h"

LTransmission *TmctlPlugin::createTransInstance()
{
    return new LTmctlTrans;
}
