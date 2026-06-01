#include <Arduino.h>
#include <DebugManager.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <ir_Fujitsu.h>
#include "WiFiManager.h"
#include "MqttManager.h"
#include <ArduinoJson.h>
#include <ezTime.h>

const char TOPICO_COMANDO[] = "senai134/fellipe/esp32/comando";

void tratarMensagemRecebida(const char *topico, const String &mensagem);
void tratarJsonComando(const String &mensagem);
void enviarACK();
void controlarAr();

const uint8_t ESP_ID = 2;
uint8_t idAr;
uint8_t espRecebido;

IRFujitsuAC ac18(18);
IRFujitsuAC ac17(17);

Timezone timeStamp;

uint8_t estado;
uint8_t temperatura;
uint8_t modo;
uint8_t vento;

void setup()
{
    configurarDebug();

    ac18.begin();
    ac18.setModel(ARRAH2E);
    ac18.setId(0);

    ac17.begin();
    ac17.setModel(ARRAH2E);
    ac17.setId(0);

    conectarWiFi();
    configurarMQTT();
    registrarCallbackMensagem(tratarMensagemRecebida);
    conectarMQTT();

    setInterval(3600);
    waitForSync();

    timeStamp.setLocation("America/Sao_Paulo");
}

void loop()
{
    garantirWiFiConectado();
    garantirMQTTConectado();
    loopMQTT();
    events();
}

void tratarMensagemRecebida(const char *topico, const String &mensagem)
{
    debugInfo("==============================");
    debugInfo("Mensagem recebida na aplicação");
    debugInfo("==============================");

    if (topico == nullptr)
    {
        debugErro("Tópico MQTT inválido");
        return;
    }

    debugInfo("Tópico: " + String(topico));
    debugInfo("Mensagem: " + mensagem);

    if (strcmp(topico, TOPICO_COMANDO) == 0)
    {
        tratarJsonComando(mensagem);
        return;
    }

    debugErro("Tópico não tratado: " + String(topico));
}

void tratarJsonComando(const String &mensagem)
{
    JsonDocument doc;

    DeserializationError erro = deserializeJson(doc, mensagem);

    if (erro)
    {
        debugErro("Erro ao interpretar JSON.");
        debugErro(erro.c_str());
        return;
    }

    if (doc["ar-condicionado"].is<JsonObject>())
    {
        JsonObject ar = doc["ar-condicionado"];

        if (ar["esp"].is<uint8_t>())
        {
            espRecebido = ar["esp"].as<uint8_t>();

            if (espRecebido != 0 && espRecebido != ESP_ID)
            {
                return;
            }
        }
        else
        {
            debugErro("ESP inválido.");
            return;
        }

       if (espRecebido != 0)
{
    if (ar["id_ar"].is<uint8_t>())
    {
        idAr = ar["id_ar"].as<uint8_t>();

        if (ESP_ID == 1)
        {
            if (idAr != 1 && idAr != 2)
            {
                debugInfo("AR nao pertence a este ESP.");
                return;
            }
        }

        if (ESP_ID == 2)
        {
            if (idAr != 3 && idAr != 4)
            {
                debugInfo("AR nao pertence a este ESP.");
                return;
            }
        }

        debugInfoSemLinha("ID AR: ");
        Serial.println(idAr);
    }
    else
    {
        debugErro("ID do ar invalido.");
        return;
    }
}
        if (ar["estado"].is<uint8_t>())
        {
            estado = ar["estado"].as<uint8_t>();

            debugInfoSemLinha("Estado: ");
            Serial.println(estado);
        }
        else
        {
            debugErro("Estado inválido.");
            return;
        }

        if (ar["temperatura"].is<uint8_t>())
        {
            temperatura = ar["temperatura"].as<uint8_t>();

            if (temperatura < 18)
                temperatura = 18;

            if (temperatura > 30)
                temperatura = 30;

            debugInfoSemLinha("Temperatura: ");
            Serial.println(temperatura);
        }
        else
        {
            debugErro("Temperatura inválida.");
            return;
        }

        if (ar["modo"].is<uint8_t>())
        {
            modo = ar["modo"].as<uint8_t>();

            debugInfoSemLinha("Modo: ");
            Serial.println(modo);
        }
        else
        {
            debugErro("Modo inválido.");
            return;
        }

        if (ar["vento"].is<uint8_t>())
        {
            vento = ar["vento"].as<uint8_t>();

            debugInfoSemLinha("Vento: ");
            Serial.println(vento);
        }
        else
        {
            debugErro("Vento inválido.");
            return;
        }
    }
    else
    {
        debugErro("Objeto ar-condicionado inválido.");
        return;
    }

    enviarACK();
    controlarAr();
}

void enviarACK()
{
    JsonDocument resposta;

    JsonObject LCD = resposta["grupo LCD"].to<JsonObject>();

    LCD["codigo"] = 1000;
    LCD["timestamp"] = timeStamp.dateTime();

    char buffer[128];

    serializeJson(resposta, buffer);

    publicarMensagem(
        "senai134/fellipe/esp32/status",
        buffer);

    debugInfo("Mensagem enviada ao grupo LCD com sucesso.");
}

void controlarAr()
{
    if (espRecebido == 0)
{
    IRFujitsuAC* aparelhos[] = { &ac18, &ac17 };

    for (int i = 0; i < 2; i++)
    {
        aparelhos[i]->setTemp(temperatura);

        switch (modo)
        {
        case 0:
            aparelhos[i]->setMode(kFujitsuAcModeCool);
            break;
        case 1:
            aparelhos[i]->setMode(kFujitsuAcModeDry);
            break;
        case 2:
            aparelhos[i]->setMode(kFujitsuAcModeFan);
            break;
        case 3:
            aparelhos[i]->setMode(kFujitsuAcModeHeat);
            break;
        }

        switch (vento)
        {
        case 0:
            aparelhos[i]->setFanSpeed(kFujitsuAcFanAuto);
            break;
        case 1:
            aparelhos[i]->setFanSpeed(kFujitsuAcFanQuiet);
            break;
        case 2:
            aparelhos[i]->setFanSpeed(kFujitsuAcFanLow);
            break;
        case 3:
            aparelhos[i]->setFanSpeed(kFujitsuAcFanMed);
            break;
        case 4:
            aparelhos[i]->setFanSpeed(kFujitsuAcFanHigh);
            break;
        }

        if (estado == 1)
            aparelhos[i]->on();
        else
            aparelhos[i]->off();

        aparelhos[i]->send();
    }

    debugInfo("Comando enviado para todos os ares deste ESP.");
    return;
}
    IRFujitsuAC *acSelecionado = nullptr;

    if (ESP_ID == 1)
    {
        if (idAr == 1)
            acSelecionado = &ac18;
        else if (idAr == 2)
            acSelecionado = &ac17;
    }
    else if (ESP_ID == 2)
    {
        if (idAr == 3)
            acSelecionado = &ac18;
        else if (idAr == 4)
            acSelecionado = &ac17;
    }

    if (acSelecionado == nullptr)
    {
        debugErro("Nenhum emissor IR selecionado.");
        return;
    }

    acSelecionado->setTemp(temperatura);

    switch (modo)
    {
    case 0:
        acSelecionado->setMode(kFujitsuAcModeCool);
        break;
    case 1:
        acSelecionado->setMode(kFujitsuAcModeDry);
        break;
    case 2:
        acSelecionado->setMode(kFujitsuAcModeFan);
        break;
    case 3:
        acSelecionado->setMode(kFujitsuAcModeHeat);
        break;
    }

    switch (vento)
    {
    case 0:
        acSelecionado->setFanSpeed(kFujitsuAcFanAuto);
        break;
    case 1:
        acSelecionado->setFanSpeed(kFujitsuAcFanQuiet);
        break;
    case 2:
        acSelecionado->setFanSpeed(kFujitsuAcFanLow);
        break;
    case 3:
        acSelecionado->setFanSpeed(kFujitsuAcFanMed);
        break;
    case 4:
        acSelecionado->setFanSpeed(kFujitsuAcFanHigh);
        break;
    }

    if (estado == 1)
    {
        acSelecionado->on();
        debugInfo("Ar configurado e ligado.");
    }
    else
    {
        acSelecionado->off();
        debugInfo("Ar desligado.");
    }

    acSelecionado->send();

    Serial.println(acSelecionado->toString());
}