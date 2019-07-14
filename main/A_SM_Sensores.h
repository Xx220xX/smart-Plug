#ifndef SENSORES_H
#define SENSORES_H


#define DHTTYPE DHT22



DHT dht(DHTPin, DHTTYPE);


void inciarSensores() {
  rtc.Begin();
  rtc.SetIsRunning(true);

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
  if (!rtc.GetIsRunning()) {
    Serial.println("RTC nao estava rodando, starting now");
    rtc.SetIsRunning(true);
  }
  meusSensores.horaAtual = rtc.GetDateTime();
  /*  meuSensor->horaAtual.dia = now.day();
    meuSensor->horaAtual.hora = now.hour();
    meuSensor->horaAtual.mes = now.month();
    meuSensor->horaAtual.ano = now.year();
    meuSensor->horaAtual.minu = now.minute();
    meuSensor->horaAtual.seg = now.second();
    meuSensor->horaAtual.realT = now.Epoch32Time();*/
}

void printaRDados(String add, DadosSensores meuSensor) {
  String str = add;
  str += String("temperatura ");
  str += String(meuSensor.tempCels);
  str += String("\nHumidade ");
  str += String(meuSensor.hum);
  str += String("\nHora atual ");
  str += String(meuSensor.horaAtual.Day());
  str += String("/");
  str += String(meuSensor.horaAtual.Month());
  str += String("/");
  str += String(meuSensor.horaAtual.Year());
  str += String(" ");
  str += String(meuSensor.horaAtual.Hour());
  str += String(":");
  str += String(meuSensor.horaAtual.Minute());
  str += String(":");
  str += String(meuSensor.horaAtual.Second());
  str += String("\n");
  Serial.print(str);
  bluetooth_sendMsgString(str);
}


char lerSensores(DadosSensores *dadosSensores) {
  lerTempo(dadosSensores);
  return lerDht(dadosSensores);
}
#define countof(a) (sizeof(a) / sizeof(a[0]))

void printDateTime(const RtcDateTime& dt)
{
  char datestring[20];

  snprintf_P(datestring,
             countof(datestring),
             PSTR("%02u/%02u/%04u %02u:%02u:%02u"),
             dt.Month(),
             dt.Day(),
             dt.Year(),
             dt.Hour(),
             dt.Minute(),
             dt.Second() );
  Serial.print(datestring);
}
void ajustarHora(Tempo t) {
  Serial.println("ajustando hora\n");
  printDateTime(t);
  rtc.SetDateTime(t);
  Serial.println(" hora ajustada para \n");
  printDateTime(rtc.GetDateTime());
  delay(100);
}

#endif
