//! WiFiManager.cpp
#include <Arduino.h>
#include "WiFiManager.h"
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include "secrets.h"
#include <WiFiClient.h>
#include "DebugManager.h"
bool wifiEstaConectado ()
{
    return WiFi.status() == WL_CONNECTED;
}

void conectarWiFi()
{
  debugInfo("===========================");
  debugInfo("Iniciando conexão WiFi...");
  debugInfo("===========================");

  // Configura o ESP32 como station, ou seja 
  //ele vai se conectar a um roteador existente.
  WiFi.mode(WIFI_STA); 

  //inicia a conexão com SSID e senha
  WiFi.begin(WIFI_SSID, WIFI_SENHA);

  debugInfo("conectando");

int tentativasWiFi = 0;
const int maxTentativasWiFi = 30;
  
  //Aguarda a conexão por até 30 tentativas
  while(WiFi.status() != WL_CONNECTED && tentativasWiFi < maxTentativasWiFi)
  {
    delay(500);
    debugInfoSemLinha(".");
    tentativasWiFi++;
  }

  debugInfoSemLinha("\n\r");
  if(WiFi.status() == WL_CONNECTED)
  {
    debugInfoSemLinha("WiFi conectado comsucesso!");
    debugInfoSemLinha("[INFO] Endereço IP: ");
    debugInfoSemLinha( WiFi.localIP().toString());
  }

  else
  {
    debugErro("Falha ao conectar noWiFi.");
    debugErro("Verifique SSID, senha e sinal de rede.");
  }
}

void garantirWiFiConectado()
{
  if(WiFi.status() != WL_CONNECTED)
  {
    debugErro("WiFi desconectado. Tentando reconectar...");
    conectarWiFi();
  }

  if(WiFi.status() != WL_CONNECTED)
  {
    debugErro("Não foi possivel reconectar ao WiFi.");
  }
}