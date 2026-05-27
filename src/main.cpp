#include <Arduino.h>
#include <ArduinoJson.h>

#include "WiFiManager.h"
#include "MqttManager.h"
#include "DebugManager.h"
#include "AirConditionerManager.h"

//==================================================
// DEFINES
//==================================================

#define COMANDO_LIGAR              1
#define COMANDO_DESLIGAR           2
#define COMANDO_AUMENTAR_TEMP      3
#define COMANDO_DIMINUIR_TEMP      4
#define COMANDO_DEFINIR_TEMP       5

//==================================================
// CALLBACK MQTT
//==================================================

void mensagemRecebida(char* topico, String mensagem)
{
    debugInfo("=================");
    debugInfo("Mensagem MQTT recebida");
    debugInfo("=================");

    //========================================
    // JSON
    //========================================

    JsonDocument documento;

    DeserializationError erro =
        deserializeJson(documento, mensagem);

    if(erro)
    {
        debugErro("Erro ao desserializar JSON.");
        return;
    }

    //========================================
    // OBTENDO VALORES
    //========================================

    int ar = documento["ar"];
    int comando = documento["comando"];
    int valor = documento["valor"];

    debugInfo("AR: " + String(ar));
    debugInfo("Comando: " + String(comando));
    debugInfo("Valor: " + String(valor));

    //========================================
    // VALIDACAO
    //========================================

    if(ar < 1 || ar > 4)
    {
        debugErro("Numero de ar invalido.");
        return;
    }

    int indiceAr = ar - 1;

    //========================================
    // COMANDOS
    //========================================

    switch(comando)
    {
        case COMANDO_LIGAR:

            ligarAr(indiceAr);

        break;

        case COMANDO_DESLIGAR:

            desligarAr(indiceAr);

        break;

        case COMANDO_AUMENTAR_TEMP:

            aumentarTemperatura(indiceAr);

        break;

        case COMANDO_DIMINUIR_TEMP:

            diminuirTemperatura(indiceAr);

        break;

        case COMANDO_DEFINIR_TEMP:

            definirTemperatura(indiceAr, valor);

        break;

        default:

            debugErro("Comando invalido.");

        break;
    }
}

//==================================================
// SETUP
//==================================================

void setup()
{
    configurarDebug();

    conectarWifi();

    configurarMQTT();

    registrarCallbackMensagem(mensagemRecebida);

    conectarMQTT();

    configurarArCondicionado();

    debugInfo("Sistema iniciado.");
}

//==================================================
// LOOP
//==================================================

void loop()
{
    garantirWiFiConectado();

    garantirMQTTConectado();

    loopMQTT();

    delay(10);
}