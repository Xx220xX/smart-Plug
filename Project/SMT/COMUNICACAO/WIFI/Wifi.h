//
// Created by PC on 05/08/2019.
//

/**
 * Sequencia de funcionamento
 * em uma Thread separada
 *  conectar wifi
 *  conectar mqtt
 *  loop{
 *      verificarWIFI_E_MQTT
 *      verificar se tem mensagem (caso tiver chamar receivedMsg de comunicacao.h)
 *  }
 *
 */
#ifndef CPP_WIFI_H
#define CPP_WIFI_H
#ifdef SMT_WIFI_DEBUG
#define PRINT(x) Serial.print(x);
#define PRINTLN(x) Serial.println(x);
#else
#define PRINT(x)
#define PRINTLN(x)
#endif

#include <WiFi.h>
#include <PubSubClient.h>
#include<ThreadEsp32.h>
#include <cstring>

#define  BROKER_pub "SMT_ESP32p"
#define  BROKER_sub "SMT_ESP32s"
#define ID_MQTT  "ESP32_smt_PLUG"

typedef char *str;

struct SMT_WIFI {

    char name[40] = {0};
    char pass[40] = {0};
    char mqtt_server[40] = {0};
    int mqtt_port = 0;
};

// variaveis
SMT_WIFI this_wifi; // contem informacoes da conexao
Thread WIFI_THREAD;
WiFiClient espClient;
PubSubClient MQTT(espClient);

// Prototipos
GLOBAL_ void WIFI_START();
GLOBAL_ void SMT_WIFI_SET(char *name, char *pass);
GLOBAL_ char * WIFI_getName();
GLOBAL_ char * WIFI_getPass();
GLOBAL_ char * WIFI_getMqttServer();
GLOBAL_ int  WIFI_getMqttPort();
void SMT_INIT_WIFI();

void SMT_INIT_MQTT();

void SMT_RECONECT_WIFI();

void SMT_RECONECT_MQTT();

void SMT_VerificaConexoesWiFIEMQTT(void);

void SMT_mqtt_callback(char *topic, byte *payload, unsigned int length);

void WIFI_LOOP(void *p);// usado na THREAD_WIFI

/**
 * seta as configuracoes do wifi e deleta a THREAD_WIFI, deve chamar WIFI_START() para ter o wifi
 * @param name
 * @param pass
 * @param mqtt_server
 * @param mqtt_port
 */
void SMT_WIFI_CONFIG(str name, str pass, str mqtt_server = 0, int mqtt_port = 0) {
    PRINTLN("configurando wifi")
    WIFI_THREAD.stop();
    strcpy(this_wifi.name, name);
    strcpy(this_wifi.pass, pass);
    if (mqtt_server) {
        strcpy(this_wifi.mqtt_server, mqtt_server);
        this_wifi.mqtt_port = mqtt_port;
    }
}

void WIFI_START() {
    PRINTLN("iniciando THREAD Wifi")
    WIFI_THREAD = Thread(WIFI_LOOP, "WIFI THREAD");
    WIFI_THREAD.start(nullptr);
}

void SMT_INIT_WIFI() {
    SMT_RECONECT_WIFI();
}

void SMT_INIT_MQTT() {
    MQTT.setServer(this_wifi.mqtt_server, this_wifi.mqtt_port);   //informa qual broker e porta deve ser conectado
    MQTT.setCallback(
            SMT_mqtt_callback);//atribui função de callback (função chamada quando qualquer informação de um dos tópicos subescritos chega)
}


void SMT_RECONECT_WIFI() {
    if (WiFi.status() == WL_CONNECTED)
        return;
    WiFi.begin(this_wifi.name, this_wifi.pass);
    while (WiFi.status() != WL_CONNECTED)
        delay(100);
}

void SMT_RECONECT_MQTT() {
    while (!MQTT.connected())
        if (MQTT.connect(ID_MQTT))
            MQTT.subscribe(BROKER_sub);
        else
            delay(2000);

}

void SMT_VerificaConexoesWiFIEMQTT(void) {
    if (!MQTT.connected())
        SMT_RECONECT_MQTT(); //se não há conexão com o Broker, a conexão é refeita
    SMT_RECONECT_WIFI(); //se não há conexão com o WiFI, a conexão é refeita
}


void WIFI_LOOP(void *p) {
    SMT_INIT_WIFI();
    SMT_INIT_MQTT();
    for(;;) {
        SMT_VerificaConexoesWiFIEMQTT();
        MQTT.loop();

        delay(200);
    }
}

void SMT_mqtt_callback(char *topic, byte *payload, unsigned int length){
    COMUNICACAO_PAUSE(ID_WIFI);
    PRINTLN("chegou mensagem pelo wifi:")
    int i = 0;
    for(;i<length&&i<499;i++)
        mensagem[i]=(char)payload[i];
    mensagem[i] = 0;
    onReceived_mensagem(ID_WIFI, mensagem);
}
void wifi_pause(){
    WIFI_THREAD.pause();
}void wifi_resume(){
    WIFI_THREAD.resume();
}
void wifi_send_msg(char *msg){
    PRINT("enviando msg")
    MQTT.publish(BROKER_pub, msg);
    PRINTLN(": sucesso!!")
}
GLOBAL_ void SMT_WIFI_SET(char *name, char *pass){
    PRINTLN("solicitacao de mudanca de nome wifi")
    SMT_WIFI_CONFIG(name,pass);
    WIFI_START();
}
GLOBAL_ char * WIFI_getName(){
    return this_wifi.name;
}
GLOBAL_ char * WIFI_getPass(){
    return this_wifi.pass;
}
GLOBAL_ char * WIFI_getMqttServer(){
    return this_wifi.mqtt_server;
}
GLOBAL_ int  WIFI_getMqttPort(){
    return this_wifi.mqtt_port;
}

#undef PRINTLN
#undef PRINT
#endif //CPP_WIFI_H
