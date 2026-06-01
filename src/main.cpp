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
const uint16_t PINO_IR = 18;
uint8_t idAr;

IRFujitsuAC ac(PINO_IR);
Timezone timeStamp;

uint8_t estado;
uint8_t temperatura;
uint8_t modo;
uint8_t vento;

void setup()
{
    configurarDebug();
    ac.begin();
    ac.setModel(ARRAH2E);
    ac.setId(0);
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
            uint8_t esp = ar["esp"].as<uint8_t>();

            if (esp != 0 && esp != ESP_ID)
            {
                return;
            }
        }
        else
        {
            debugErro("ESP inválido.");
            return;
        }

        if (ar["id_ar"].is<uint8_t>())
        {
            idAr = ar["id_ar"].as<uint8_t>();

            // ESP 1 controla apenas os ARs 1 e 2
            if (ESP_ID == 1)
            {
                if (idAr != 1 && idAr != 2)
                {
                    debugInfo("AR nao pertence a este ESP.");
                    return;
                }
            }

            // ESP 2 controla apenas os ARs 3 e 4
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

            // Proteção para não virar uma sauna
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
            debugErro("modo inválido.");
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
            debugErro("vento inválido.");
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

    // OPÇÃO 1: Enviar como TEXTO formatado (Ex: "2026-06-01T16:21:00-03:00")
    LCD["timestamp"] = timeStamp.dateTime();

    // OPÇÃO 2: Enviar como NÚMERO Unix Epoch (Ex: 1772472060)
    // Se preferir o número puro, comente a linha de cima e use esta:
    // LCD["timestamp"] = timeStamp.now();

    // ATENÇÃO: Aumentei o buffer de 64 para 128 bytes!
    // Com o timestamp, o texto do JSON cresce e 64 bytes iriam cortar a mensagem.
    char buffer[128];

    serializeJson(resposta, buffer);

    publicarMensagem(
        "senai134/fellipe/esp32/status",
        buffer);

    debugInfo("Mensagem enviada ao grupo LCD com sucesso.");
}

void controlarAr()
{
    // 1. CONFIGURA OS PARÂMETROS PRIMEIRO
    // (Mudar o modo/vento altera o comando interno para "Alterar Ajuste")
    ac.setTemp(temperatura);

    switch (modo)
    {
    case 0:
        ac.setMode(kFujitsuAcModeCool);
        break;
    case 1:
        ac.setMode(kFujitsuAcModeDry);
        break;
    case 2:
        ac.setMode(kFujitsuAcModeFan);
        break;
    case 3:
        ac.setMode(kFujitsuAcModeHeat);
        break;
    }

    switch (vento)
    {
    case 0:
        ac.setFanSpeed(kFujitsuAcFanAuto);
        break;
    case 1:
        ac.setFanSpeed(kFujitsuAcFanQuiet);
        break;
    case 2:
        ac.setFanSpeed(kFujitsuAcFanLow);
        break;
    case 3:
        ac.setFanSpeed(kFujitsuAcFanMed);
        break;
    case 4:
        ac.setFanSpeed(kFujitsuAcFanHigh);
        break;
    }

    // 2. POR ÚLTIMO, O COMANDO DE ENERGIA
    // ac.on() ou ac.off() vão sobrescrever o byte de comando,
    // garantindo que o ar entenda que deve LIGAR ou DESLIGAR,
    // mas ainda levando junto a temperatura, modo e vento configurados acima.
    if (estado == 1)
    {
        ac.on();
        debugInfo("Ar configurado e ligado.");
    }
    else
    {
        ac.off();
        debugInfo("Ar desligado.");
    }

    // 3. ENVIA O SINAL COMPLETO
    ac.send();

    Serial.println(ac.toString());
}