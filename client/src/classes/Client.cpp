#include "Client.h"

#include "Socket.h"
#include "Message.h"

#include <iostream>

using namespace std;

void Client::sendSumRequisition(Message* mensagem, int numRequisition){
    mensagem -> setType(/*Define aqui o tipo 'sum'*/);
    mensagem -> setNumToSum(numRequisition);
    mensagem -> send();
    mensagem -> waitAck();

    /*Atualizar os atributos do objeto aqui.
    Verificar o retorno da mensagem*/
}

int Client::listenTerminal(){
    int userNum;
    cin >> userNum;
    return userNum;
}

void Client::discoverServer(Message* mensagem){

    string retorno;
    mensagem -> setType(/*Define aqui o tipo 'discover'*/);
    mensagem -> send();

    /*Conferir como funciona o retorno da mensagem para atribuir
    corretamente aos atributos do objeto*/

    retorno = mensagem -> waitAck();
    this->serverAdress = retorno;
}