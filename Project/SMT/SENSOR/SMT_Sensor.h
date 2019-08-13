#ifndef CPP_SMT_SENSOR_H
#define CPP_SMT_SENSOR_H

#include "lib/TIME.h"

class{
    DHT dht;
public:
    void init(uint8_t pin,uint8_t type = 22){
         dht = DHT (pin,type);

    }

    float getTemperature(){
        return dht.readTemperature();
    }
    float getHumidy(){
        return dht.readHumidity();
    }
}Sensor;


#endif //CPP_SMT_SENSOR_H
