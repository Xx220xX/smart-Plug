//
// Created by PC on 02/08/2019.
//

#ifndef CPP_TASK_H
#define CPP_TASK_H

#include <cstdarg>

enum TASK : int {
    blink = 0,
    despertador,
    periodo,
    temperatura_menor,
    temperatura_maior,
    EVER_OFF,
    EVER_ON
};
enum SMT : int {
    TIME = 0,
    WIFI,
    CURRENT_TASK,
    TALK
};

class SMT_DATA_TASK {
    int pin;
    bool ligado = false;
    struct {
        int hr, min, seg, segundsAtuante;
        char days[11];
    } despertador;
    struct {
        int seconds;
    } blink;
    struct {
        float max, min;
    } TEMPERATURA;
public:
    TASK task_atual = TASK::EVER_OFF;
    Thread lastTask;

    void toString(char *string, int size);

    void begin(int pin) {
        this->pin = pin;
        pinMode(this->pin, OUTPUT);
    }

    void setTaskTO_DESPERTADOR(int hr, int min, int seg, int seg_atuante, char *dayOfWeek);

    void setTaskTO_BLINK(int seg);

    void setTaskTO_EVERY_ON();

    void setTaskTO_EVERY_OFF();

    void FUNCTION_blink();

    void setPower(bool b);

    void FUNCTION_despertador();
};


SMT_DATA_TASK SMT_task;

void SMT_ANSWER_(int ID, int REQUEST, const char *msg1 = "", const char *msg2 = "");

void SMT_SET_TASK(TASK task, ...);

void SMT_ANSWER_(int ID, int REQUEST, const char *msg1, const char *msg2) {
    char *msg = (char *)calloc(sizeof(char),500);
    switch (REQUEST) {
        case SMT::TALK:
            snprintf(msg, 499, "%s %s\n", msg1, msg2);
            COMUNICACAO_sendMsg(ID, msg);
            break;
        case SMT::WIFI:

            snprintf(msg, 499, "name: %s\npass: %s\nmqqt_server: %s\nmqtt_port:%d\n",
                     WIFI_getName(), WIFI_getPass(), WIFI_getMqttServer(), WIFI_getMqttPort());
            COMUNICACAO_sendMsg(ID, msg);
        case SMT::CURRENT_TASK:
            //envia a tarefa atual
            SMT_task.toString(mensagem, 500);
            COMUNICACAO_sendMsg(ID, msg);
            break;
        case SMT::TIME:
            //envia o tempo do microcontrolador
            break;
    }
    free(msg);
}

void SMT_SET_TASK(TASK task, ...) {
    va_list args;
    va_start(args, task);

    switch (task) {
        case despertador:

            break;

        case blink:
            SMT_task.setTaskTO_BLINK(va_arg(args, int));
            break;
        case periodo:
            break;
        case temperatura_menor:
            break;
        case temperatura_maior:
            break;
        case EVER_OFF:
            SMT_task.setTaskTO_EVERY_OFF();
            break;
        case EVER_ON:
            SMT_task.setTaskTO_EVERY_ON();

            break;
    }
    va_end(args);
};

void SMT_PLUG_start(int pin) {
    SMT_task.begin(pin);
}



/*** @class SMT_DATA_TASK **/



void SMT_DATA_TASK::toString(char *string, int size) {

}

void _FUNCTION(void *param) {
    TASK  next_task =  ((SMT_DATA_TASK *) param)->task_atual;
    switch (next_task){
        case TASK::EVER_ON:
            break;
        case blink:
            ((SMT_DATA_TASK *) param)->FUNCTION_blink();
            break;
        case despertador:
            ((SMT_DATA_TASK *) param)->FUNCTION_despertador();
            break;
        case periodo:
            break;
        case temperatura_menor:
            break;
        case temperatura_maior:
            break;
        case EVER_OFF:
            break;
    }
    ((SMT_DATA_TASK *) param)->FUNCTION_blink();
}

void SMT_DATA_TASK::setTaskTO_BLINK(int seg) {
    this->blink.seconds = seg;
    if (this->task_atual == TASK::blink)return;
    this->lastTask.stop();
    this->lastTask = Thread(_FUNCTION, "TASK_BLINK");
    this->lastTask.start((void *) this);
}

void SMT_DATA_TASK::FUNCTION_blink() {
    for (;;) {
        this->setPower(!this->ligado);
        delay(this->blink.seconds);
    }
}

void SMT_DATA_TASK::setPower(bool b) {
    digitalWrite(this->pin, b);
    this->ligado = b;
}

void SMT_DATA_TASK::setTaskTO_EVERY_ON() {
    this->task_atual = EVER_ON;
    this->lastTask.stop();
    this->lastTask = Thread();
    this->setPower(true);
}

void SMT_DATA_TASK::setTaskTO_EVERY_OFF() {
    this->task_atual = EVER_OFF;
    this->lastTask.stop();
    this->lastTask = Thread();
    this->setPower(false);
}

void SMT_DATA_TASK::setTaskTO_DESPERTADOR(int hr, int min, int seg, int seg_atuante, char *dayOfWeek) {
    this->despertador.hr=hr;
    this->despertador.min=min;
    this->despertador.seg=seg;
    this->despertador.segundsAtuante=seg_atuante;
    snprintf(this->despertador.days,11,"%s",dayOfWeek);
    if(this->task_atual== TASK::despertador)return;
    this->lastTask.stop();
    this->lastTask =Thread(_FUNCTION,"TASK_WAKE_UP");
    this->lastTask.start((void *) this);
}

void SMT_DATA_TASK::FUNCTION_despertador() {
    // precisa dos dados da hora
}

#endif //CPP_TASK_H
