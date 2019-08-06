//
// Created by PC on 02/08/2019.
//

#ifndef CPP_TASK_H
#define CPP_TASK_H
enum {
    TASK_blink = 0,
    TASK_despertador,
    TASK_periodo,
    TASK_temperatura_menor,
    TASK_temperatura_maior
};
enum {
    SMT_TIME = 0,
    SMT_WIFI,
    SMT_CURRENT_TASK,
    SMT_TALK
};

void SMT_ANSWER_(int ID, int REQUEST, const char *msg1 = 0);


void SMT_ANSWER_(int ID, int REQUEST, const char *msg1) {
    switch (REQUEST) {
        case SMT_TALK:
            COMUNICACAO_sendMsg(ID, msg1);
            break;
        case SMT_WIFI:
            char msg[500];
            snprintf(msg,499,"name: %s\npass: %s\nmqqt_server: %s\nmqtt_port:%d",
                    WIFI_getName(),WIFI_getPass(),WIFI_getMqttServer(),WIFI_getMqttPort());
            COMUNICACAO_sendMsg(ID,msg);
        case SMT_CURRENT_TASK:
            //envia a tarefa atual
            break;
        case SMT_TIME:
            //envia o tempo do microcontrolador
            break;
    }
}


#endif //CPP_TASK_H
