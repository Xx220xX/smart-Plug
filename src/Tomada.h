typedef  struct {
    int dia,hora,min,seg;
    long long int realT;
}Tempo;


typedef enum {
    TEMPERATURA, HUMIDADE, TEMPO, EVERON, EVEROFF
} Parametros;

typedef struct {
    int minHum, maxHum, minTemp, maxTemp;
    Tempo tempo;
    
} DadosParametro;

typedef struct {
    float hum, tempCels, tempFar;
    Tempo horaAtual;
} DadosSensores;
void arrumaTempo(Tempo *tmp);
void manterAte(char *ativa,Tempo *tmpInicio,Tempo *tmpAtual,long long int fim);
void manterEntre(char *ativa, int min, int max, float atual);
void ativarTomada(char parametro, char *ativa, DadosParametro *myparamentros, DadosSensores *myDados);



void arrumaTempo(Tempo *tmp){
    tmp->realT = tmp->hora*60*60+tmp->min*60+tmp->seg+tmp->dia*24*60*60;
}
void manterAte(char *ativa,Tempo *tmpInicio,Tempo *tmpAtual,long long int fim){
    arrumaTempo(tmpInicio);
    arrumaTempo(tmpAtual);
    if(tmpAtual->realT-tmpInicio->realT>fim){
        *ativa=0;
    }
}


void manterEntre(char *ativa, int min, int max, float atual) {
    if (atual <= min) {
        *ativa = 1;
    } else if (atual >= max) {
        *ativa = 0;
    }
}

void ativarTomada(char parametro, char *ativa, DadosParametro *myparamentros, DadosSensores *myDados,long long int *tmpInicio) {
    switch (parametro) {
        case TEMPERATURA:
            manterEntre(ativa, myparamentros->minTemp, myparamentros->maxTemp, myDados->tempCels);
            break;
        case HUMIDADE:
            manterEntre(ativa, myparamentros->minHum, myparamentros->maxHum, myDados->hum);
            break;
        case TEMPO:
            manterAte(ativa, &myparamentros->tempo, &myDados->horaAtual,*tmpInicio);
            break;
        case EVERON:
            *ativa = 1;
            break;
        case EVEROFF:
            *ativa = 0;
            break;
    }

}
