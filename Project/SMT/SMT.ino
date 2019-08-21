#define SMT_DEBUG_COMANDS
//#define SMT_BLUETOOTH_DISABLE
#define SMT_WIFI_DISABLE
//#define SMT_DEBUG_TASK
// dependencias
//Adafruit_Sensor.h https://github.com/adafruit/Adafruit_Sensor
#include<DataProcess.h> //https://github.com/Xx220xX/DataProcess
#include<ThreadEsp32.h> //https://github.com/Xx220xX/ThreadEsp32
#include<DHT.h> // https://github.com/adafruit/DHT-sensor-library

#include "time.h"
#include "sys/time.h"
#include "SENSOR/SMT_Sensor.h"
#include "COMUNICACAO/Comunicacao.h"
#include "TASK/Smt_tarefas.h"
#include "COMMANDS/Commands.h"


// para atulizar a hora chamar smt_cl_wifi.atualizarHora();
void setup() {
    Sensor.init(4, 22);
    SMT.addPlug(PLUG("teste", 2));//por ordem de insercao
    Comunicacao::init_ALL(115200,
                          "esp_32",
                          "meuWifi",
                          "123456789",
                          "iot.eclipse.org",
                          1883);
#ifndef SMT_WIFI_DISABLE
    smt_cl_wifi.atualizarHora();
#endif

    Thread::getCurrent().stop();
}

void loop() {

}
