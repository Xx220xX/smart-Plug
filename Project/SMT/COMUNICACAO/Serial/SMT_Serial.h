#ifndef CPP_SMT_SERIAL_H
#define CPP_SMT_SERIAL_H


class MEU_SERIAL : public Comunicacao {
private:
    static MEU_SERIAL *me;

    static void serial_loop(void *p) {

        Serial.begin(me->veloc);
        char *msg;
        for (;;) {
            if (Serial.available()) {
                Comunicacao::PAUSE_ALL(SMT_ID_SERIAL);
                int i = 0;
                msg = (char *) (calloc(500, sizeof(char)));
                for (i = 0; Serial.available() && i < 499; ++i) {
                    msg[i] = Serial.read();
                }
                msg[i] = 0;
                received_mensagem(me, msg);
                free(msg);
            }
            delay(100);
        }
    }

    int veloc;
public:
    void sendMensage(char *string) override {
        Serial.println(string);
    }

    void start() override {
        me = this;
        myThread.start();
    }

    void init(int veloc) override {
        this->veloc = veloc;
        myThread = Thread(serial_loop, "SERIAL THREAD");
    }

    MEU_SERIAL() {
        Comunicacao::comunicacoes[SMT_ID_SERIAL] = this;
    }
};

MEU_SERIAL *MEU_SERIAL::me = nullptr;


#endif //CPP_SMT_SERIAL_H
