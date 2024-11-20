#include "Client.h"

#include "Socket.h"
#include "packets.h"

#include <iostream>

using namespace std;

Client::Client(){
    lastReq = 0;
    lastSum = 0;
    //serverAdress = "255.255.255.255";
    serverAdress = "143.54.68.153";

    sockHandler = Socket("127.0.0.1", 4000);
    sockHandler.create();
}

void Client::sendSumRequisition(int numToSum){

    printf("1\n");
    packet sumPacket, packetReceived;
    sumPacket.req.value = numToSum;
    sumPacket.type = REQ;
    sumPacket.seqn = lastReq;
    string ipServer;

    printf("2\n");
    char buf[SIZE_BUFFER];
    int ackReceived = -1;
    int waitedType = 0;

    while (!waitedType){
        printf("3\n");
        do{
             printf("4\n");
            sockHandler.send(&sumPacket, sizeof(sumPacket), serverAdress, 4000);
            ackReceived = sockHandler.receive(buf, SIZE_BUFFER, ipServer);
            
        }while (ackReceived <= 0);

        printf("5\n");
        memcpy(&packetReceived, buf, sizeof(packet));
        if (packetReceived.ack.seqn == lastReq)
            waitedType = 1;
    }

    lastReq = packetReceived.ack.seqn + 1;
    lastSum = packetReceived.ack.total_sum;

    printf("Requisicao enviada ao servidor!\n");
    printf("Somatorio atual: %d \n", lastSum);
    printf("Total de requisicoes enviadas ao servidor: %d\n\n", packetReceived.ack.num_reqs);

}

int Client::listenTerminal(){
    int userNum;
    cin >> userNum;
    return userNum;
}

void Client::discoverServer(){
    
    char buf[SIZE_BUFFER]; 
    int ackReceived = -1;

    packet discoverPacket;
    discoverPacket.type = DESC;

    do{
        sockHandler.send(&discoverPacket, sizeof(discoverPacket), serverAdress, 4000);
        ackReceived = sockHandler.receive(buf, SIZE_BUFFER, serverAdress);
    }while (ackReceived <= 0);

    return;
}



//Getters

string Client::getServerAdress(){
    return this -> serverAdress;
}

//CÓDIGO ANTIGO

// void Client::sendSumRequisition(Message* mensagem, int numRequisition){
//     mensagem -> setType("sum");
//     mensagem -> setNumberToSum(numRequisition);
//     mensagem -> send(0);
//     mensagem -> waitAck();

//     /*Atualizar os atributos do objeto aqui.
//     Verificar o retorno da mensagem*/

// }

// int Client::listenTerminal(){
//     int userNum;
//     cin >> userNum;
//     return userNum;
// }

// void Client::discoverServer(Message* mensagem){

//     ServerResponse retorno;
//     mensagem -> setType("discover");
//     mensagem -> send(1);
//     cout << "Mandou a mensagem Broadcast" << endl;

//     retorno = mensagem -> waitAck();

//     if (retorno.serverIp == ""){
//         return;
//     }
//     else{
//         this->serverAdress = retorno.serverIp;
//         mensagem -> setRecipient(retorno.serverIp);
//     }
// }