//version: beta 1.0.0004
#include"A_SM_Object.h"
#include"A_SM_Sensores.h"
#include"A_SM_ConexaoBluetooth.h"


void gerenciadorDeSensores(void *p);

void gerenciadorConexaoBluetooth(void *p);

void setup() {
    Serial.begin(115200);
    pinMode(ledWarnRed, OUTPUT);
    pinMode(PIN_TO_PLUG, OUTPUT);
    condicoes.meuParametro=2;
    xTaskCreatePinnedToCore(
            gerenciadorDeSensores,   /* função que implementa a tarefa */
            "gerenciadosDeSensores", /* nome da tarefa */
            10000,      /* número de palavras a serem alocadas para uso com a pilha da tarefa */
            NULL,       /* parâmetro de entrada para a tarefa (pode ser NULL) */
            5,          /* prioridade da tarefa (0 a N) */
            NULL,       /* referência para a tarefa (pode ser NULL) */
            nucleo1);         /* Núcleo que executará a tarefa */

    delay(500); //tempoN_parametro4 para a tarefa iniciar
    xTaskCreatePinnedToCore(
            gerenciadorConexaoBluetooth,   /* função que implementa a tarefa */
            "gerenciadorConexaoBluetooth", /* nome da tarefa */
            10000,      /* número de palavras a serem alocadas para uso com a pilha da tarefa */
            NULL,       /* parâmetro de entrada para a tarefa (pode ser NULL) */
            5,          /* prioridade da tarefa (0 a N) */
            NULL,       /* referência para a tarefa (pode ser NULL) */
            nucleo0);         /* Núcleo que executará a tarefa */

    delay(500); //tempoN_parametro4 para a tarefa iniciar
}

void loop() {
    // gerenciadorwifi
    delay(10);

}

void gerenciadorDeSensores(void *p) {
    inciarSensores();
    int estadoAnterior = 0;
    char bugDht = 0;
    while (1) {
        if (pausa) {
            if (pausa == 2){
              delay(1);
              continue;}
            pausa = 0;
            delay(2000);
        }
        bugDht = lerSensores(&meusSensores);
        digitalWrite(ledWarnRed, bugDht); //acende caso dht falhe
       delay(50);
        TMD_controle(condicoes.meuParametro, &condicoes, &meusSensores, bugDht);
        if (ativa != estadoAnterior) {
            TMD_ativar(ativa);
            estadoAnterior = ativa;
        }
    }
}

void gerenciadorConexaoBluetooth(void *p) {
    iniciaBluetooth();
    BLUETOOTH_servicos();
}
