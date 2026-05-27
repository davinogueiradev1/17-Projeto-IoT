#include "AirConditionerManager.h"
#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <ir_Fujitsu.h>
#include "DebugManager.h"

//==================================================
// STRUCT
//==================================================

struct EstadoAr
{
    bool ligado;

    int temperatura;

    uint8_t modo;

    uint8_t ventilacao;

    uint8_t swing;
};

//==================================================
// PINOS IR
//==================================================

IRFujitsuAC ar1(16);

IRFujitsuAC ar2(17);

IRFujitsuAC ar3(18);

IRFujitsuAC ar4(19);

//==================================================
// ARRAY
//==================================================

IRFujitsuAC* aparelhos[] =
{
    &ar1,
    &ar2,
    &ar3,
    &ar4
};

//==================================================
// ESTADOS
//==================================================

EstadoAr estados[4];

//==================================================
// FUNCOES
//==================================================

void aplicarEstado(int ar)
{
    aparelhos[ar]->setTemp(
        estados[ar].temperatura
    );

    aparelhos[ar]->setMode(
        estados[ar].modo
    );

    aparelhos[ar]->setFanSpeed(
        estados[ar].ventilacao
    );

    aparelhos[ar]->setSwing(
        estados[ar].swing
    );

    if(estados[ar].ligado)
    {
        aparelhos[ar]->setCmd(
            kFujitsuAcCmdTurnOn
        );
    }

    else
    {
        aparelhos[ar]->setCmd(
            kFujitsuAcCmdTurnOff
        );
    }

    aparelhos[ar]->send();

    debugInfo(
        "Comando IR enviado para AR "
        + String(ar + 1)
    );
}

//==================================================

void configurarArCondicionado()
{
    ar1.begin();

    ar2.begin();

    ar3.begin();

    ar4.begin();

    for(int i = 0; i < 4; i++)
    {
        estados[i].ligado = false;

        estados[i].temperatura = 24;

        estados[i].modo =
            kFujitsuAcModeCool;

        estados[i].ventilacao =
            kFujitsuAcFanHigh;

        estados[i].swing =
            kFujitsuAcSwingOff;

        aparelhos[i]->setModel(
            ARRAH2E
        );
    }

    debugInfo(
        "Ar condicionado configurado."
    );
}

//==================================================

void ligarAr(int ar)
{
    estados[ar].ligado = true;

    aplicarEstado(ar);
}

//==================================================

void desligarAr(int ar)
{
    estados[ar].ligado = false;

    aplicarEstado(ar);
}

//==================================================

void aumentarTemperatura(int ar)
{
    if(estados[ar].temperatura < 30)
    {
        estados[ar].temperatura++;

        aplicarEstado(ar);
    }
}

//==================================================

void diminuirTemperatura(int ar)
{
    if(estados[ar].temperatura > 16)
    {
        estados[ar].temperatura--;

        aplicarEstado(ar);
    }
}

//==================================================

void definirTemperatura(
    int ar,
    int temperatura
)
{
    if(
        temperatura >= 16
        &&
        temperatura <= 30
    )
    {
        estados[ar].temperatura =
            temperatura;

        aplicarEstado(ar);
    }
}