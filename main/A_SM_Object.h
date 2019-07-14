//
// Created by hslhe on 10/02/2019.
//

#ifndef UNTITLED_OBJECT_H
#define UNTITLED_OBJECT_H

#include <Wire.h>
#include "RtcDS3231.h"


#include "DHT.h"
#include "BluetoothSerial.h"

#include "A_SM_Tipos.h"
#include "A_SM_Variaveis.h"
#include"A_SM_Pinagem.h"

void mensagens(const char *msg);

void pausar();

void despausar();

typedef long long int Long;

/*** tomada.h <init>*/

int TMD_getPower();

void TMD_ativar(int ativa);

void manterEntre(char *ativa, int min, int max, float atual);

void enviarParametros(String add, int modo);
void ajustarHora(Tempo t);
void
TMD_controle(int parametro, DadosParametro *myparamentros, DadosSensores *myDados, char sensorDesativado);

/***<end>*/

/**bluetooth.h<init>*/
void bluetooth_sendMsg(const char *msg);
void bluetooth_sendMsgString(String msg);
/***<end>*/

#include "A_SM_Tomada.h"

void mensagens(const char *msg) {
  bluetooth_sendMsg(msg);
}

void pausar() {
  pausa = 2;
}

void despausar() {
  pausa = 0;
}



#endif //UNTITLED_OBJECT_H
