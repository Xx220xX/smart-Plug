//
// Created by PC on 06/08/2019.
//
#ifndef CPP_BLUETOOTH_H
#define CPP_BLUETOOTH_H

#include <BluetoothSerial.h>

BluetoothSerial SerialBT;
struct {
    char name[40] = {0};
} BT__BLUETOOTH;
Thread BLUETOOTH_THREAD;

void BlUETOOTH_CONFIG(char *name_device);

void BLUETOOTH_START();

void bluetooth_loop(void *param);

void BlUETOOTH_CONFIG(char *name_device) {
    BLUETOOTH_THREAD.stop();
    strcpy(BT__BLUETOOTH, name_device);
    BLUETOOTH_THREAD = Thread(bluetooth_loop, "Bluetooth thread");
}

void BLUETOOTH_START() {
    BLUETOOTH_THREAD.start();
}

void bluetooth_loop(void *param) {
    SerialBT.begin(BT__BLUETOOTH.name);
    for (;;) {
        if (SerialBT.available()) {
            COMUNICACAO_PAUSE(ID_BLUETOOTH);
            int i = 0;
            while (SerialBT.available() && i < 499) {
                mensagem[i++] = SerialBT.read();
            }
            mensagem[i] = 0;
            onReceived_mensagem(ID_BLUETOOTH, mensagem);
        }
        delay(100);
    }
}

void Bluetooth_send_msg(char *msg){
    int i;
    for(i=0;msg[i];i++);
    if(SerialBT.hasClient()){
        SerialBT.write(msg,i);
    }
}
void bluetooth_pause();
void bluetooth_resume();

#endif //CPP_BLUETOOTH_H
