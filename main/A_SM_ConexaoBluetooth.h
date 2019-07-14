#ifndef CONNECTION_BT_H
#define CONNECTION_BT_H


BluetoothSerial SerialBT;

int contains(char *str1, const char *str2);

void analizarParametros(char *comandos);

void BLUETOOTH_servicos();

int capturarFloat(char *str, float *buff);

void configurar(char *string);

int capturaTempo(char *comandos, Tempo *ptr);

int capturaTime(char *str, Time *ptr);
void enviarHora();
void iniciaBluetooth() {
  SerialBT.begin("SmartPlug");
}

#define CONTEM(x)\
  (i=contains(string,x))

void configurar(char *string) {
  //mudar nome wifi
  //sincronizar hora
  //responder a chamados
  int i;
  if (CONTEM("verify")) {
    bluetooth_sendMsg("#confirmar");
  } else if (CONTEM("getDados")) {
    printaRDados(String("#0"), meusSensores);
  } else if (CONTEM("getModo")) {
    bluetooth_sendMsgString(String("#1") + String(condicoes.meuParametro, DEC));
  } else if (CONTEM("getParametro")) {
    enviarParametros(String("#2"), 0);
  } else if (CONTEM("getPr3")) {
    enviarParametros(String("#3"), 3);
  } else if (CONTEM("getPr4")) {
    enviarParametros(String("#4"), 4);
  } else if (CONTEM("getPr5")) {
    enviarParametros(String("#5"), 5);
  } else if (CONTEM("getPr6")) {
    enviarParametros(String("#6"), 6);
  } else if (CONTEM("getPr7")) {
    enviarParametros(String("#7"), 7);
  } else if (CONTEM("getHour")) {
    enviarHora();
  }

}

#undef CONTEM

void BLUETOOTH_servicos() {
  char receive[51] = {0};
  int i;
  for (;;) {
    i = 0;
    if (SerialBT.available()) {
      while (SerialBT.available() && i < 50) {
        receive[i] = SerialBT.read();
        i++;
        delay(1);
      }
      receive[i] = 0;
      analizarParametros(receive);
    }
    delay(1);
  }
}


void analizarParametros(char *comandos) {
  //primeiro caractere sera o tipo de parametro
  pausar();
  Serial.println(String("receive ---> ") + String(comandos) + String(" <"));
  int i;
  Tempo
  p;
  switch (comandos[0]) {
    //"022.30/25.52"
    case '1':
      mensagens("EVERON");
      condicoes.meuParametro = 1;
      break;
    case '2':
      mensagens("EVEROFF");
      condicoes.meuParametro = 2;
      break;
    case '3':
      mensagens("HUMIDADE Entre min e max");
      condicoes.meuParametro = 3;
      i = capturarFloat(comandos + 1, &condicoes.minHum);
      capturarFloat(comandos + 1 + i, &condicoes.maxHum);
      break;
    case '4':
      mensagens("ligar por determinado tempo");
      capturaTime(comandos + 1, &condicoes.tempoN_parametro4);
      condicoes.tempoInicial_parametro4 = Tempo(meusSensores.horaAtual);
      condicoes.meuParametro = 4;
      break;
    case '5':// 5/73949327/
      mensagens("alternar periodicamente");
      condicoes.meuParametro = 5;
      capturaTime(comandos + 1, &condicoes.periodico);
      condicoes.tempoInicial_parametro4 = Tempo(meusSensores.horaAtual);
      break;
    case '6':
      mensagens("ligar se temperatura > max");
      condicoes.meuParametro = 6;
      capturarFloat(comandos + 1, &condicoes.maxTemp);
      break;
    case '7':
      mensagens("ligar se temperatura < min ");
      condicoes.meuParametro = 7;
      capturarFloat(comandos + 1, &condicoes.minTemp);
      break;
    case '#':
      configurar(comandos + 1);
      break;
    case '*':
      capturaTempo(comandos + 1, &p);
      ajustarHora(p);
      break;
    default:
      break;
  }
  despausar();

}


int capturaTempo(char *str, Tempo *ptr) {

  // /long  seg,
  unsigned long buff = 0;
  char c = 0;
  int i = 0;
  buff = 0;
  //263/
  for (i = 0;  str[i] && str[i] != '/'; i++) {
    c = str[i];
    if (c < '0' || c > '9')break;
    buff *= 10;
    buff += (c - 48);
  }
  i++;
  *ptr = Tempo(buff);
  return i;
}

int capturaTime(char *str, Time *ptr) {
  while (*str == '/') {
    str = str + 1;
  }
  // /long  seg,
  unsigned long buff = 0;
  char c = 0;
  int i = 0;
  buff = 0;
  //263/
  for (i = 0;  str[i] && str[i] != '/'; i++) {
    c = str[i];
    if (c < '0' || c > '9')break;
    buff *= 10;
    buff += (c - 48);
  }
  i++;

  *ptr = buff;
  return i;
}

#define CONTEM(x) \
  contains(string,x)


int capturarFloat(char *str, float *buff) {
  char c = 0;
  int i = 0, divisor = 1, dividir = 0;
  *buff = 0;
  //26.3/
  for (i = 1;  str[i] && str[i] != '/'; i++) {
    c = str[i];

    if (c == '.' || c == ',') {
      dividir = 1;
      continue;
    }
    if (c < '0' || c > '9')break;
    if (dividir) {
      divisor *= 10;
    }

    *buff *= 10;
    *buff += (c - '0');
  }
  *buff = *buff / divisor;
  i++;//sair de cima do '/'
  return i;
}

void capturarTempo(char *comandos, Tempo *tempo) {
  int i = 1;
  //segundo de tempoN_parametro4 ligado
  uint32_t tmp_segundos = 0;
  for (i = 1;  comandos[i] && comandos[i] != '/'; i++) {
    tmp_segundos = tmp_segundos * 10 + comandos[i] - 48;
  }
  *tempo = Tempo(tmp_segundos);
}

int contains(char *str1, const char *str2) {
  int i, j, k, flag = 0;
  for (i = 0; str1[i]; i++) {
    k = i;
    for (j = 0; str2[j]; j++) {
      if (str1[k]) {
        if (str2[j] == str1[k]) {
          flag = 1;
          k++;
          continue;
        } else {
          flag = 0;
          break;
        }
      }
    }
    if (flag) {
      return k;
    }
  }
  return 0;
}

void bluetooth_sendMsg(const char *msg) {
  bluetooth_sendMsgString(String(msg));
}


void bluetooth_sendMsgString(String msg) {
  Serial.println(String("::>") + msg);
  int i = 0, j = msg.length();
  if (j >= 100) {
    Serial.println("!!!!!!!!!!ERROR!!!!! mensage length unsupported\n!!!!!!!!!!!!!!! Missing data");
  }
  for (; i < j && j < 100; i++)
    SerialBT.write(msg.charAt(i));
  for (; j < 100; j++) {
    SerialBT.write((char) 1);
  }

}

void enviarParametros(String add, int modo) {
  String sstr = add;
  if (!modo) {
    modo = condicoes.meuParametro;
  }
  switch (modo) {
    case 3:
      sstr += String("Humidade minima = ");
      sstr += String(condicoes.minHum);
      sstr += String("\nHumidade maxima = ");
      sstr += String(condicoes.maxHum);
      break;
    case 4:
      sstr += String("Tempo = ");
      sstr += String(condicoes.tempoN_parametro4);
      break;
    case 5:
      sstr = String("Periodo = ");
      sstr += String(condicoes.periodico);
      break;
    case 6:
      sstr += String("Ligar se temperatura maior que:");
      sstr += String(condicoes.maxTemp);
      break;
    case 7:
      sstr += String("Ligar se temperatura menor que:");
      sstr += String(condicoes.minTemp);
      break;
    default :
      sstr += String("NULL");
      break;
  }

  bluetooth_sendMsgString(sstr);
}

void enviarHora() {

  String sHour = String("");
  uint64_t nHour = meusSensores.horaAtual.Epoch64Time();
  int n = 0;
  while (nHour > 0) {
    n = nHour % 10;
    nHour = nHour / 10;
    sHour = String(n,DEC)+sHour;
  }
  bluetooth_sendMsgString(String("#A") +sHour);
}
#endif
