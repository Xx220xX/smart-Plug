#ifndef CPP_SMT_SENSOR_H
#define CPP_SMT_SENSOR_H

#include "lib/TIME.h"

class{
    DHT dht;
    bool usando = false;
public:

    void init(uint8_t pin,uint8_t type = 22){
         dht = DHT (pin,type);
    }

    float getTemperature(){
        while(usando) delay(1);
        usando = true;
        float temp = dht.readTemperature();
        usando = false;
        return temp;
    }
    float getHumidy(){
        while(usando) delay(1);
        usando = true;
        float temp = dht.readHumidity();
        usando = false;
        return temp;
        }
}Sensor;


#endif //CPP_SMT_SENSOR_H
