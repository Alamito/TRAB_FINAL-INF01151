## Compilando o programa

    Primeiramente, crie uma pasta build no diretorio root:
        - mkdir build

        OBS: Caso ja exista uma pasta build, apague-a e crie uma nova

    Acesse a pasta build e rode os seguintes comandos:
        - cmake ..
        - make

## Rodando o programa

    Dentro da pasta build, execute os seguintes comandos para executar
    o servidor e o cliente respectivamente:
        - ./Server <port>
        - ./Client <port>

    No diretorio root existe um arquivo txt com 10000 numeros aleatorios
    que somam 255539. É possível utiliza-lo como teste, passando-o como
    parâmetro:
        - ./Client <port> ./../10000-num.txt