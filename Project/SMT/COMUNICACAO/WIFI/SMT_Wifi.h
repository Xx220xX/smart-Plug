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
#ifndef SMT_WIFI_H
#define SMT_WIFI_H

#include <WiFi.h>
#include <PubSubClient.h>

#define  BROKER_pub "SMT_ESP32p"
#define  BROKER_sub "SMT_ESP32s"
#define ID_MQTT  "ESP32PLUG_001"

WiFiClient espClient;
PubSubClient MQTT(espClient);

class MEU_WIFI : public Comunicacao {
private:
    char name[40] = {0};
    bool att_hour = true;
private:
    char pass[40] = {0};
    char mqtt_server[40] = {0};
    int mqtt_port = 0;
    static MEU_WIFI *me;

    static void call_back(char *topic, byte *payload, unsigned int length) {
        Comunicacao::PAUSE_ALL(SMT_ID_WIFI);
        char *msg = (char *) calloc(sizeof(char), length + 1);
        for (int i = 0; i < length; i++)
            msg[i] = (char) payload[i];
        received_mensagem(me, msg);
    }

    static void wifi_loop(void *p) {
        me->reconectWifi();
        MQTT.setServer(me->mqtt_server, me->mqtt_port);   //informa qual broker e porta deve ser conectado
        MQTT.setCallback(call_back);
        for (;;) {
            if (!MQTT.connected()) {
                me->reconectMQTT(); //se não há conexão com o Broker, a conexão é refeita
            }
            me->reconectWifi();
            MQTT.loop();
            delay(200);
            if(me->att_hour){
                me->att_hour= false;
                configTime(0, -3600*3, "pool.ntp.org");
            }
        }
    }

    void reconectMQTT() {
        while (!MQTT.connected() && WiFi.status() == WL_CONNECTED)
            if (MQTT.connect(ID_MQTT)) {
                MQTT.subscribe(BROKER_sub);
            } else
                delay(2000);

    }

    void reconectWifi() {
        if (WiFi.status() == WL_CONNECTED)
            return;
        WiFi.begin(name, pass);
        while (WiFi.status() != WL_CONNECTED)
            delay(100);
    }

public:
/*
    void setWIFI(char *_name, char *_pass) {
        strcpy(name, _name);
        strcpy(pass, _pass);
    }
    agr sem bluetooth isso nao faz sentido mais :/
*/

    void sendMensage(char *msg) override {
        try {
            MQTT.publish(BROKER_pub, msg);
        } catch (...) {

        }
    }

    void start() override {
        me = this;
        myThread.start();
    }

    void init(char *_name, char *_pass, char *_mqtt_server = nullptr, int port = 0) override {
        myThread = Thread( "WIFI THREAD",MEU_WIFI::wifi_loop);
        strcpy(this->name, _name);
        strcpy(this->pass, _pass);
        if (_mqtt_server) {
            strcpy(this->mqtt_server, _mqtt_server);
            this->mqtt_port = port;
        }
    }

    /**
    * Subclasse de Comunicacao possui o necessario para o funcionamento do mqtt, basta chamar o metodo
     * @init (name,pass,mqqttserver,porta)
     * e dpois o metodo @start() herdado de comunicacao
     * quando uma mensagem chegar a funcao @received_mensagem sera chamada
    */

    MEU_WIFI() {
        Comunicacao::comunicacoes[SMT_ID_WIFI] = this;
    }
    void atualizarHora(){
        this->att_hour =true;
    }
};

MEU_WIFI *MEU_WIFI::me = nullptr;
#endif //CPP_WIFI_H
