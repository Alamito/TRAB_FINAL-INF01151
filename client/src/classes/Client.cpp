#include "Client.h"

#include "Socket.h"
#include "Message.h"

#include <iostream>

using namespace std;

Client::Client(){
    lastReq = 0;
    lastSum = 0;
    serverAdress = "255.255.255.255";
}

void Client::sendSumRequisition(Message* mensagem, int numRequisition){
    mensagem -> setType("sum");
    mensagem -> setNumberToSum(numRequisition);
    mensagem -> send(0);
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

    ServerResponse retorno;
    mensagem -> setType("discover");
    mensagem -> send(1);
    cout << "Mandou a mensagem Broadcast" << endl;

    retorno = mensagem -> waitAck();

    if (retorno.serverIp == ""){
        return;
    }
    else{
        this->serverAdress = retorno.serverIp;
        mensagem -> setRecipient(retorno.serverIp);
    }
}

//Getters

string Client::getServerAdress(){
    return this -> serverAdress;
}