#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <ir_Fujitsu.h>

struct EstadoAr {
  bool ligado;
  uint8_t temperatura;
  uint8_t modo;
  uint8_t ventilacao;
  uint8_t swing;
};

const uint16_t PINO_IR = 4;

IRFujitsuAC ac(PINO_IR);

EstadoAr estadoAtual = {
  true,
  24,
  kFujitsuAcModeCool,
  kFujitsuAcFanHigh,
  kFujitsuAcSwingOff
};

void aplicarEstado(EstadoAr estado) {

  ac.setModel(ARRAH2E);

  ac.setTemp(estado.temperatura);

  ac.setMode(estado.modo);

  ac.setFanSpeed(estado.ventilacao);

  ac.setSwing(estado.swing);

  if (estado.ligado) {
    ac.setCmd(kFujitsuAcCmdTurnOn);
  }
  else {
    ac.setCmd(kFujitsuAcCmdTurnOff);
  }

  ac.send();

  Serial.println(ac.toString());
}

void setup() {

  Serial.begin(115200);

  ac.begin();

  delay(1000);

  aplicarEstado(estadoAtual);
}

void loop() {

}