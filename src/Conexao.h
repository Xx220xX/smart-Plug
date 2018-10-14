//tipos de conexao
#define NO_CONECTION 0
#define WIFI_TYPE 1
#define BLUETOOTH_TYPE 2

char haveBluetooth(char *typeConection){
  return 1;
}
char haveWifi(char *typeConection){
  return 0;
}
char enviarDadosBluetooth(DadosSensores minhaLeitura){
  Serial.println("Temperatura: "); 
  Serial.print("Celsios ");
  Serial.println(minhaLeitura.tempCels);
  Serial.print(" fahren h"); 
  Serial.println(minhaLeitura.tempFar);
  Serial.print("nHumidade: ");
  Serial.println(minhaLeitura.hum);
  Serial.println("__________________________________");
  return 1;
}
