//
// Created by PC on 05/08/2019.
//

#ifndef CPP_COMUNICACQAO_H
#define CPP_COMUNICACQAO_H
#define GLOBAL_
#define ID_WIFI 1
#define ID_SERIAL 2
#define ID_BLUETOOTH 3

//incrementado na biblioteca wifi
void wifi_send_msg(char *msg);
void wifi_pause();
void wifi_resume();

// incrementado na biblioteca Serial
void Serial_send_msg(char *msg);
void serial_pause();
void serial_resume();

// incrementado na biblioteca bluetooth
void Bluetooth_send_msg(char *msg);
void bluetooth_pause();
void bluetooth_resume();
/**
 * interpreta a mensagem e ao final despausara as comunicacoes
 *
 * @param ID identificador da comunicao
 * @param msg mensagem recebida
 *
 */
GLOBAL_ void onReceived_mensagem(int ID, char *msg);
GLOBAL_ void COMUNICACAO_sendMsg(int ID, char * msg);
GLOBAL_ void COMUNICACAO_PAUSE(int ID_REQUEST);
GLOBAL_ void COMUNICACAO_RESUME();
GLOBAL_ char mensagem[500];

#include "WIFI/Wifi.h"
#include "Serial/Serial.h"
#include "Bluetooth/Bluetooth.h"

void COMUNICACAO_init(char *name_device, char *name_wifi, char *pass_wifi, char *mqtt_server, int mqtt_port) {
    SMT_WIFI_CONFIG(name_wifi, pass_wifi, mqtt_server, mqtt_port);
    BlUETOOTH_CONFIG(name_device);
    SMT_SERIAL_INIT(115200);

    SERIAL_START();
    WIFI_START();
    BLUETOOTH_START();
}

bool use_pause = false;
void COMUNICACAO_PAUSE(int ID_REQUEST){
    while(use_pause) delay(100);
    use_pause = true;
    if(ID_REQUEST!=ID_WIFI)
        wifi_pause();
    if(ID_REQUEST!= ID_SERIAL)
        serial_pause();
    if(ID_REQUEST!=ID_BLUETOOTH)
        bluetooth_pause();
    use_pause = false;
}

GLOBAL_ void COMUNICACAO_sendMsg(int ID, char *msg){
    switch (ID){
        case ID_WIFI:
            wifi_send_msg(msg);
            break;
        case ID_SERIAL:
            Serial_send_msg(msg);
            break;
        case ID_BLUETOOTH:
            Bluetooth_send_msg(msg);
    }
}
GLOBAL_ void COMUNICACAO_RESUME(){
    wifi_resume();
    serial_resume();
    bluetooth_resume();
}

#endif //CPP_COMUNICACQAO_H
