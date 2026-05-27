#include "AirConditionerManager.h"

#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <ir_Fujitsu.h>

#include "DebugManager.h"

//==================================================
// HARDWARE IR (TSAL)
//==================================================

IRFujitsuAC ac[4] =
{
    IRFujitsuAC(16),
    IRFujitsuAC(17),
    IRFujitsuAC(18),
    IRFujitsuAC(19)
};

//==================================================
// ESTADO INTERNO (CRÍTICO)
//==================================================

struct EstadoAr
{
    bool ligado;
    int temperatura;
    uint8_t modo;
    uint8_t vento;
};

EstadoAr estados[4];

//==================================================
// MAPAS
//==================================================

uint8_t mapModo(int m)
{
    switch(m)
    {
        case 1: return kFujitsuAcModeAuto;
        case 2: return kFujitsuAcModeCool;
        case 3: return kFujitsuAcModeDry;
        case 4: return kFujitsuAcModeFan;
        default: return kFujitsuAcModeCool;
    }
}

uint8_t mapVento(int v)
{
    switch(v)
    {
        case 1: return kFujitsuAcFanAuto;
        case 2: return kFujitsuAcFanHigh;
        case 3: return kFujitsuAcFanMed;
        case 4: return kFujitsuAcFanLow;
        case 5: return kFujitsuAcFanQuiet;
        default: return kFujitsuAcFanAuto;
    }
}

//==================================================
// ENVIO IR (SEMPRE ESTADO COMPLETO)
//==================================================

void enviar(int i)
{
    if(estados[i].ligado)
        ac[i].setCmd(kFujitsuAcCmdTurnOn);
    else
        ac[i].setCmd(kFujitsuAcCmdTurnOff);

    ac[i].setTemp(estados[i].temperatura);
    ac[i].setMode(estados[i].modo);
    ac[i].setFanSpeed(estados[i].vento);
    ac[i].setSwing(kFujitsuAcSwingOff);

    ac[i].send();

    debugInfo("IR enviado AR " + String(i + 1));
}

//==================================================
// SETUP
//==================================================

void configurarArCondicionado()
{
    for(int i = 0; i < 4; i++)
    {
        ac[i].begin();
        ac[i].setModel(ARRAH2E);

        estados[i].ligado = false;
        estados[i].temperatura = 24;
        estados[i].modo = kFujitsuAcModeCool;
        estados[i].vento = kFujitsuAcFanAuto;
    }

    debugInfo("Ar condicionado pronto");
}

//==================================================
// UPDATE PRINCIPAL
//==================================================

void atualizarEstado(
    int ar,
    int estado,
    int temperaturaCmd,
    int modo,
    int vento
)
{
    if(ar < 0 || ar > 3)
        return;

    // POWER
    estados[ar].ligado = (estado == 1);

    // TEMPERATURA (COMANDO RELATIVO)
    if(temperaturaCmd == 1)
        estados[ar].temperatura++;
    else if(temperaturaCmd == 0)
        estados[ar].temperatura--;

    if(estados[ar].temperatura < 16)
        estados[ar].temperatura = 16;

    if(estados[ar].temperatura > 30)
        estados[ar].temperatura = 30;

    // MODO
    estados[ar].modo = mapModo(modo);

    // VENTO
    estados[ar].vento = mapVento(vento);

    enviar(ar);
}