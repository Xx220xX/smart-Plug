#PLUG
 [CLASSE SMT_DATA_TASK](#data_task_smt)


<a name="data_task_smt"></a>
### SMT_DATA_TASK
| Comando |  ação |arg1  | arg2 | arg3 | arg4 | arg5 | arg6 | possui segundo plano |
 |  :----   | :----  | :---- | :---- | :---- | :---- | :---- | :---- |         :----         |
 | get time | retorna a hora | - | - | - | -  | - | -  | não   |
 | inf all | @deprecated retorna humidade, hora  e tomada está ligado ou deslida | tomada(int)  | - | - |  - | - |  - | não |
 |  info process   | retorna informacoes da tarefa sendo realizada pela tomada | -  | - | - | - | - | - | não |  
 |  get humidity   | retorna a humida lida pelo sensor | -  | - | - | - | - | - | não |  
 |  get temperature   | retorna a temperatura lida pelo sensor | -  | - | - | - | - | - | não |  
 |  -   | -  | - | - | - | - | - | - | - |  
 |  blink   | alterna entre ligado e desligado no periodo de 2 vezes o tempo passado | tomada(int)  | milissegundos(int) | - | - | - | - | sim |  
 |  off   | desliga a tomada | tomada(int)  | - | - | - | - | - | não |  
 |  on   | liga a tomada | tomada(int)  | - | - | - | - | - | não |  
 |  alarme   | liga a tomada nos dias da semana passada no horario passado| tomada(int)  | hora(int) | minuto(int) | segundo(int) | dias da semana(string) | periodo em millis para ficar ligado(int) | não |  
 |  refrigera   | liga a tomada caso a temperatura do sensor supere a passsada | tomada(int)  | temperatura maxima aceitavel(float)  | - | - | - | - | sim  |  
 |  aquece   | liga a tomada caso a temperatura do sensor fique menor que  a temperatura passada passsada | tomada(int)  | temperatura minima aceitavel(float)  | - | - | - | - | sim  |  
 |  humidade entre   | liga a tomada caso a humidade esteja entre os valores min e max | tomada(int)  | humidade minima(float)  | humidade maxima(float) | - | - | - | sim  |  
 |  on for   | liga a tomada pelo tempo enviado depois muda para off | tomada(int)  | milissegundos(int)  | - | - | - | - | sim  |
 