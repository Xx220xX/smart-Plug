//
// Created by PC on 06/08/2019.
//
#ifndef CPP_SMT_BLUETOOTH_H
#define CPP_SMT_BLUETOOTH_H

#include <BluetoothSerial.h>
class MEU_BlUETOOTH : public Comunicacao {
    BluetoothSerial SerialBT;
    char name[20];
    static MEU_BlUETOOTH * me;
    static void bluetooth_loop(void *p) {
        me->SerialBT.begin(me->name);
        char *mensagem;
        for (;;) {
            if (me->SerialBT.available()) {
                Comunicacao::PAUSE_ALL(SMT_ID_BLUETOOTH);
                int i = 0;
                mensagem = (char *) calloc(500, sizeof(char));
                while (me->SerialBT.available() && i < 499) {
                    mensagem[i++] = me->SerialBT.read();
                }
                mensagem[i] = 0;
                received_mensagem(me, mensagem);
                free(mensagem);
            }
            delay(100);
        }
    }

public:
    void init(char *_name) override {
        myThread = Thread( "BLUETOOTH THREAD",MEU_BlUETOOTH::bluetooth_loop);
        strcpy(name, _name);
    }
    void setName(char *name_) {
        strcpy(name, name_);
        SerialBT.begin(name);
    }
    void start() override {
        me = this;
        myThread.start();
    }
    void sendMensage(char *string) override {
        int i;
        for (i = 0; string[i]; i++);
        this->SerialBT.write((const unsigned char *) string, i);
    }

    MEU_BlUETOOTH()  {
        Comunicacao::comunicacoes[SMT_ID_BLUETOOTH] = this;
    }
};
MEU_BlUETOOTH * MEU_BlUETOOTH::me = nullptr;

#endif //CPP_SMT_BLUETOOTH_H
