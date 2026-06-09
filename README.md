# <div align="center">❄️ Smart AC Control</div>

<div align="center">

![header](https://capsule-render.vercel.app/api?type=waving&color=0:00BFFF,100:1E90FF&height=250&section=header&text=Smart%20AC%20Control&fontSize=50&fontColor=ffffff)

<img src="https://readme-typing-svg.herokuapp.com?font=Fira+Code&size=24&duration=3000&pause=1000&color=00BFFF&center=true&vCenter=true&width=700&lines=Controle+Inteligente+de+Ar-Condicionado;ESP32+%2B+MQTT+%2B+IR;Projeto+de+Internet+das+Coisas+(IoT)" alt="Typing SVG" />

<br>

![ESP32](https://img.shields.io/badge/ESP32-Microcontrolador-blue)
![MQTT](https://img.shields.io/badge/MQTT-Protocolo-orange)
![Arduino](https://img.shields.io/badge/Arduino-Framework-green)
![JSON](https://img.shields.io/badge/JSON-Comunicação-lightgrey)
![Status](https://img.shields.io/badge/Status-Concluído-brightgreen)

</div>

---

## 📖 Sobre o Projeto

O **Smart AC Control** é uma solução IoT desenvolvida para controlar aparelhos de ar-condicionado remotamente utilizando um ESP32.

Os comandos são enviados via MQTT em formato JSON, processados pelo microcontrolador e transmitidos ao equipamento através de sinais infravermelhos (IR).

O sistema foi projetado para possibilitar o gerenciamento centralizado de múltiplos aparelhos de ar-condicionado, tornando o controle mais simples, eficiente e escalável.

---

## Funcionalidades

• Comunicação MQTT
• Conexão Wi-Fi
• Liga/Desliga remoto
• Controle de temperatura
• Alteração de modo de operação
• Controle da velocidade do ventilador
• Processamento de mensagens JSON
• Aplicação em automação residencial

---

## Arquitetura do Sistema

```text
┌─────────────────────┐
│      Usuário        │
└──────────┬──────────┘
           │
           ▼
┌─────────────────────┐
│Interface de Controle│
└──────────┬──────────┘
           │
           ▼
┌─────────────────────┐
│     Broker MQTT     │
└──────────┬──────────┘
           │
           ▼
┌─────────────────────┐
│        ESP32        │
└──────────┬──────────┘
           │
           ▼
┌─────────────────────┐
│ LED Infravermelho   │
└──────────┬──────────┘
           │
           ▼
┌─────────────────────┐
│  Ar-Condicionado    │
└─────────────────────┘
```

---

## Estrutura da Mensagem MQTT

Exemplo de mensagem recebida pelo ESP32:

```json
{
  "ar-condicionado": {
    "esp": 1,
    "estado": true,
    "temperatura": 22,
    "modo": 0,
    "vento": 3
  }
}
```

---

## Parâmetros

| Campo | Tipo | Descrição |
|---------|---------|-----------|
| esp | int | Identificador do ESP responsável |
| estado | bool | Liga (`true`) ou desliga (`false`) |
| temperatura | float | Temperatura desejada |
| modo | int | Modo de operação |
| vento | int | Velocidade do ventilador |

---

## Modos de Operação

| Valor | Modo |
|---------|---------|
| 0 | Auto |
| 1 | Cool |
| 2 | Dry |
| 3 | Fan |

---

## Velocidades do Ventilador

| Valor | Velocidade |
|---------|---------|
| 0 | Auto |
| 1 | Baixa |
| 2 | Média |
| 3 | Alta |

---

## Hardware Utilizado

• ESP32
• LED Infravermelho (IR)
• Resistor para o LED IR
• Fonte de alimentação USB
• Ar-condicionado compatível com controle infravermelho

---

## Tecnologias Utilizadas

• ESP32
• Arduino Framework
• MQTT
• JSON
• Wi-Fi
• IRremoteESP8266
• Git
• GitHub

---

## Instalação

### 1. Clonar o repositório

```bash
git clone //https://github.com/davinogueiradev1/17-Projeto-IoT
```

### 2. Configurar credenciais Wi-Fi

Edite o arquivo:

```cpp
secrets.h
```

Exemplo:

```cpp
#define WIFI_SSID "SuaRede"
#define WIFI_PASSWORD "SuaSenha"
```

### 3. Configurar MQTT

Defina:

• Endereço do broker
• Porta
• Usuário
• Senha
• Tópico de comunicação

### 4. Compilar e gravar

Utilize:

• Arduino IDE
• PlatformIO

Envie o firmware para o ESP-32.

---

## Estrutura do Projeto

```text
src/
├── main.cpp
├── MQTTManager.cpp
├── MQTTManager.h
├── WiFiManager.cpp
├── WiFiManager.h
├── DebugManager.cpp
├── DebugManager.h
├── IRManager.cpp
├── IRManager.h

include/
├── secrets.h

README.md
```

---

## Objetivos do Projeto

• Aplicar conceitos de IoT
• Utilizar comunicação MQTT
• Integrar sistemas embarcados
• Controlar dispositivos físicos através de software
• Desenvolver uma solução escalável para automação residencial

---

## Equipe

| Integrantes | Responsabilidades |
|------------|------------------|
| Davi Nogueira | Desenvolvimento do sistema |
| Fellipe Simon | Integração e interface |
| Heitor Barreto | Documentação e testes |
| Gabriel Expindola | Comunicação IR |
| Gabriel Bocchino | Comunicação MQTT e hardware |

---

## Conceitos Aplicados

• Internet das Coisas (IoT)
• Sistemas Embarcados
• MQTT
• Redes de Computadores
• Comunicação sem fio
• JSON
• Automação Residencial
• Programação em C++

---

## Licença

Este projeto foi desenvolvido para fins acadêmicos e educacionais.

---

<div align="center">

### Smart AC Control

Controle inteligente de ar-condicionado utilizando ESP32, MQTT e comunicação infravermelha.

⭐ Se este projeto foi útil para você, considere deixar uma estrela no repositório!

</div>
