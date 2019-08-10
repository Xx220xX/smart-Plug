#define SMT_DEBUG_TASK
#define SMT_DEBUG_COMANDS
#define SMT_BLUETOOTH_DISABLE
#include<DataProcess.h> //https://github.com/Xx220xX/DataProcess
#include<ThreadEsp32.h> //https://github.com/Xx220xX/ThreadEsp32
#include "time.h"
#include "sys/time.h"
#include "SENSOR/SMT_Sensor.h"
#include "COMUNICACAO/Comunicacao.h"
#include "TASK/Smt_tarefas.h"
#include "COMMANDS/Commands.h"


// para atulizar a hora chamar smt_cl_wifi.atualizarHora();
void setup() {
   
    SMT.addPlug(PLUG("teste",2));//por ordem de insercao
    Comunicacao::init_ALL(115200,
                          "esp_32",
                          "meuWifi",
                          "123456789",
                          "iot.eclipse.org",
                          1883);
    smt_cl_wifi.atualizarHora();
    Thread::getCurrent().stop();
}

void loop() {

}
