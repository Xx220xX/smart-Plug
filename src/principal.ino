//version: beta 1.0.0003

#include "Wire.h"
#include "DHT.h"
#include "Tomada.h"
#include "Conexao.h"
#include "ConexaoBluetooth.h"

#define DHTTYPE DHT22
#define nucleo0  1
#define nucleo1  0
// pinos reservados
#define DHTPin     4
#define ledWarnRed 5
#define pinTomada 18


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




//tipos de conexao
#define NO_CONECTION 0
#define WIFI_TYPE 1
#define BLUETOOTH_TYPE 2
String dados = "    ";
 char str[256];
char haveBluetooth(char *typeConection) {
  return 1;
}
char haveWifi(char *typeConection) {
  return 0;
}
char enviarDadosBluetooth(DadosSensores minhaLeitura) {
  Serial.println();
  dados = "Celsios " + String(minhaLeitura.tempCels) + "\nfahren " + String(minhaLeitura.tempFar) + "\nHumidade: " + String(minhaLeitura.hum);
  Serial.println(dados);
  if (envia) {
    Serial.println("enviado");
    int i=0;
    for(;dados.length();i++){
      str[i]=dados.charAt(i);
    }
    str[i]=0;
    
    enviarMensagemBT(str);
    envia = 0;
  }

  return 1;
}



/*    funcoes   */
int lerSensores(DadosSensores *mydhtLeitura);

void lerTempo(DadosSensores *mydhtLeitura);

void gerenciadorDaTomada(void *pvParameters);

void gerenciadorConexoes(void *pvParameters);

void inciarPinos();
void BluetoothConect(void *pvParameters);

void setup() {
    inciarPinos();
    Serial.begin(115200);

    //nucleo0:gerenciar a tomada
    xTaskCreatePinnedToCore(
            gerenciadorDaTomada,   /* função que implementa a tarefa */
            "gerenciadorDaTomada", /* nome da tarefa */
            10000,      /* número de palavras a serem alocadas para uso com a pilha da tarefa */
            NULL,       /* parâmetro de entrada para a tarefa (pode ser NULL) */
            5,          /* prioridade da tarefa (0 a N) */
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
            5,          /* prioridade da tarefa (0 a N) */
            NULL,       /* referência para a tarefa (pode ser NULL) */
            nucleo1);         /* Núcleo que executará a tarefa */

    delay(500); //tempo para a tarefa iniciar
     xTaskCreatePinnedToCore(
            BluetoothConect,   /* função que implementa a tarefa */
            "BluetoothConect ", /* nome da tarefa */
            10000,      /* número de palavras a serem alocadas para uso com a pilha da tarefa */
            NULL,       /* parâmetro de entrada para a tarefa (pode ser NULL) */
            3,          /* prioridade da tarefa (0 a N) */
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
    delay(400);
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
            delay(10000);
            pausa = 0;
        }
        digitalWrite(ledWarnRed, bugSensores);
        if (!lerSensores(&meusSensores)) {
            bugSensores = 1;
        } else {
            bugSensores = 0;
        }
        ativarTomada(parametroType, &tomadaAtivada, &meusParametros, &meusSensores, &tmpInicial,bugSensores);
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
    char typeConection=NO_CONECTION;
    while (true) {
      delay(5000);
      if(typeConection==NO_CONECTION){
        if(haveBluetooth(&typeConection)){
          enviarDadosBluetooth(meusSensores);
        
            
         }else if(haveWifi(&typeConection)){
          
         }
      }
    }
}

void BluetoothConect(void *pvParameters){
  iniciaBluetooth();
  delay(1000);
  while(1){
    Serial.println("bluetooth ligado");
    delay(2000);
  }
  
}
