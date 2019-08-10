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


GLOBAL_ void received_mensagem(Comunicacao *self, char *msg) {
    SMT_PRINTLN("recebi mensagem: ");
    int id = 0;
    Condicoes condicoes;
    if (loadCommand(msg, "getTime", "d", &id)) {
        SMT_PRINTLN("solicitou getTime")
        SMT.answer(self, id, SMT_REQUEST_GET_TIME);
    } else if (loadCommand(msg, "info all", "d", &id)) {
        SMT_PRINT("solicitou info all")
        SMT.answer(self, id, SMT_REQUEST_GET_ALL);
    } else if (loadCommand(msg, "info task", "d", &id)) {
        SMT_PRINT("solicitou info task")
        SMT.answer(self, id, SMT_REQUEST_GET_CURRENT_TASK);
    }

        /** atuacao*/
    else if (loadCommand(msg, "blink", "d d", &id, &condicoes.millis)) {
        SMT.set_task(SMT_ID_TASK_BLINK, id, condicoes);
    } else if (loadCommand(msg, "off", "d", &id)) {
        SMT.set_task(SMT_ID_TASK_EVER_OFF, id, condicoes);
    } else if (loadCommand(msg, "on", "d", &id)) {
        SMT.set_task(SMT_ID_TASK_EVER_ON, id, condicoes);
    }else if (loadCommand(msg, "alarme", "d d:d:d s[7] d", &id,&condicoes.hora,&condicoes.minuto,&condicoes.segundo,condicoes.wek,&condicoes.periodo)) {
        SMT_PRINTLN("requisitou alarme")
        SMT.set_task(SMT_ID_TASK_ALARM, id, condicoes);
    }
    /* conversa*/
    else {
        SMT.answer(self, 0, SMT_REQUEST_GET_TALK, "ERROR 404: COMMAND NOT FOUND\n", msg);
    }

    Comunicacao::RESUME_ALL();
}

#endif //CPP_COMMANDS_H
