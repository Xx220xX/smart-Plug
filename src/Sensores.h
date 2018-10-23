
#include "Wire.h"
#include "DHT.h"
#include "RTClib.h"
//#include"Tipos.h"

#define DHTTYPE DHT22

DHT dht(DHTPin, DHTTYPE);

RTC_DS3231 rtc;


void inciarSensores() {
  rtc.begin();
  delay(2000);
}

char lerDht(DadosSensores *meuSensor) {
  meuSensor->hum = dht.readHumidity();
  meuSensor->tempCels = dht.readTemperature();
  meuSensor->tempFar = dht.readTemperature(true);
  if (isnan(meuSensor->hum) || isnan(meuSensor->tempCels) || isnan(meuSensor->tempFar)) {
    return 1;
  }
  return 0;
}

void lerTempo(DadosSensores *meuSensor) {
  DateTime now = rtc.now();
  meuSensor->horaAtual.dia = now.day();
  meuSensor->horaAtual.hora = now.hour();
  meuSensor->horaAtual.mes = now.month();
  meuSensor->horaAtual.ano = now.year();
  meuSensor->horaAtual.minu = now.minute();
  meuSensor->horaAtual.seg = now.second();
  meuSensor->horaAtual.realT = now.unixtime();

}

void printaRDados(DadosSensores meuSensor) {
  Serial.print("temperatura ");
  Serial.println(meuSensor.tempCels);
  Serial.print("Humidade ");
  Serial.println(meuSensor.hum);
  Serial.println("Hora atual");
  Serial.print(meuSensor.horaAtual.dia);
  Serial.print("/");
  Serial.print(meuSensor.horaAtual.mes);
  Serial.print("/");
  Serial.print(meuSensor.horaAtual.ano);
  Serial.print(" ");
  Serial.print(meuSensor.horaAtual.hora);
  Serial.print(":");
  Serial.print(meuSensor.horaAtual.minu);
  Serial.print(":");
  Serial.print(meuSensor.horaAtual.seg);
  Serial.println(" ");
}


char lerSensores(DadosSensores *dadosSensores) {
  lerTempo(dadosSensores);
  return lerDht(dadosSensores);
}
