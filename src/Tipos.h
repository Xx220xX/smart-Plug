
typedef enum {
    TEMPERATURA, HUMIDADE, TEMPO, EVERON, EVEROFF
} Parametros;

typedef struct {
  int ano, mes, dia, hora, minu, seg;
  unsigned long  realT;
} Tempo;

typedef struct {
    float minHum, maxHum, minTemp, maxTemp;
    Tempo tempo;
    char meuParametro;
    
} DadosParametro;


typedef struct {
  float hum, tempCels, tempFar;
  Tempo horaAtual;
} DadosSensores;
