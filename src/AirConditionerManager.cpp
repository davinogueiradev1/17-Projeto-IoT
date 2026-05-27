#include "AirConditionerManager.h"

#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <ir_Fujitsu.h>

#include "DebugManager.h"

//============================

IRFujitsuAC ar[4] =
{
    IRFujitsuAC(16),
    IRFujitsuAC(17),
    IRFujitsuAC(18),
    IRFujitsuAC(19)
};

//============================

void configurarArCondicionado()
{
    for(int i = 0; i < 4; i++)
    {
        ar[i].begin();
        ar[i].setModel(ARRAH2E);
    }

    debugInfo("Ar condicionado pronto");
}

//============================

int modoMap(int modo)
{
    switch(modo)
    {
        case 1: return kFujitsuAcModeAuto;
        case 2: return kFujitsuAcModeCool;
        case 3: return kFujitsuAcModeDry;
        case 4: return kFujitsuAcModeFan;
        default: return kFujitsuAcModeCool;
    }
}

//============================

int fanMap(int vento)
{
    switch(vento)
    {
        case 1: return kFujitsuAcFanAuto;
        case 2: return kFujitsuAcFanHigh;
        case 3: return kFujitsuAcFanMed;
        case 4: return kFujitsuAcFanLow;
        case 5: return kFujitsuAcFanQuiet;
        default: return kFujitsuAcFanAuto;
    }
}

//============================

void setEstadoCompleto(
    int arIndex,
    int estado,
    int temperatura,
    int modo,
    int vento
)
{
    if(arIndex < 0 || arIndex > 3)
        return;

    IRFujitsuAC &ac = ar[arIndex];

    // POWER
    if(estado == 1)
        ac.setCmd(kFujitsuAcCmdTurnOn);
    else
        ac.setCmd(kFujitsuAcCmdTurnOff);

    // TEMP
    if(temperatura == 1)
        ac.setTemp(24); // default seguro
    else if(temperatura == 0)
        ac.setTemp(24);

    // MODO
    ac.setMode(modoMap(modo));

    // VENTILACAO
    ac.setFanSpeed(fanMap(vento));

    ac.setSwing(kFujitsuAcSwingOff);

    ac.send();

    debugInfo("IR enviado AR " + String(arIndex + 1));
}