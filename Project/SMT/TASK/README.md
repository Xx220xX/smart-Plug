#PLUG
 [CLASSE SMT_DATA_TASK](#data_task_smt)


<a name="data_task_smt"></a>
### SMT_DATA_TASK
| Comando | arg1  | arg2 | arg3 | arg4 | arg5 | arg6 | ação | possui segundo plano |
 |  ----   | ----  | ---- | ---- | ---- | ---- | ---- | ---- |         ----         |
 | get time | - | - | - | -  | - | -  | retorna a hora | não   |
 | inf all | tomada(int)  | - | - |  - | - |  - | @deprecated retorna humidade, hora  e tomada está ligado ou deslida | não |
 |  info process   | -  | - | - | - | - | - | retorna informacoes da tarefa sendo realizada pela tomada | não |  
 |  get humidity   | -  | - | - | - | - | - | retorna a humida lida pelo sensor | não |  
 |  get temperature   | -  | - | - | - | - | - | retorna a temperatura lida pelo sensor | não |  
 |  -   | -  | - | - | - | - | - | - | - |  
 |  blink   | tomada(int)  | milissegundos(int) | - | - | - | - | alterna entre ligado e desligado no periodo de 2 vezes o tempo passado | sim |  
 |  off   | tomada(int)  | - | - | - | - | - | desliga a tomada | não |  
 |  on   | tomada(int)  | - | - | - | - | - | liga a tomada | não |  
 |  alarme   | tomada(int)  | hora(int) | minuto(int) | segundo(int) | dias da semana(string) | periodo em millis para ficar ligado(int) | liga a tomada | não |  
 |  refrigera   | tomada(int)  | temperatura maxima aceitavel(float)  | - | - | - | - | liga a tomada caso a temperatura do sensor supere a passsada | sim  |  
 |  aquece   | tomada(int)  | temperatura minima aceitavel(float)  | - | - | - | - | liga a tomada caso a temperatura do sensor fique menor que  a temperatura passada passsada | sim  |  
 |  humidade entre   | tomada(int)  | humidade minima(float)  | humidade maxima(float) | - | - | - | liga a tomada caso a humidade esteja entre os valores min e max | sim  |  
 |  on for   | tomada(int)  | milissegundos(int)  | - | - | - | - | liga a tomada pelo tempo enviado depois muda para off | sim  |
 