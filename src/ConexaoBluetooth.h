#include <Wire.h>                //Biblioteca para I2C
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

BLECharacteristic *characteristicTX; //através desse objeto iremos enviar dados para o client

bool deviceConnected = false; //controle de dispositivo conectado

#define SERVICE_UUID           "ab0828b1-198e-4351-b779-901fa0e0371e" // UART service UUID
#define CHARACTERISTIC_UUID_RX "4ac8a682-9736-4e5d-932b-e9b31405049c"
#define CHARACTERISTIC_UUID_TX "0972EF8C-7613-4075-AD52-756F33D4DA91"

char envia = 0;


void  analizarParametros(  char  *comandos, int tam);
void enviarMensagemBT(char mensagem[]);
void iniciaBluetooth();
void capturarFloat( char  *str, float *minimo, float *maximo, int tam);
void  capturarTempo(char *comandos, Tempo *tempo, int tam);



class ServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
    };

    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
    }
};

//callback  para envendos das características
class CharacteristicCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *characteristic) {
      //retorna ponteiro para o registrador contendo o valor atual da caracteristica
      std::string rxValue = characteristic->getValue();
      //verifica se existe dados (tamanho maior que zero)
      char recebido[500];
      int i = 0;
      if (rxValue.length() > 0) {
        Serial.println("*********");
        Serial.print("Received Value: ");
        for (i = 0; i < rxValue.length(); i++) {
          recebido[i] = rxValue[i];
        }
        recebido[i] = 0;
        analizarParametros(recebido, rxValue.length());





      }
    }
};

void iniciaBluetooth() {
  // Create the BLE Device
  BLEDevice::init("ESP32-BLE"); // nome do dispositivo bluetooth
  // Create the BLE Server
  BLEServer *server = BLEDevice::createServer(); //cria um BLE server
  server->setCallbacks(new ServerCallbacks()); //seta o callback do server
  // Create the BLE Service
  BLEService *service = server->createService(SERVICE_UUID);
  // Create a BLE Characteristic para envio de dados
  characteristicTX = service->createCharacteristic(
                       CHARACTERISTIC_UUID_TX,
                       BLECharacteristic::PROPERTY_NOTIFY
                     );
  characteristicTX->addDescriptor(new BLE2902());
  // Create a BLE Characteristic para recebimento de dados
  BLECharacteristic *characteristic = service->createCharacteristic(
                                        CHARACTERISTIC_UUID_RX,
                                        BLECharacteristic::PROPERTY_WRITE
                                      );
  characteristic->setCallbacks(new CharacteristicCallbacks());
  // Start the service
  service->start();
  // Start advertising (descoberta do ESP32)
  server->getAdvertising()->start();
  Serial.println("Waiting a client connection to notify...");

}

void enviarMensagemBT(char mensagem[]) {
  //se existe algum dispositivo conectado
  if (deviceConnected) {
    //chamamos o método "read" do sensor para realizar a leitura da temperatura
    //read retornará 1 caso consiga realizar a leitura, ou 0 caso contrário

    characteristicTX->setValue(mensagem); //seta o valor que a caracteristica notificará (enviar)
    characteristicTX->notify(); // Envia o valor para o smartphone
    Serial.print("*** Sent Value: ");
    Serial.print(mensagem);
    Serial.println(" ***");
  }

}
void  analizarParametros(  char  *comandos, int tam) {
  //primeiro caractere sera o tipo de parametro
  switch (comandos[0]) {
    //"022.30/25.52"
    case '0':
      Serial.println("TEMPERATURA");
      condicoes.meuParametro = TEMPERATURA;
      capturarFloat(comandos, &condicoes.minTemp, &condicoes.maxTemp, tam);

      break;
    case '1':
      Serial.println("HUMIDADE");
      condicoes.meuParametro = HUMIDADE;
      capturarFloat(comandos, &condicoes.minHum, &condicoes.maxHum, tam);
      break;
    case '2':

      Serial.println("TEMPO");
      capturarTempo(comandos, &condicoes.tempo, tam);
      Serial.println((unsigned int )condicoes.tempo.realT);
      t0 = meusSensores.horaAtual;
      condicoes.meuParametro = TEMPO;
      break;
    case '3':
      Serial.println("EVERON");
      condicoes.meuParametro = EVERON;
      break;
    case '4':
      Serial.println("EVEROFF");
      condicoes.meuParametro = EVEROFF;
      break;
    default:
      return;

  }
  pausa = 1;
}


void capturarFloat( char  *str, float *minimo, float *maximo, int tam) {
  char c = 0;
  int i = 1, divisor = 1, dividir = 0;
  *minimo = 0;
  *maximo = 0;
  //26.3/33.4/
  for (i = 0; i < tam && str[i] != '/'; i++) {
    c = str[i];
    if (c == '.' || c == ',') {
      dividir = 1;
      continue;
    }
    if (dividir) {
      divisor *= 10;
    }
    *minimo *= 10;
    *minimo += (c - 48);
  }
  *minimo /= divisor;
  i++;//sair de cima do '/'
  dividir = 0;
  divisor = 1;
  for (; i < tam && str[i] != '/'; i++) {
    c = str[i];
    if (c == '.' || c == ',') {
      dividir = 1;
      continue;
    }

    if (dividir) {
      divisor *= 10;
    }
    *maximo *= 10;
    *maximo += (c - 48);

  }
  *maximo /= divisor;

}
void  capturarTempo(char *comandos, Tempo *tempo, int tam) {
  int i = 1;
  //segundo de tempo ligado
  tempo->realT = 0;
  for (i = 1; i < tam && comandos[i] != '/'; i++) {
    tempo->realT = tempo->realT * 10 + comandos[i] - 48;
  }
}


