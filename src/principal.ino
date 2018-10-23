//version: beta 1.0.0004
#include"Tipos.h"
#include"Variaveis.h"
#include"Pinagem.h"
#include"Sensores.h"
#include"ConexaoBluetooth.h"
#include"Tomada.h"


void gerenciadorDeSensores(void *p);
void gerenciadorConexaoBluetooth(void *p);

void setup() {
  Serial.begin(115200);
  pinMode(ledWarnRed, OUTPUT);
  pinMode(PIN_TO_PLUG , OUTPUT);
  xTaskCreatePinnedToCore(
    gerenciadorDeSensores,   /* função que implementa a tarefa */
    "gerenciadosDeSensores", /* nome da tarefa */
    10000,      /* número de palavras a serem alocadas para uso com a pilha da tarefa */
    NULL,       /* parâmetro de entrada para a tarefa (pode ser NULL) */
    5,          /* prioridade da tarefa (0 a N) */
    NULL,       /* referência para a tarefa (pode ser NULL) */
    nucleo1);         /* Núcleo que executará a tarefa */

  delay(500); //tempo para a tarefa iniciar
  xTaskCreatePinnedToCore(
    gerenciadorConexaoBluetooth,   /* função que implementa a tarefa */
    "gerenciadorConexaoBluetooth", /* nome da tarefa */
    10000,      /* número de palavras a serem alocadas para uso com a pilha da tarefa */
    NULL,       /* parâmetro de entrada para a tarefa (pode ser NULL) */
    5,          /* prioridade da tarefa (0 a N) */
    NULL,       /* referência para a tarefa (pode ser NULL) */
    nucleo0);         /* Núcleo que executará a tarefa */

  delay(500); //tempo para a tarefa iniciar
}

void loop() {
  // gerenciadorwifi
  delay(10);

}

void gerenciadorDeSensores(void *p) {
  inciarSensores();
  char bugDht = 0;
  while (1) {
    if (pausa) {
      pausa = 0;
      delay(2000);
    }
    bugDht = lerSensores(&meusSensores);
    digitalWrite(ledWarnRed, bugDht); //acende caso dht falhe
    printaRDados(meusSensores);
    ativarTomada(condicoes.meuParametro, &ativa, &condicoes, &meusSensores,  bugDht);
    digitalWrite(PIN_TO_PLUG, ativa);
  }
}

void gerenciadorConexaoBluetooth(void *p) {
  iniciaBluetooth();
  while (1) {
    delay(10);

  }
}

