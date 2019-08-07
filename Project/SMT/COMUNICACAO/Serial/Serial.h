//
// Created by PC on 06/08/2019.
//

#ifndef CPP_SERIAL_H
#define CPP_SERIAL_H
Thread SERIAL_THREAD;

void SERIAL_loop(void *param);

void SMT_SERIAL_INIT(int speed);

void SERIAL_START();

void SMT_SERIAL_INIT(int speed) {
    SERIAL_THREAD.stop();
    Serial.begin(speed);
    SERIAL_THREAD = Thread(SERIAL_loop, "SERIAL THREAD");
}

void serial_pause() {
    SERIAL_THREAD.pause();
}

void serial_resume() {
    SERIAL_THREAD.resume();
}

void SERIAL_START() {
    SERIAL_THREAD.start(nullptr);
}

void SERIAL_loop(void *param) {
    for (;;) {
        if (Serial.available()) {
            COMUNICACAO_PAUSE(ID_SERIAL);
            int i = 0;
            while (Serial.available() && i < 499) {
                mensagem[i++] = Serial.read();
            }
            mensagem[i] = 0;
            onReceived_mensagem(ID_SERIAL, mensagem);
        }
        delay(100);
    }
}

void Serial_send_msg(char *msg) {
    Serial.print(msg);
}

#endif //CPP_SERIAL_H
