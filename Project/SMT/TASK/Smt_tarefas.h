//
// Created by PC on 02/08/2019.
//
#ifndef CPP_SMT_TAREFAS_H
#define CPP_SMT_TAREFAS_H

#define  MAX_PLUG 3
/** debug*/
#ifndef SMT_DEBUG_TASK
#define SMT_PRINT(msg)
#define SMT_PRINTLN(msg)
#else
#define SMT_PRINT(msg) Serial.print(msg);
#define SMT_PRINTLN(msg) Serial.println(msg);
#endif

/*** Requerimentos**/
#define  SMT_REQUEST_GET_TIME 0
#define  SMT_REQUEST_GET_TALK 1
#define  SMT_REQUEST_GET_CURRENT_TASK 2
#define  SMT_REQUEST_GET_SENSOR 3
#define  SMT_REQUEST_GET_ALL 4
#define  SMT_REQUEST_GET_HUMIDITY 5
#define  SMT_REQUEST_GET_TEMPERATURE 6


/** tarefas */
#define  SMT_ID_TASK_EVER_OFF 0
#define  SMT_ID_TASK_EVER_ON 1
#define  SMT_ID_TASK_BLINK 2
#define  SMT_ID_TASK_ALARM 3
#define  SMT_ID_TASK_TEMPERATURA_MENOR 4
#define  SMT_ID_TASK_TEMPERATURA_MAIOR 5
#define  SMT_ID_TASK_HUMIDADE_ENTRE 6
#define  SMT_ID_ON_FOR 7

class Condicoes {
public:
    float min, max;
    int hora, minuto, segundo, millis;
    char wek[8];
    int periodo;
};

class PLUG {
    int pin;
    Condicoes condicoes;
public:
    int id_task = SMT_ID_TASK_EVER_OFF;
    Thread myThread;
    char name[11];

    PLUG() {
        this->pin = -1;
    }

    PLUG(char *name, int pin) {
        this->begin(pin);
        snprintf(this->name, 10, "%s", name);
    }

    void liga(bool on) {
        digitalWrite(pin, on);
    }

    bool ligado() {
        return digitalRead(this->pin);
    }

    void begin(int pin) {
        this->pin = pin;
        pinMode(this->pin, OUTPUT);
        liga(false);
    }

    void setCondicoes(Condicoes condicoes) {
        this->condicoes = condicoes;
    }

    static void blink(void *pTASK) {
        PLUG *c = (PLUG *) pTASK;
        for (;;) {
            c->liga(!c->ligado());
            delay(c->condicoes.millis);
        }
    }

    static void on_for(void *pTASK) {
        PLUG *c = (PLUG *) pTASK;
        c->liga(true);
        Serial.println("LIGUEI");
        Serial.println(c->condicoes.millis);
        delay(c->condicoes.millis);
        c->liga(false);
        c->id_task = SMT_ID_TASK_EVER_OFF;
        Serial.println("DESLIGUEI");
        c->stop();

    }

    static void refri(void *pTASK) {
        PLUG *c = (PLUG *) pTASK;
        float temp = 0;
        for (;;) {
            temp = Sensor.getTemperature();
            if (temp > c->condicoes.max) {
                c->liga(true);
            } else {
                c->liga(false);
            }
            delay(1000);
        }
    }

    static void aquece(void *pTASK) {
        PLUG *c = (PLUG *) pTASK;
        float temp = 0;
        for (;;) {
            temp = Sensor.getTemperature();
            if (temp < c->condicoes.min) {
                c->liga(true);
            } else {
                c->liga(false);
            }
            delay(1000);
        }
    }

    static void entre(void *pTASK) {
        PLUG *c = (PLUG *) pTASK;
        float humidity = 0;
        for (;;) {
            humidity = Sensor.getHumidy();
            if (humidity < c->condicoes.min) {
                c->liga(true);// humidade baixa entao liga
            } else if (humidity > c->condicoes.max) {
                c->liga(false);//humidade alta entao desliga
            }
            delay(1000);
        }
    }

    static void periodo(void *pTASK) {
        PLUG *c = (PLUG *) pTASK;
        // condicoes.wek == '12345670
        //arrumar dias da semana
        int i;
        for (i = 0; c->condicoes.wek[i]; i++) {
            if (c->condicoes.wek[i] > '0' && c->condicoes.wek[i] <= '7') {
                c->condicoes.wek[i] = c->condicoes.wek[i] - '0' - 1;
            } else {
                c->condicoes.wek[i] = -1;//dia invalido
            }
        }
        c->condicoes.wek[i] = -2;//fim
        /**
         * se é o dia da semana escolhido entao ve se a hora atual é maior que inicial liga por n segundos
         */
        SMT_PRINTLN("cheguei aqui linha 98 tarefas")
        struct tm tempoAtual = getDataTime();
        bool liguei = false;
        c->liga(false);
        for (;;) {
            for (i = 0; i < 7; i++) {
                if (tempoAtual.tm_wday == c->condicoes.wek[i]) {
                    SMT_PRINTLN("HOJE eu vou  ligar")
                    if (tempoAtual.tm_hour * 3600 + tempoAtual.tm_min * 60 + tempoAtual.tm_sec >
                        c->condicoes.hora * 3600 + c->condicoes.minuto * 60 + c->condicoes.segundo && !liguei) {
                        // aki eu devo ligar
                        SMT_PRINTLN("eu devo ligar agr :D")
                        liguei = true;
                        c->liga(true);
                        delay(c->condicoes.periodo);
                        c->liga(false);
                    }
                    break;
                }
            }
            delay(1000);
            tempoAtual = getDataTime();
        }
    }

    void start() {
        this->myThread.start(this);
    }

    void stop() {
        this->myThread.stop();
        this->myThread = Thread();
    }

    void tostring(char *bf, int size) {
        switch (id_task) {
            case SMT_ID_TASK_BLINK:
                snprintf(bf, size, "%s ->BLINK tempo : %d ms", this->name, this->condicoes.millis);
                break;
            case SMT_ID_TASK_EVER_ON:
                snprintf(bf, size, "%s ->Ever on", this->name);
                break;
            case SMT_ID_TASK_EVER_OFF:
                snprintf(bf, size, "%s ->Ever off", this->name);
                break;
            case SMT_ID_TASK_HUMIDADE_ENTRE:
                snprintf(bf, size, "%s ->humidade ente (%f , %f)", this->name, this->condicoes.min,
                         this->condicoes.max);
                break;
            case SMT_ID_TASK_TEMPERATURA_MENOR:
                snprintf(bf, size, "%s -> refrigera mantem T < %f", this->name, this->condicoes.max);
                break;
            case SMT_ID_TASK_TEMPERATURA_MAIOR:
                snprintf(bf, size, "%s -> refrigera mantem T > %f", this->name, this->condicoes.min);
                break;
            case SMT_ID_ON_FOR:
                snprintf(bf, size, "%s -> ligado por %f ms", this->name, this->condicoes.millis);
                break;

        }
    }


};

class {
public:
    void answer(Comunicacao *comunicacao, int plug_id, int REQUEST, const char *msg1 = "", const char *msg2 = "") {
        if (plug_id < 0 && plug_id >= all_plugs) {
            comunicacao->sendMensage((char *) "erro id plug error");
            return;
        }
        char *msg = (char *) calloc(sizeof(char), 500);
        switch (REQUEST) {
            case SMT_REQUEST_GET_TALK:
                snprintf(msg, 499, "%s %s\n", msg1, msg2);
                break;
            case SMT_REQUEST_GET_TIME:
                snprintf(msg, 499, "%s", time_tostr());
                break;
            case SMT_REQUEST_GET_CURRENT_TASK:
                this->plug[plug_id].tostring(msg, 499);
                break;
            case SMT_REQUEST_GET_HUMIDITY:
                snprintf(msg, 499, " %f ", Sensor.getHumidy());
                break;
            case SMT_REQUEST_GET_TEMPERATURE:
                snprintf(msg, 499, " %f ", Sensor.getTemperature());
                break;
            case SMT_REQUEST_GET_ALL:
                snprintf(msg, 499, " Temperatura:%f\nHumidade: %f\nTOMADA atualmente: %s\n",
                         Sensor.getHumidy(), Sensor.getHumidy(), this->plug[plug_id].ligado() ? "ligado" : "desligado");
                break;
        }
        comunicacao->sendMensage(msg);
        free(msg);
    }

    bool set_task(int TASK_ID, int plug_id, Condicoes condicoes) {
        if (plug_id < 0 || plug_id >= this->all_plugs) {
            Serial.println("falha id desconhecido");
            return true;
        }
        this->plug[plug_id].setCondicoes(condicoes);
        this->plug[plug_id].stop();
        switch (TASK_ID) {
            case SMT_ID_TASK_BLINK:
                this->plug[plug_id].id_task = SMT_ID_TASK_BLINK;
                this->plug[plug_id].myThread = Thread( "PLUG BLINK",PLUG::blink, 2048);
                this->plug[plug_id].start();
                break;
            case SMT_ID_TASK_EVER_ON:
                this->plug[plug_id].id_task = SMT_ID_TASK_EVER_ON;
                this->plug[plug_id].liga(true);
                break;
            case SMT_ID_TASK_EVER_OFF:
                SMT_PRINTLN("modo off ")
                this->plug[plug_id].id_task = SMT_ID_TASK_EVER_OFF;
                this->plug[plug_id].liga(false);
                break;
            case SMT_ID_TASK_ALARM:
                SMT_PRINTLN("ativando alarme")
                this->plug[plug_id].id_task = SMT_ID_TASK_ALARM;
                this->plug[plug_id].myThread = Thread("PLUG ALARM",PLUG::periodo, 2048);
                this->plug[plug_id].start();
                break;
            case SMT_ID_TASK_TEMPERATURA_MAIOR:
                SMT_PRINTLN("ativando refrigera")
                this->plug[plug_id].id_task = SMT_ID_TASK_TEMPERATURA_MAIOR;
                this->plug[plug_id].myThread = Thread( "PLUG refrigera",PLUG::refri, 2048);
                this->plug[plug_id].start();
                break;
            case SMT_ID_TASK_TEMPERATURA_MENOR:
                SMT_PRINTLN("ativando aquece")
                this->plug[plug_id].id_task = SMT_ID_TASK_TEMPERATURA_MENOR;
                this->plug[plug_id].myThread = Thread( "PLUG refrigera",PLUG::aquece, 2048);
                this->plug[plug_id].start();
                break;
            case SMT_ID_TASK_HUMIDADE_ENTRE:
                SMT_PRINTLN("ativando por humidade entre")
                this->plug[plug_id].id_task = SMT_ID_TASK_HUMIDADE_ENTRE;
                this->plug[plug_id].myThread = Thread("PLUG humidity",PLUG::entre,  2048);
                this->plug[plug_id].start();
                break;
            case SMT_ID_ON_FOR:
                SMT_PRINTLN("ativando ligado por")
                if (condicoes.millis<=0){
                    SMT_PRINTLN("parametro invalido voltando para ever off")
                    this->plug[plug_id].id_task = SMT_ID_TASK_EVER_OFF;
                    this->plug[plug_id].liga(false);
                    break;
                }
                this->plug[plug_id].id_task = SMT_ID_ON_FOR;
                this->plug[plug_id].myThread = Thread( "PLUG on for",PLUG::on_for, 2048);
                this->plug[plug_id].start();
                break;
        }
        return false;
    }

    int all_plugs = 0;
    PLUG plug[MAX_PLUG];

    void addPlug(PLUG plug) {
        if (this->all_plugs < MAX_PLUG) {
            this->plug[this->all_plugs++] = plug;
        }
    }


} SMT;


#endif //CPP_SMT_TAREFAS_H
