
#include"Sensores.h"
#include"Data.h"
#include"thread2.h"

Sensores sensores = Sensores(pinDHT,pinRelogio);
Data data = Data();
Condicoes condicoes = Condicoes();

void setup(){
  sensores.begin();
  data.begin();
  thread2_setup();
}



void loop(){ // Thread 1
  while(pausa()) // pausar thread 1
    delay(10);
  
  lerSensor( sensores , data ); // read sensor data
  ativar(verificarCondicoes(  Data * read, Condicoes * condicoes )); // 
  delay(10);
}
