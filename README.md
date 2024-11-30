# COMANDOS DOCKER:
    Antes devemos entrar na pasta do Dockerfile
    - docker build -t servidor-udp .
    - docker run --rm -it servidor-udp

# DESCOBRIR IP DO DOCKER:
    - docker ps
        - pegar CONTAINER ID (como b260c9bf3467)
    - docker exec -it b260c9bf3467 hostname -I

# Testar envio de mensagens?
    Exemplo em MessageTest.cpp

# Fazer o CMAKE funcionar
    - adicionar os caminhos no arquivo CMakeLists.txt
    - criar pasta build, caso ela não exista
        -- dentro de build: `cmake ..`

## Para rodar o programa com Cmake
    - dentro de build: `make`
    - dentro de build: `./"nome do executavel que esta no CMakeLists.txt"`

    