//
// Created by PC on 10/08/2019.
//

#ifndef CPP_TIME_H
#define CPP_TIME_H

#include "time.h"
#include "sys/time.h"

long t_seg(int year, int month, int day, int hour, int minute, int second) {
    struct tm temp;
    temp.tm_year = year - 1900;
    temp.tm_mon = month - 1;
    temp.tm_mday = day;
    temp.tm_hour = hour;
    temp.tm_min = minute;
    temp.tm_sec = second;
    return mktime(&temp);
}

void setTime(time_t seg) {
    timeval tv = {0, 0};//Cria a estrutura temporaria para funcao abaixo.
    tv.tv_sec = seg;//Atribui minha data atual. Voce pode usar o NTP para isso ou o site citado no artigo!
    settimeofday(&tv, NULL);
}

/**
 *
 * @param year referente ao ano
 * @param month referente ao mes
 * @param day referente ao dia do mes
 * @param hour referente a hora
 * @param minute referente ao minuto
 * @param second referente ao segundo
 */

void setTime(int year, int month, int day, int hour, int minute, int second) {
    setTime(t_seg(year, month, day, hour, minute, second));
}

char *time_tostr(time_t rawtime = -1) {
    struct tm *timeinfo;
    if (rawtime == -1)
        time(&rawtime);
    timeinfo = localtime(&rawtime);
    return asctime(timeinfo);
}
 tm  getDataTime(){
    time_t rawtime;
    struct tm *timeinfo;
    time(&rawtime);
    return *localtime(&rawtime);
}

#endif //CPP_TIME_H
