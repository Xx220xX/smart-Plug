//
// Created by Lace2 on 03/09/2018.

//

void setup();

void loop();

unsigned int millis();

void lerSensor();

int main() {
    setup();
    do {
        loop();
    } while (1);
}
 

unsigned int tempoInicialdht = millis();
unsigned int deltaTempoDht = 0;
int temperatura = 0;
int umidade = 0;

void setup() {
//version 1.0.001
}

void loop() {
//criar conexao via bluetooth 
//criar conexao via wifi 
// caso desconect continuar 
// ter uma struct de dados para saber os parametros
// dar preferencia  a apenas um tipo de conexao, caso wifi ligado desativar bluetooth e vice versa
// enviar dados para o aplicativo 

    deltaTempoDht = millis() - tempoInicialdht;
    if (deltaTempoDht > 5000) {
        lerSensor();
        tempoInicialdht = millis();
    }
}

void lerSensor() {
// ler sensores do dht umidade temperatura
}


unsigned int millis() {
//essa funcao é representativa o arduino ja posui uma 
    return 0;
}


