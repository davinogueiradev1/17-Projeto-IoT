//!mqttManager.cpp

#include <Arduino.h>

#include "MQTTManager.h"

#include <PubSubClient.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h>

//TODO: Talvez precise .cpp do secrets
#include "secrets.h"
#include "WiFiManager.h"
#include "DebugManager.h"

//===========Instancias==========
WiFiClient wifiCliente;
WiFiClientSecure wifiClienteSecure;
PubSubClient mqttClient;

CallbackMensagemMQTT callbackDaAplicacao = nullptr;

void registrarCallbackMensagem(CallbackMensagemMQTT callback)
{
    callbackDaAplicacao = callback;

    if(callbackDaAplicacao != nullptr)
    {
        debugInfo("Callback da aplicacao registrada com sucesso.");
    }
    else
    {
        debugErro("Callback da aplicacao não foi registrada.");
    }
}

const char* obterTopicoPublicacao(int indiceTopico)
{
    if(indiceTopico < 0 || indiceTopico >= TOTAL_TOPICOS_PUBLICAR) // quando tem || é ou
    {
        debugErro("Indice inválido para tópico de publicação: " + String(indiceTopico));
        return "";
    }

    return TOPICOS_PUBLICAR[indiceTopico];// se o if for falso
}



const char* obterTopicoRecebimento(int indiceTopico)
{
    if(indiceTopico < 0 || indiceTopico >= TOTAL_TOPICOS_RECEBER) // quando tem || é ou
    {
        debugErro("Indice inválido para tópico de recebimento: " + String(indiceTopico));
        return "";
    }

    return TOPICOS_RECEBER[indiceTopico];// se o if for falso
}

void callbackInternoMQTT(char* topico, byte* payload, unsigned int tamanho)
{
    String mensagem = "";

    for(unsigned int i = 0; i < tamanho; i ++)
    {
        mensagem += (char)payload[i]; // +=atribuição por soma
    }

    debugInfo("======================");
    debugInfo("mensagem MQTT recebida");
    debugInfo("======================");
    debugInfo(" Tópico: " + String(topico));
    debugInfo(" Mensagem: " + mensagem);

    if(callbackDaAplicacao != nullptr)
    {
        callbackDaAplicacao(topico, mensagem);
    }
    else
    {
        debugErro("Mensagem recebida, mas  nenhum callback da aplicação foi registrado");
    }
}

void configurarMQTT() //
{
    debugInfo("=====================");
    debugInfo(" Configurando MQTT...");
    debugInfo("=====================");

    if(USAR_AWS_IOT)
    {
        debugInfo("modo selecaonado: AWS IoT Core");
            debugInfo("Configurando certificados da AWS IoT Core.");

            wifiClienteSecure.setCACert(AWS_CERT_CA);
            wifiClienteSecure.setCertificate(AWS_CERT_CRT);
            wifiClienteSecure.setPrivateKey(AWS_CERT_PRIVATE);

            mqttClient.setClient(wifiClienteSecure);
            mqttClient.setServer(AWS_IOT_ENDPOINT, AWS_IOT_PORT);

            debugInfo("Endpoint AWS IoT : " + String(AWS_IOT_ENDPOINT));
            debugInfo("Porta AWS IoT: " + String(AWS_IOT_PORT));
    }

    else if (MQTT_USAR_TLS)
    {
        debugInfo("Modo selecionado: MQTT com TLS.");

        if(strlen(MQTT_CERTIFICADO_CA) > 100)
        {
            debugInfo("Certificado CA do broker MQTT configurado.");
            wifiClienteSecure.setCACert(MQTT_CERTIFICADO_CA);
        }

        else
        {
            debugErro("Certificado CA do MQTT não configurado. Usando setInsecure apenas teste");
            wifiClienteSecure.setInsecure();
        }

 debugInfo("Modo selecionado: MQTT sem TLS.");

        mqttClient.setClient(wifiClienteSecure);
        mqttClient.setServer(MQTT_BROKER, MQTT_PORTA);

        debugInfo("Broker MQTT: " + String(MQTT_BROKER));
        debugInfo("Porta MQTT: " + String(MQTT_PORTA));
    }

    else // conectar no broker sem certificado
    {
        debugInfo("Modo selecionado: MQTT sem TLS.");

        mqttClient.setClient(wifiCliente);
        mqttClient.setServer(MQTT_BROKER, MQTT_PORTA);

        debugInfo("Broker MQTT: " + String(MQTT_BROKER));
        debugInfo("Porta MQTT: " + String(MQTT_PORTA));
    }

    mqttClient.setCallback(callbackInternoMQTT);
    debugInfo("Callback interno do MQTT configurado");
}

void conectarMQTT() //
{
    if(!wifiEstaConectado())
    {
        debugErro("MQTT não pode conectar porque o WiFi está desconectado.");
        return;
    }

    debugInfo("=========================");
    debugInfo("Iniciando conexão MQTT...");
    debugInfo("=========================");

    int tentativasMQTT = 0;
    const int maxTentativasMQTT = 5; // vai tentar conectar 5 veses e depois vai parar

    while(!mqttClient.connected() && tentativasMQTT < maxTentativasMQTT)// vai realizar a conexao ate conectar ou vai tentar 5 veses e depois vai parar
    {
        debugInfo("Tentando conectar ao broker MQTT. Tentativa: " + String(tentativasMQTT));

        bool conectado = false;

        if(USAR_AWS_IOT)
        {
            conectado = mqttClient.connect(AWS_IOT_CLIENT_ID);
        }
        else
        {
            if(strlen(MQTT_USUARIO) > 0)
            {
                debugInfo("Conectando MQTT com usuário e senha");

                conectado = mqttClient.connect(MQTT_CLIENT_ID, MQTT_USUARIO, MQTT_SENHA);
            }
            else    //sem usuario
            {
                debugInfo("Conectendo MQTT sem usuário e senha.");

                conectado = mqttClient.connect(MQTT_CLIENT_ID);
            }
        }
        if(conectado)
        {
            debugInfo("MQTT conectado com sucesso.");

            int totalTopicos = obterTotalTopicosRecebimento();

            debugInfo("Total de tópicos para inscrição: " + String(totalTopicos));

            for(int i = 0; i < totalTopicos; i++)
            {
                const char* topico = obterTopicoRecebimento(i);

                bool inscrito = mqttClient.subscribe(topico);

                if(inscrito)
                {
                    debugInfo("Inscrito no tópico: " + String(topico));
                }
                else
                {
                    debugErro("Falha ao se inscrever no tópico: " + String(topico));
                }
            }
            //TODO: publicar uma mensagem em um tópico informando que o esp foi cinectado.
        }
        else
        {
            debugErro("Falha ao conectar no MQTT. Código de erro: " + String(mqttClient.state() ) );

            tentativasMQTT++;
            delay(2000);
        }
    }//fim do while

    if(!mqttClient.connected())
    {
        debugErro("Não foi possivel conectar ao MQTT após " + String(maxTentativasMQTT) + " tentativas" );
    }
}

void garantirMQTTConectado()//
{
    if(!wifiEstaConectado())
    {
        debugErro("MQTT não será reconectado porque o WiFi está desconectado.");
        return;
    }

    if(!mqttClient.connected())
    {
        debugErro("MQTT desconectado. Tentando reconectar...");
        conectarMQTT();
    }
}

void loopMQTT()//
{
    mqttClient.loop();
}

void publicarMensagem(const char* topico, const char* mensagem)//
{
    if(!mqttClient.connected())
    {
        debugErro("Não foi possivel publicar. MQTT desconectado.");
        return;
    }

    bool publicado = mqttClient.publish(topico, mensagem);

    if(publicado)
    {
        debugInfo("Mensagem publicada via MQTT.");
        debugInfo("Topico: " + String(topico));
        debugInfo("Mensagem: " + String(mensagem));
    }
    else
    {
        debugErro("Falha ao publicar mensagem no tópico: " + String(topico));
    }
}

void publicarMensagemNoTopico(int indiceTopico, const char* mensagem)//
{
    const char* topico = obterTopicoPublicacao(indiceTopico);

    if(strlen(topico) == 0)
    {
        debugErro("Não foi possivel publicar. Indice de tópico inválido: " + String(indiceTopico));
        return;
    }
    publicarMensagem(topico, mensagem);
}

bool mqttEstaConectado()
{
    return mqttClient.connected();
}

int obterTotalTopicosRecebimento()
{
    return TOTAL_TOPICOS_RECEBER;
}