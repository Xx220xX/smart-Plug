//
// Created by PC on 05/08/2019.
//

#ifndef CPP_WIFI_H
#define CPP_WIFI_H

#include <WiFi.h>
#include <PubSubClient.h>
#include<ThreadEsp32.h>

#define  BROKER_pub "SMT_ESP32p"
#define  BROKER_sub "SMT_ESP32s"
#define ID_MQTT  "ESP32_smt_PLUG"

typedef char *str;

class SMT_WIFI {
protected:
    char name[40] = {0};
    char pass[40] = {0};
    char mqtt_server[40] = {0};
    char mqtt_pass[40] = {0};
    int mqtt_port = 0;
};

// variaveis
SMT_WIFI this_wifi; // contem informacoes da conexao
Thread WIFI_THREAD;
WiFiClient espClient;
PubSubClient MQTT(espClient);

// Prototipos
void SMT_INIT_WIFI();

void SMT_INIT_MQTT();

void SMT_RECONECT_WIFI();

void VerificaConexoesWiFIEMQTT(void);

void SMT_mqtt_callback(char *topic, byte *payload, unsigned int length);


void WIFI_LOOP(void *p);// usado na THREAD_WIFI

/**
 * seta as configuracoes do wifi e deleta a THREAD_WIFI, deve chamar WIFI_START() para ter o wifi
 * @param name
 * @param pass
 * @param mqtt_server
 * @param mqtt_pass
 * @param mqtt_port
 */
void SMT_WIFI_CONFIG(str name, str pass, str mqtt_server = 0, str mqtt_pass = 0, int mqtt_port = 0) {
    WIFI_THREAD.stop();
    strcpy(this_wifi.name, name);
    strcpy(this_wifi.pass, pass);
    if (mqtt_server) {
        strcpy(this_wifi.mqtt_server, mqtt_server);
        strcpy(this_wifi.mqtt_pass, mqtt_pass);
        this_wifi.mqtt_port = mqtt_port;
    }
}

void WIFI_START() {
    WIFI_THREAD = Thread(WIFI_LOOP, "WIFI THREAD");
    WIFI_THREAD.start();
}

void SMT_INIT_WIFI() {
    SMT_RECONECT_WIFI();
}

void SMT_INIT_MQTT() {
    MQTT.setServer(this_wifi.mqtt_server, this_wifi.mqtt_port);   //informa qual broker e porta deve ser conectado
    MQTT.setCallback(
            SMT_mqtt_callback);            //atribui função de callback (função chamada quando qualquer informação de um dos tópicos subescritos chega)
}


void SMT_RECONECT_WIFI() {
    if (WiFi.status() == WL_CONNECTED)
        return;
    WiFi.begin(this_wifi.name, this_wifi.pass);
    while (WiFi.status() != WL_CONNECTED)
        delay(100);
}

void VerificaConexoesWiFIEMQTT(void) {
    if (!MQTT.connected())
        reconnectMQTT(); //se não há conexão com o Broker, a conexão é refeita

    reconectWiFi(); //se não há conexão com o WiFI, a conexão é refeita
}

void reconnectMQTT() {
    while (!MQTT.connected())
        if (MQTT.connect(ID_MQTT))
            MQTT.subscribe(BROKER_sub);
        else
            delay(2000);

}

#endif //CPP_WIFI_H
