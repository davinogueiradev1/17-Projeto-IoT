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

void mensagemRecebida(const char *topico, const String &mensagem)
{
    debugInfo("=================");
    debugInfo("Mensagem MQTT recebida");
    debugInfo("=================");

    JsonDocument documento;

    DeserializationError erro = deserializeJson(documento, mensagem);

    if (erro)
    {
        debugErro("Erro ao desserializar JSON.");
        return;
    }

    int ar = documento["ar"];
    int comando = documento["comando"];
    int valor = documento["valor"];

    debugInfo("AR: " + String(ar));
    debugInfo("Comando: " + String(comando));
    debugInfo("Valor: " + String(valor));

    if (ar < 1 || ar > 4)
    {
        debugErro("Numero de ar invalido.");
        return;
    }

    int indiceAr = ar - 1;

    switch (comando)
    {
        case 1:
            ligarAr(indiceAr);
            break;

        case 2:
            desligarAr(indiceAr);
            break;

        case 3:
            aumentarTemperatura(indiceAr);
            break;

        case 4:
            diminuirTemperatura(indiceAr);
            break;

        case 5:
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