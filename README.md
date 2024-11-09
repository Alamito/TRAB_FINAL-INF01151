COMANDOS DOCKER:
    Antes devemos entrar na pasta do Dockerfile
    - docker build -t servidor-udp .
    - docker run --rm -it servidor-udp

DESCOBRIR IP DO DOCKER:
    - docker ps
        - pegar CONTAINER ID (como b260c9bf3467)
    - docker exec -it b260c9bf3467 hostname -I

Testar envio de mensagens?
    Exemplo em MessageTest.cpp
