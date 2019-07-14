#ifndef TIPOS_H
#define TIPOS_H

#include "RtcDateTime.h"

#define PLUG_ON 1
#define PLUG_OFF 0

#define Tempo RtcDateTime

typedef uint32_t  Time;
typedef struct {
    float minHum, maxHum, minTemp, maxTemp;
    Tempo  tempoInicial_parametro4;
    Time periodico,tempoN_parametro4;
    int meuParametro;
    int modoPeriodicoInicial;
} DadosParametro;

typedef struct {
    float hum, tempCels, tempFar;
    Tempo horaAtual;
} DadosSensores;
#endif
