//#include "SENSOR/Sensor.h"
#include "COMUNICACAO/Comunicacao.h"
#include "Execute_Commands/Commands.h"
void onReceived_mensagem(int ID, char *msg){
    execute(ID,msg);
    COMUNICACAO_RESUME();
}

void setup() {
    COMUNICACAO_init("esp_32",
                     "meuWifi",
                     "123456789",
                     "iot.eclipse.org",
                     1883);

}

void loop() {

}