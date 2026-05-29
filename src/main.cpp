#include <Arduino.h>
#include <DebugManager.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <ir_Fujitsu.h>
#include "WiFiManager.h"
#include "MqttManager.h"
#include <ArduinoJson.h>

const char TOPICO_COMANDO[] = "senai134/fellipe/esp32/comando";

void tratarMensagemRecebida(const char* topico, const String& mensagem);
void tratarJsonComando(const String &mensagem);
void enviarACK();
void controlarAr();

const uint8_t ESP_ID = 1;
const uint16_t PINO_IR = 18;
IRFujitsuAC ac(PINO_IR);

uint8_t estado;
uint8_t temperatura;
uint8_t modo;
uint8_t vento;

void setup() 
{
  configurarDebug();
  ac.begin();
  conectarWiFi();
  configurarMQTT();
  registrarCallbackMensagem(tratarMensagemRecebida);
  conectarMQTT();
}

void loop() 
{
  garantirWiFiConectado();
  garantirMQTTConectado();
  loopMQTT();
}

void tratarMensagemRecebida(const char* topico, const String& mensagem)
{
  debugInfo("==============================");
  debugInfo("Mensagem recebida na aplicação");
  debugInfo("==============================");
  
  if(topico == nullptr)
  {
    debugErro("Tópico MQTT inválido");
    return;
  }

  debugInfo("Tópico: " + String(topico));
  debugInfo("Mensagem: " + mensagem);

  if(strcmp(topico, TOPICO_COMANDO) == 0)
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

  if(erro)
  {
    debugErro("Erro ao interpretar JSON.");
    debugErro(erro.c_str());
    return;
  }

  if(doc["ar-condicionado"].is<JsonObject>())
  {
    JsonObject ar = doc["ar-condicionado"];

    if(ar["esp"].is<uint8_t>())
    {
        uint8_t esp = ar["esp"].as<uint8_t>();

        if(esp != 0 && esp != ESP_ID)
        {
            return;
        }
    }
    else
    {
        debugErro("ESP inválido.");
        return;
    }

    if(ar["estado"].is<uint8_t>())
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

    if(ar["temperatura"].is<uint8_t>())
    {
      temperatura = ar["temperatura"].as<uint8_t>();

      debugInfoSemLinha("Temperatura: ");
      Serial.println(temperatura);
    }
    else
    {
      debugErro("Temperatura inválida.");
      return;
    }

    if(ar["modo"].is<uint8_t>())
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

    if(ar["vento"].is<uint8_t>())
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

    JsonObject ar =
        resposta["ar-condicionado"].to<JsonObject>();

    ar["codigo"] = 1000;

    char buffer[64];

    serializeJson(resposta, buffer);

    publicarMensagem(
        "senai134/fellipe/esp32/status",
        buffer
    );

    debugInfo("Mensagem enviada ao grupo LCD com sucesso.");
}

void controlarAr()
{
            // ===== LIGAR =====
        if(estado == 1)
        {
            ac.on();

            ac.send();

            debugInfo("Ar ligado");
        }

        // ===== DESLIGAR =====
        else
        {
            ac.off();

            ac.send();

            debugInfo("Ar desligado");
        }
}