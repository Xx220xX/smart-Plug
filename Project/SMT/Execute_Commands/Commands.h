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



#include<DataProcess.h> //https://github.com/Xx220xX/DataProcess
#include<Thread.h> //https://github.com/Xx220xX/ThreadEsp32
#include "../TASK/TASK.h"

#define COMMAND_IS(x, y)loadCommand(x,y,"")

void execute(int ID, char *mensage) {
    if (COMMAND_IS(mensage, "setWifi")) {
        char name[41];
        char pass[41];
        loadCommand(mensage, "setWifi", "s[40] s[40]", name, pass);
        SMT_WIFI_SET(name, pass);
        return;
    } else if (COMMAND_IS(mensage, "setTime")) {
        int d, m, a, hr, min, seg;
        loadCommand(mensage, "setTime", "d/d/d d/d/d", &d, &m, &a, &hr, &min, &seg);
//        SMT_TIME_SET(d,m,a,hr,min,seg);
    } else if (COMMAND_IS(mensage, "getTime")) {
        SMT_ANSWER_(ID, SMT_TIME);
    } else if (COMMAND_IS(mensage, "getTask")) {
        SMT_ANSWER_(ID, SMT_CURRENT_TASK);
    }

        //atuacao
    else if (COMMAND_IS(mensage, "despertador")) {
        int hr, min, seg;
        int segATUANTE;
        char diasAtuantes[11];
        /**
         * @param diasAtuais
         *  domindo é 1 sabado 7
         *  para segunda a sexta 123456
         *  para somente segunda 2
         *  segunda e quinta
         *  25
         *
         */
        loadCommand(mensage, "despertador", "d/d/d d s[10]", &hr, &min, &seg, &segATUANTE, diasAtuantes);
       // SMT_SET_TASK(TASK_despertador, hr, min, seg, segATUANTE, diasAtuantes);
    } else if (COMMAND_IS(mensage, "periodo")) {
        int ihr, imin, iseg;
        int fhr, fmin, fseg;
        char diasAtuantes[11];
        loadCommand(mensage, "periodo", "d/d/d d/d/d s[10]", &ihr, &imin, &iseg, &fhr, &fmin, &fseg, diasAtuantes);
       // SMT_SET_TASK(TASK_periodo, ihr, imin, iseg, fhr, fmin, fseg, diasAtuantes);
    } else {
        SMT_ANSWER_(ID, SMT_TALK, mensage);
        SMT_ANSWER_(ID,SMT_TALK,"command not found");
    }
}

#endif //CPP_COMMANDS_H
