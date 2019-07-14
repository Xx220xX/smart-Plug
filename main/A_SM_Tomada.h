#ifndef TMDPLUG_H
#define  TMDPLUG_H


void TMD_ativar(int ativa) {
  digitalWrite(PIN_TO_PLUG, ativa);
}

int TMD_getPower() {
  return ativa;
}

void changeTypeOFF() {
  condicoes.meuParametro = 2;
}

void setPower(int ativo) {
  ativa = ativo;
}


void manterEntre(char *ativa, int min, int max, float atual) {
  if (atual <= min) {
    *ativa = 1;
  } else if (atual >= max) {
    *ativa = 0;
  }
}

void humEntreMiniEmax(float l, float h, float at);


void ligarPorNtempo(Tempo inicial, Time tempoN, Tempo atual);

void ligarPeriodico(Time *periodico, Tempo *atual);

void ligarTempMAIOR(float max, float atual);

void ligarTempMIN(float min, float atual);

void TMD_controle(int parametro, DadosParametro *myparamentros, DadosSensores *myDados, char sensorDesativado) {
  if (parametro == 3) {
    if (sensorDesativado) {
      mensagens("falha no sensor de Temperatura");
    } else {
      //HUMIDADE ENTRE MIN E MAX
      humEntreMiniEmax(myparamentros->minHum, myparamentros->maxHum, myDados->hum);
    }
  }
  switch (parametro) {
    case 1://EVERON
      setPower(PLUG_ON);
      break;
    case 2://EVEROFF
      setPower(PLUG_OFF);
      break;
    case 4://Ligar por N tempoN_parametro4
      ligarPorNtempo(myparamentros->tempoInicial_parametro4, myparamentros->tempoN_parametro4,
                     myDados->horaAtual);
      break;
    case 5://LIGAR periodico comecando ON/OFF
      ligarPeriodico(&myparamentros->periodico, &myDados->horaAtual);
      break;
    case 6://Ligar se temperatura >max(refrigeracao
      ligarTempMAIOR(myparamentros->maxTemp, myDados->tempCels);
      break;
    case 7://Ligar se temperatura <min(aquecimento)
      ligarTempMIN(myparamentros->minTemp, myDados->tempCels);
    default:
      break;
  }


}

void ligarTempMIN(float min, float atual) {
  if (atual <= min) {
    setPower(PLUG_ON);
  } else {
    setPower(PLUG_OFF);
  }
}

void ligarTempMAIOR(float max, float atual) {
  if (atual >= max) {
    setPower(PLUG_ON);
  } else {
    setPower(PLUG_OFF);
  }

}






void ligarPeriodico(Time *periodico, Tempo *atual) {

  if (atual->Epoch32Time() - condicoes.tempoInicial_parametro4.Epoch32Time() > *periodico) {
    setPower(!TMD_getPower());
    condicoes.tempoInicial_parametro4 = Tempo(*atual);
  }

}

void ligarPorNtempo(Tempo inicial, Time tempoN, Tempo atual) {
  if (atual.Epoch64Time() - inicial.Epoch64Time() > tempoN) {

    changeTypeOFF();
  } else {
    setPower(PLUG_ON);
  }
}




void humEntreMiniEmax(float l/*LOW*/, float h/*HIGH*/ , float at/*ATUAL*/) {
  if (at < l) {
    setPower(PLUG_ON);
  } else if (at > h) {
    setPower(PLUG_OFF);
  }
}

#endif
