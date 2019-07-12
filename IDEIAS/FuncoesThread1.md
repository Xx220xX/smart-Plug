# LER SENSOR  
  - escopo :
    ```cpp
    void lerSensor( SENSORES *s , Data * read );
    ```    
  - ação :
    > le os sensores disponiveis e salva em read
    
# Verificar condições
  - escopo :
    ```cpp
    int verificarCondicoes(  Data * read, Condicoes * condicoes );
    ```    
  - ação :
    > Atravéz dos dados lidos sera avaliado qual ação  realizar,
    > retornando ``` 1 ``` para ativar ou ``` 0 ``` para desativar.
# Atuar
  - escopo :
    ```cpp
        void ativar( bool ligar );
    ```    
  - ação :
    > ativa ou não a tomada.

