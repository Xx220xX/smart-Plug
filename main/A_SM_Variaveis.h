#ifndef VARIAVEIS_H
#define VARIAVEIS_H

#define nucleo0 0
#define nucleo1 1

DadosSensores meusSensores;
DadosParametro condicoes;
int ativa = 0;
char pausa = 0;
RtcDS3231<TwoWire> rtc(Wire);

#endif
