//secrets.cpp
#include "secrets.h"
#include <Arduino.h>

const char* WIFI_SSID = "SALA 09";
const char* WIFI_SENHA = "info@134";


//=======================
//MQTT
//=======================


const char* MQTT_BROKER = "07ee2a3fd39e4744b93c38344c0c9865.s1.eu.hivemq.cloud";
const int MQTT_PORTA = 8883;

const char* MQTT_CLIENT_ID = "esp32_fellipe_augusto";

const char* MQTT_USUARIO = "Fellipe";
const char* MQTT_SENHA = "Senai@134";

const bool MQTT_USAR_TLS = true;   //modo de conexao sem segurança

const char MQTT_CERTIFICADO_CA[] PROGMEM = "";


//=======================
//aws
//=======================
const char* AWS_IOT_CLIENT_ID = "GABRIEL_EXPINDOLA";

const bool USAR_AWS_IOT = true;

const char AWS_CERT_CA[] PROGMEM = R"EOF(-----BEGIN CERTIFICATE-----
MIIDQTCCAimgAwIBAgITBmyfz5m/jAo54vB4ikPmljZbyjANBgkqhkiG9w0BAQsF
ADA5MQswCQYDVQQGEwJVUzEPMA0GA1UEChMGQW1hem9uMRkwFwYDVQQDExBBbWF6
b24gUm9vdCBDQSAxMB4XDTE1MDUyNjAwMDAwMFoXDTM4MDExNzAwMDAwMFowOTEL
MAkGA1UEBhMCVVMxDzANBgNVBAoTBkFtYXpvbjEZMBcGA1UEAxMQQW1hem9uIFJv
b3QgQ0EgMTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALJ4gHHKeNXj
ca9HgFB0fW7Y14h29Jlo91ghYPl0hAEvrAIthtOgQ3pOsqTQNroBvo3bSMgHFzZM
9O6II8c+6zf1tRn4SWiw3te5djgdYZ6k/oI2peVKVuRF4fn9tBb6dNqcmzU5L/qw
IFAGbHrQgLKm+a/sRxmPUDgH3KKHOVj4utWp+UhnMJbulHheb4mjUcAwhmahRWa6
VOujw5H5SNz/0egwLX0tdHA114gk957EWW67c4cX8jJGKLhD+rcdqsq08p8kDi1L
93FcXmn/6pUCyziKrlA4b9v7LWIbxcceVOF34GfID5yHI9Y/QCB/IIDEgEw+OyQm
jgSubJrIqg0CAwEAAaNCMEAwDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8EBAMC
AYYwHQYDVR0OBBYEFIQYzIU07LwMlJQuCFmcx7IQTgoIMA0GCSqGSIb3DQEBCwUA
A4IBAQCY8jdaQZChGsV2USggNiMOruYou6r4lK5IpDB/G/wkjUu0yKGX9rbxenDI
U5PMCCjjmCXPI6T53iHTfIUJrU6adTrCC2qJeHZERxhlbI1Bjjt/msv0tadQ1wUs
N+gDS63pYaACbvXy8MWy7Vu33PqUXHeeE6V/Uq2V8viTO96LXFvKWlJbYK8U90vv
o/ufQJVtMVT8QtPHRh8jrdkPSHCa2XV4cdFyQzR1bldZwgJcJmApzyMZFo6IQ6XU
5MsI+yMRQ+hDKXJioaldXgjUkK642M4UwtBV8ob2xJNDd2ZhwLnoQdeXeGADbkpy
rqXRfboQnoZsG4q5WTP468SQvvG5
-----END CERTIFICATE-----
)EOF";

const char AWS_CERT_CRT[] PROGMEM = R"EOF(-----BEGIN CERTIFICATE-----
MIIDWTCCAkGgAwIBAgIUcWXJOj6l9nJs6qVbaWI8ux53L5UwDQYJKoZIhvcNAQEL
BQAwTTFLMEkGA1UECwxCQW1hem9uIFdlYiBTZXJ2aWNlcyBPPUFtYXpvbi5jb20g
SW5jLiBMPVNlYXR0bGUgU1Q9V2FzaGluZ3RvbiBDPVVTMB4XDTI2MDUxODE3MzE1
MloXDTQ5MTIzMTIzNTk1OVowHjEcMBoGA1UEAwwTQVdTIElvVCBDZXJ0aWZpY2F0
ZTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBANAfyWwnJ1H4QqmzwELd
gwlqfaQBVyoq2DGoAWNJjpcGkJjkBIuisa8kJfoh0xR+SOGDi+WD2jH3E28IiAzg
uyqgw/tlgBvQ+QwJhSgAxTA+2o3y/khGVmxWHFz/2JxfUVGw+ePmG+E9026W18Dl
9w+twNTngQBsDbNGPnBeSoueRweywgZBkQJgGuYZMLkGN6OqNNPWyXWmRyej2kov
n9nk6G6zRvH0NDs/fX61NkAYOy9LCdn3HrTXHWn64Prj3ZbpyfZrR7L3208s3R/6
SN7qQAmVJ2aB3DMrPCzSAivJvn5oKJEX5cC7FCuylY8j/8EMJjyA3ait9BMNyEWZ
4wsCAwEAAaNgMF4wHwYDVR0jBBgwFoAUm5GgbmxgseB4TmHTst/hhmkXWXEwHQYD
VR0OBBYEFGvbD5qUiXESLcW4TJy4j1mvmlGQMAwGA1UdEwEB/wQCMAAwDgYDVR0P
AQH/BAQDAgeAMA0GCSqGSIb3DQEBCwUAA4IBAQByiu9gZmZM6Cr0aGTVs/yGQDfG
sEhWr0rdSsWpgZy/m6K+bkHoxqeU6U5BB85dH4uxuZACKEVyM2Q3IMuZqRTVtDkE
iNDCn8LI5G6urz7bc/LmuuyZ6kD4oUU+2dAmbcmRUqwvheta9wIxp/na0Yrk7gP3
7Dfr5Fm8sFo0LcBRstE1KyYjFzqqmgMhXMgQekAnsAQbrt7i4U35ChvaGPE4hJch
TIDe4zL63jZLhABvGDCOw1cP3Y5LVGKKBzabs7kfxMCeqEOYJ7sWGr8kJUkySE3y
E4L8cFU0PtgybioPfu/JxXny00B8ukOV913bAnBZ8UX6eBeDTLJMV5daYICS
-----END CERTIFICATE-----
)EOF";

const char AWS_CERT_PRIVATE[] PROGMEM = R"EOF(-----BEGIN RSA PRIVATE KEY-----
MIIEpgIBAAKCAQEA0B/JbCcnUfhCqbPAQt2DCWp9pAFXKirYMagBY0mOlwaQmOQE
i6KxryQl+iHTFH5I4YOL5YPaMfcTbwiIDOC7KqDD+2WAG9D5DAmFKADFMD7ajfL+
SEZWbFYcXP/YnF9RUbD54+Yb4T3TbpbXwOX3D63A1OeBAGwNs0Y+cF5Ki55HB7LC
BkGRAmAa5hkwuQY3o6o009bJdaZHJ6PaSi+f2eTobrNG8fQ0Oz99frU2QBg7L0sJ
2fcetNcdafrg+uPdlunJ9mtHsvfbTyzdH/pI3upACZUnZoHcMys8LNICK8m+fmgo
kRflwLsUK7KVjyP/wQwmPIDdqK30Ew3IRZnjCwIDAQABAoIBAQCty42BAiUknyLT
4aXQcHfHOiUW+oOWU9bbiIc4KSmoVjQTaH2fOUoVjiMkgHBqsZYcs6P0Ein4BXI2
CzLJRjUlwMBHw5e6IsSF18FZCWOhCSuq09qh8hKjPzlKM9/nnPUkOZxDQlnPurg/
pjke/SpV7muTnoqsFo9OQH9NiH/Jm+4D3RqWVjmCIsGDKO6Mjr03mATBkOyR+Ir2
waQ6qkXwMTZmt+avYgpOaAcuhNzufJp7kQctoswSprZuzLBP3IIMOyV45pQKPW3/
3+g7pG61PQ2OTdSKML2XwHvzmtruzSliR2BN7UwGD2W4eKZFhuvjvUCRFtbul3T/
6RibYF1BAoGBAOfNIIPpBzBLPZwZlUrPAPtSmOpTHsw2kM1ZFSk2L6PeSOCWpWBK
mnKzdDIGxiMtSqiquoXQ9f4EzrOwvYc2g5tI+Om03gGc3QrRUiCi17LsqNc5tSVJ
BIpZGUOu2cmZDufxJl/Jm7pebuMM0J6OWjqA30tMn0O6RUHa+ufPhSbhAoGBAOXZ
5AM0fJxpS6bkdDXSdQtp7JBzjDDRuzRh/QMdhYIdlVkXpae2i5bzi3uQDV7KcK+l
PbFFTjpbavRRlG+iEMGS1+5NnGNN9H447GApRN9B/TaYNAHyWsQfZPEKO7f23cVR
HIS8LRLJX/c/ecE8bJBpiQw/mQoq5AECc7tvPgNrAoGBAKqqQJJec8h+3TGdNGoL
1vEzcxqzcC1yz1IhkI7zMkg4VPfmcoRe6TAiKB7dFey4YAunCAWWQEdySq3KJwDy
hhtCsuF/JSWS1LwuOYWF38IAtJ8iAdkd1ZX2278VcYwYgP0Mc/B6g4nIFWRd/wt+
S7hfQggyteAruOovzCP4ilSBAoGBAJxyDSc9T1+qr6pdrtd/ctkc9L5xV0dXSYNi
+rJnIpsohaHJXaBkuGg7RoIIz4Doa4CyCOai1JcV6Bt5d33wIlWPHFgADWTuB27u
Ub01+cG4CaBiQmvcyHjbIXqKy5Dwf4wxJCcYadlypVYFp1qmRQKaQoTKuODZxq0F
PT6jfeMXAoGBAJe/i8aqiB9z91N+1ZjbZg8XA77YIWaNNBim7LiQAvWfCciP7Qhi
PK6CRLkCsvS9em78KjT+Jgbp8zbrTjklhVBam+dARWHntUsWayFg4vNQQIm5X+mn
Ekjwuvgs3Cxp8iHn167ptKsdoDIRMdiHDQgJM+LVYH/gwEEowVWV8xI5
-----END RSA PRIVATE KEY-----
)EOF";

const char* AWS_IOT_ENDPOINT = "a13eikbd1cvk92-ats.iot.us-east-1.amazonaws.com";

const int AWS_IOT_PORT = 8883;







//=======================
//TOPICOS
//=======================

//raw string
const char* TOPICOS_PUBLICAR[] = {
    "senai134/fellipe/esp32/status",
    "senai134/fellipe/esp32/log",
    "senai134/fellipe/esp32/resposta"
};
const int TOTAL_TOPICOS_PUBLICAR = sizeof(TOPICOS_PUBLICAR) / sizeof (TOPICOS_PUBLICAR[0]);

const char* TOPICOS_RECEBER[] = {
    "senai134/fellipe/esp32/comando",
    "senai134/fellipe/esp32/config",
    "senai134/fellipe/esp32/display"
};
const int TOTAL_TOPICOS_RECEBER = sizeof(TOPICOS_RECEBER) / sizeof (TOPICOS_RECEBER[0]);   //quantos topicos eu posso receber

//====================================
//DEBUG
//====================================

// 0 = sem mensagens
// 1 = apenas erros
// 2 = todas as mensagens
const int DEBUG_NIVEL_INICIAL = 2;

// pino usado para forçar todas as mensagens na inicialização
const int PINO_HABILITA_DEBUG_COMPLETO = 4;