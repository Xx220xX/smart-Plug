//
// Created by PC on 02/08/2019.
//

#ifndef CPP_COMMANDS_H
#define CPP_COMMANDS_H



// executa os comandos recebidos pela conexao atual , atua em uma tread separada quando recebe os comandos pausa tds as outras threads e ao fim ela é destruida
// modifica o indentificador de tarefa a ser executada e começa a atuar na nova tarefa .

/**
   @param mensage
        esta é a mensagem recebida contendo o comando e os parametros necessario para sua atuacao
        os comandos sao :

        setWifi: name = [[wifi_name]] password[[pass]]
        setTime: dd/mm/aa hr/min/seg
        getTime:
        getWifi:
        setServer: [[(ainda desconhecidos os parametros)]]
        getTask:(este retorna a tarefa atual que esta sendo executada sendo )

        despertador: hr:min:seg seg_de_atuacao [[(dias da semana| vazio para tds os dias)]]
        periodo: hora  inicial = hr:min:seg ,hora final hr:min:seg  [[(dias da semana| vazio para tds os dias)]]
        blink: seg
        setPlug: (1|0)
        ligar_se_humidade_entre: minimo = (min) maximo = (max)
        ligar_se_temperatura_menor_que: max = (max)
        ligar_se_temperatura_maior_que: min = (min)



*/
#ifndef SMT_DEBUG_COMANDS
#define SMT_PRINT(msg)
#define SMT_PRINTLN(msg)
#else
#define SMT_PRINT(msg) Serial.print(msg);
#define SMT_PRINTLN(msg) Serial.println(msg);


#endif

Thread show;

/***
     * @param *p: deve conter as condiçoes que sera chamado de @c;
     * Envia uma mensagem periodicamente, o tempo é definido por @c.millis
     * caso @c.hora então enviará a temperatura
     * caso @c.minuto então enviará a humidade
     * caso @c.segundo então enviará a hora da tomada
     */
void fshow(void *p) {
    Condicoes c = *((Condicoes *) p);
    for (;;) {
        if (c.hora) {
            Serial.println(Sensor.getTemperature());
        }
        if (c.minuto)
            Serial.println(Sensor.getHumidy());
        if (c.segundo)
            Serial.println(time_tostr());
        delay(c.millis);
    }
}

/**
 *
 * @param self : correspode a quem enviou a mensagem
 * @param msg : a mensagem recebida
 *  os comando sao interpretados e entao é criado uma tarefa em segundo plano caso necessario
 *  | Comando | arg1  | arg2 | arg3 | arg4 | arg5 | arg6 | ação | possui segundo plano |
 *  |  ----   | ----  | ---- | ---- | ---- | ---- | ---- | ---- |         ----         |
 *  | get time | - | - | - | -  | - | -  | retorna a hora | não   |
 *  | inf all | tomada(int)  | - | - |  - | - |  - | @deprecated retorna humidade, hora  e tomada está ligado ou deslida | não |
 *  |  info process   | -  | - | - | - | - | - | retorna informacoes da tarefa sendo realizada pela tomada | não |
 *  |  get humidity   | -  | - | - | - | - | - | retorna a humida lida pelo sensor | não |
 *  |  get temperature   | -  | - | - | - | - | - | retorna a temperatura lida pelo sensor | não |
 *  |  -   | -  | - | - | - | - | - | - | - |
 *  |  blink   | tomada(int)  | milissegundos(int) | - | - | - | - | alterna entre ligado e desligado no periodo de 2 vezes o tempo passado | sim |
 *  |  off   | tomada(int)  | - | - | - | - | - | desliga a tomada | não |
 *  |  on   | tomada(int)  | - | - | - | - | - | liga a tomada | não |
 *  |  alarmae   | tomada(int)  | hora(int) | minuto(int) | segundo(int) | dias da semana(string) | periodo em millis para ficar ligado(int) | liga a tomada | não |
 *  |  refrigera   | tomada(int)  | temperatura maxima aceitavel(float)  | - | - | - | - | liga a tomada caso a temperatura do sensor supere a passsada | sim  |
 *  |  aquece   | tomada(int)  | temperatura minima aceitavel(float)  | - | - | - | - | liga a tomada caso a temperatura do sensor fique menor que  a temperatura passada passsada | sim  |
 *  |  humidade entre   | tomada(int)  | humidade minima(float)  | humidade maxima(float) | - | - | - | liga a tomada caso a humidade esteja entre os valores min e max | sim  |
 *  |  on for   | tomada(int)  | milissegundos(int)  | - | - | - | - | liga a tomada pelo tempo enviado depois muda para off | sim  |
 *
 */
GLOBAL_ void received_mensagem(Comunicacao *self, char *msg) {
    SMT_PRINTLN("recebi mensagem: ");
    int id = 0;
    Condicoes condicoes = {0};
    if (loadCommand(msg, "get time", "d", &id)) {
        SMT.answer(self, id, SMT_REQUEST_GET_TIME);
    } else if (loadCommand(msg, "info all", "d", &id)) {
        SMT.answer(self, id, SMT_REQUEST_GET_ALL);
    } else if (loadCommand(msg, "info process", "d", &id)) {
        SMT.answer(self, id, SMT_REQUEST_GET_CURRENT_TASK);
    } else if (loadCommand(msg, "get humidity", "d", &id)) {
        SMT.answer(self, id, SMT_REQUEST_GET_HUMIDITY);
    } else if (loadCommand(msg, "get temperature", "d", &id)) {
        SMT.answer(self, id, SMT_REQUEST_GET_TEMPERATURE);
    }

        /** atuacao*/
    else if (loadCommand(msg, "blink", "d d", &id, &condicoes.millis)) {
        SMT.set_task(SMT_ID_TASK_BLINK, id, condicoes);
    } else if (loadCommand(msg, "off", "d", &id)) {
        SMT.set_task(SMT_ID_TASK_EVER_OFF, id, condicoes);
    } else if (loadCommand(msg, "on", "d", &id)) {
        SMT.set_task(SMT_ID_TASK_EVER_ON, id, condicoes);
    } else if (loadCommand(msg, "alarme", "d d:d:d s[7] d", &id, &condicoes.hora, &condicoes.minuto, &condicoes.segundo,
                           condicoes.wek, &condicoes.periodo)) {
        SMT.set_task(SMT_ID_TASK_ALARM, id, condicoes);
    } else if (loadCommand(msg, "refrigera", "d f", &id, &condicoes.max)) {
        SMT.set_task(SMT_ID_TASK_TEMPERATURA_MAIOR, id, condicoes);
    } else if (loadCommand(msg, "aquece", "d f", &id, &condicoes.min)) {
        SMT.set_task(SMT_ID_TASK_TEMPERATURA_MENOR, id, condicoes);
    } else if (loadCommand(msg, "humidade entre", "d f f", &id, &condicoes.min, &condicoes.max)) {
        SMT.set_task(SMT_ID_TASK_HUMIDADE_ENTRE, id, condicoes);
    } else if (loadCommand(msg, "on for", "d d", &id, &condicoes.millis)) {
        SMT.set_task(SMT_ID_ON_FOR, id, condicoes);
    } else if (loadCommand(msg, "info", "d d d d", &condicoes.millis, &condicoes.hora, &condicoes.minuto,
                           &condicoes.segundo)) {
        show.stop();
        show = Thread();
        if (condicoes.millis == 0) {
            Serial.println("parando info");
        } else {
            Serial.println("iniciando info");
            show = Thread("show", fshow, 4000);
            show.start(&condicoes);
        }
    }
            /* conversa*/
    else {
        SMT.answer(self, 0, SMT_REQUEST_GET_TALK, "ERROR 404: COMMAND NOT FOUND\n", msg);
    }

    Comunicacao::RESUME_ALL();
}

#endif //CPP_COMMANDS_H
