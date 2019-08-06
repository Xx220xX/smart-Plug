//
// Created by PC on 05/08/2019.
//

#ifndef CPP_COMUNICACQAO_H
#define CPP_COMUNICACQAO_H
#define ID_WIFI 1
#define ID_SERIAL 1
#define ID_BLUETOOTH 1
void wifi_send_msg(char * msg);
void Serial_send_msg(char * msg);
void Bluetooth_send_msg(char * msg);
void received_mensagem(int ID,char *msg);
#include "WIFI/Wifi.h"
void COMUNICACAO_init(){

}


#endif //CPP_COMUNICACQAO_H
