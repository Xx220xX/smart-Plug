//
// Created by PC on 05/08/2019.
//

#ifndef CPP_COMUNICACQAO_H
#define CPP_COMUNICACQAO_H
#define GLOBAL_

#define SMT_ID_WIFI 0
#define SMT_ID_SERIAL 1
#define SMT_ID_BLUETOOTH 2

class Comunicacao {
protected:
    Thread myThread;
    int id;
public:

    void pause() { myThread.pause(); };

    void resume() { myThread.resume(); };

    virtual void sendMensage(char *) {};

    virtual void start() {};

/**
 * Deve iniciar as configuracoes necessarias e tambem instanciar o objeto myThread
 *
 */
    virtual void init(char *_name, char *_pass, char *_mqtt_server = nullptr, int port = 0) { throw; };//for wifi
    virtual void init(int veloc) {};//for serial
    virtual void init(char *deviceName) {};//for bluetooth
public:
    static Comunicacao *comunicacoes[3];
    static bool use_pause;

    static void PAUSE_ALL(int ID_REQUEST) {
        while (use_pause)delay(100);
        use_pause = true;
        for (int i = 0; i < 3; i++)
            if (i != ID_REQUEST)
                comunicacoes[i]->pause();
        use_pause = false;
    }

    static void RESUME_ALL() {
        for (int i = 0; i < 3; i++)
            comunicacoes[i]->resume();
    }

    static void
    init_ALL(int veloc, char *name_device, char *name_wifi, char *pass_wifi, char *mqtt_server, int mqtt_port) {
#ifndef SMT_WIFI_DISABLE
        comunicacoes[0]->init(name_wifi, pass_wifi, mqtt_server, mqtt_port);
        comunicacoes[0]->start();
#endif
#ifndef SMT_SERIAL_DISABLE
        comunicacoes[1]->init(veloc);
        comunicacoes[1]->start();
#endif
#ifndef SMT_BLUETOOTH_DISABLE
        comunicacoes[2]->init(name_device);
        comunicacoes[2]->start();
#endif
    }

};
Comunicacao defaultz_;
Comunicacao *Comunicacao::comunicacoes[3]={&defaultz_,&defaultz_,&defaultz_};
bool Comunicacao::use_pause = false;

GLOBAL_ void received_mensagem(Comunicacao *self, char *msg);

#ifndef SMT_WIFI_DISABLE
#include "WIFI/SMT_Wifi.h"
MEU_WIFI smt_cl_wifi;
#endif

#ifndef SMT_SERIAL_DISABLE
#include "Serial/SMT_Serial.h"
MEU_SERIAL smt_cl_serial;
#endif

#ifndef SMT_BLUETOOTH_DISABLE
#include "Bluetooth/SMT_Bluetooth.h"
MEU_BlUETOOTH smt_cl_bluetooth;
#endif

#endif //CPP_COMUNICACQAO_H
