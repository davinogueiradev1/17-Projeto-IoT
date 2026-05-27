#include <Arduino.h>
#include <ArduinoJson.h>

#include "WiFiManager.h"
#include "MqttManager.h"
#include "DebugManager.h"
#include "AirConditionerManager.h"

//==================================================

void mensagemRecebida(const char *topico, const String &mensagem)
{
    debugInfo("MQTT recebido");

    JsonDocument doc;

    if(deserializeJson(doc, mensagem))
    {
        debugErro("JSON invalido");
        return;
    }

    JsonObject ar = doc["ar-condicionado"];

    int esp = ar["esp"];
    int estado = ar["estado"];
    int temperatura = ar["temperatura"];
    int modo = ar["modo"];
    int vento = ar["vento"];

    if(esp < 1 || esp > 4)
    {
        debugErro("ESP invalido");
        return;
    }

    int index = esp - 1;

    atualizarEstado(
        index,
        estado,
        temperatura,
        modo,
        vento
    );
}

//==================================================

void setup()
{
    configurarDebug();

    conectarWifi();

    configurarMQTT();

    registrarCallbackMensagem(mensagemRecebida);

    conectarMQTT();

    configurarArCondicionado();
}

//==================================================

void loop()
{
    garantirWiFiConectado();
    garantirMQTTConectado();
    loopMQTT();
}