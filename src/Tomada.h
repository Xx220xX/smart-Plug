/** version beta 1.0.0002**/



void arrumaTempo(Tempo *tmp);
void manterEntre(char *ativa, int min, int max, float atual);
void ativarTomada(char parametro, char *ativa, DadosParametro *myparamentros, DadosSensores *myDados, char sensorDesativado);

void ativoAte(char *ativa, Tempo fim, Tempo horaAtual, Tempo t0);


void arrumaTempo(Tempo *tmp) {
  tmp->realT = tmp->hora * 60 * 60 + tmp->minu * 60 + tmp->seg + tmp->dia * 24 * 60 * 60;
}

void manterEntre(char *ativa, int min, int max, float atual) {
  if (atual <= min) {
    *ativa = 1;
  } else if (atual >= max) {
    *ativa = 0;
  }
}

void ativarTomada(char parametro, char *ativa, DadosParametro *myparamentros, DadosSensores *myDados,  char sensorDesativado) {
  if (!sensorDesativado) {
    switch (parametro) {
      case TEMPERATURA:
        manterEntre(ativa, myparamentros->minTemp, myparamentros->maxTemp, myDados->tempCels);

        break;
      case HUMIDADE:
        manterEntre(ativa, myparamentros->minHum, myparamentros->maxHum, myDados->hum);
        break;
      case TEMPO:
        ativoAte(ativa, myparamentros->tempo, myDados->horaAtual, t0);
        break;
      case EVERON:
        *ativa = 1;
        break;
      case EVEROFF:
        *ativa = 0;
        break;
    }
  } else {
    Serial.println("os sensores estao com defeito");
  }
}

void ativoAte(char *ativa, Tempo fim, Tempo horaAtual, Tempo t0) {
  if (horaAtual.realT - t0.realT > fim.realT) {
    *ativa = 0;
  }else {
    *ativa=1;
  }
}
