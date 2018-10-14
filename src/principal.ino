//version: beta 1.0.0001 
#include "Wire.h"
#include "DHT.h"
#include "Tomada.h"

#define DHTTYPE DHT22
#define nucleo0  0
#define nucleo1  1
// pinos reservados
#define DHTPin     4
#define ledWarnRed 5
#define pinTomada 18
//tipos de conexao
#define NO_CONECTION 0
#define WIFI_TYPE 1
#define BLUETOOTH_TYPE 2

//variaveis para Dht quem edita é o gerenciador de tomada
DadosSensores meusSensores;
//variaveis para conexao quem edita é o gerenciador de conexao
DadosParametro meusParametros;

DHT dht(DHTPin, DHTTYPE);
// variaveis para tomada
char tomadaAtivada = 0;
char parametroType = EVEROFF;
long long int tmpInicial;
//variaveis para warnings
char bugSensores = 0;

/*** comunicacaoes entre conexao e tomada**/
char pausa = 0; // pausa de 2 segundos para ser mudado os parametroType

/*    funcoes   */
int lerSensores(DadosSensores *mydhtLeitura);

void lerTempo(DadosSensores *mydhtLeitura);

void gerenciadorDaTomada(void *pvParameters);

void gerenciadorConexoes(void *pvParameters);

void inciarPinos();

void setup() {
    inciarPinos();
    Serial.begin(115200);

    //nucleo0:gerenciar a tomada
    xTaskCreatePinnedToCore(
            gerenciadorDaTomada,   /* função que implementa a tarefa */
            "gerenciadorDaTomada", /* nome da tarefa */
            10000,      /* número de palavras a serem alocadas para uso com a pilha da tarefa */
            NULL,       /* parâmetro de entrada para a tarefa (pode ser NULL) */
            2,          /* prioridade da tarefa (0 a N) */
            NULL,       /* referência para a tarefa (pode ser NULL) */
            nucleo0);         /* Núcleo que executará a tarefa */

    delay(500); //tempo para a tarefa iniciar

    //cria uma tarefa que será executada na função coreTaskOne, com prioridade 2 e execução no núcleo 1
    //coreTaskOne: atualizar as informações do display
    xTaskCreatePinnedToCore(
            gerenciadorConexoes,   /* função que implementa a tarefa */
            "gerenciadorConexoes", /* nome da tarefa */
            10000,      /* número de palavras a serem alocadas para uso com a pilha da tarefa */
            NULL,       /* parâmetro de entrada para a tarefa (pode ser NULL) */
            2,          /* prioridade da tarefa (0 a N) */
            NULL,       /* referência para a tarefa (pode ser NULL) */
            nucleo1);         /* Núcleo que executará a tarefa */

    delay(500); //tempo para a tarefa iniciar

}


void loop() {
}

int lerSensores(DadosSensores *mydhtLeitura) {
    mydhtLeitura->hum = dht.readHumidity();
    mydhtLeitura->tempCels = dht.readTemperature();
    mydhtLeitura->tempFar = dht.readTemperature(true);
    lerTempo(mydhtLeitura);
    if (isnan(mydhtLeitura->hum) || isnan(mydhtLeitura->tempCels) || isnan(mydhtLeitura->tempFar)) {
        return false;
    }
    return true;
}

void inciarPinos() {
    pinMode(ledWarnRed, OUTPUT);
    pinMode(pinTomada, OUTPUT);
    
}

void lerTempo(DadosSensores *mydhtLeitura) {
    mydhtLeitura->horaAtual.seg = millis() / 1000;
}


void gerenciadorDaTomada(void *pvParameters) {
    int estadoDaTomada = tomadaAtivada;
    while (true) {
        if (pausa) {
            delay(2000);
            pausa = 0;
        }
        digitalWrite(ledWarnRed, bugSensores);
        if (!lerSensores(&meusSensores)) {
            bugSensores = 1;
        } else {
            bugSensores = 0;
        }
        ativarTomada(parametroType, &tomadaAtivada, &meusParametros, &meusSensores, &tmpInicial);
        if (estadoDaTomada != tomadaAtivada) {
            digitalWrite(pinTomada, tomadaAtivada);
            estadoDaTomada = tomadaAtivada;
            Serial.println("tomada esta ativa? ");
            Serial.print(tomadaAtivada);
        }


    }
}

void gerenciadorConexoes(void *pvParameters) {
    pausa = 1;
    int typeConection=NO_CONECTION;
    while (true) {
      if(typeConection==NO_CONECTION){
        if(haveBluetooth(&typeConection))
        else if(haveWifi(&typeConection))
      }
    }
}
