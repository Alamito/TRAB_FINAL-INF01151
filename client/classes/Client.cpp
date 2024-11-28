#include "Client.h"

#include "Socket.h"
#include "packets.h"

#include <iostream>

using namespace std;

Client::Client(){
    lastReq = 0;
    lastSum = 0;
    serverAdress = "255.255.255.255" ;     //CONSTRUTOR PADRÃO
    //serverAdress = "143.54.68.153";       //ENDEREÇO JULIANA
    //serverAdress = "127.0.0.1";             //ENDEREÇO LOCAL
    //docker = ""172.17.0.2""

    sockHandler = Socket("127.0.0.1", 4000, 1);
    sockHandler.create();

    cout << " ----------------------------------------------" << endl;
    cout << "|      Cliente iniciado na porta 4000          |"  << endl;
    cout << " ----------------------------------------------" << endl;
}

void Client::sendSumRequisition(int numToSum){

    //printf("1\n");
    packet sumPacket, packetReceived;
    sumPacket.req.value = numToSum;
    sumPacket.type = REQ;
    sumPacket.seqn = lastReq;
    string ipServer;

    ///printf("2\n");
    char buf[SIZE_BUFFER];
    int ackReceived = -1;
    int waitedSeq = 0, waitedType = 0;

    while (!waitedSeq || !waitedType){
        //printf("3\n");
        do{
            //printf("4\n");
            sockHandler.setBroadcastEnable(0);
            sockHandler.send(&sumPacket, sizeof(sumPacket), serverAdress, 4000);

            ackReceived = sockHandler.receive(buf, SIZE_BUFFER, ipServer);
            
        }while (ackReceived <= 0);

        //printf("5\n");
        memcpy(&packetReceived, buf, sizeof(packet));
        //printf("%d == %d\n", packetReceived.ack.seqn, lastReq);
        if (packetReceived.ack.seqn == lastReq){
            waitedType = 1;
            //printf("Seqn = %d \n", packetReceived.ack.seqn);
        }   
        if (packetReceived.type == REQ_ACK){
            waitedSeq = 1;
            //printf("Type = %d \n", packetReceived.type);
        }
    }

    lastReq = packetReceived.ack.seqn + 1;
    lastSum = packetReceived.ack.total_sum;

    printf("Requisicao enviada ao servidor!\n");
    printf("Somatorio atual: %d \n", lastSum);
    printf("Total de requisicoes enviadas ao servidor: %d\n\n", packetReceived.ack.num_reqs);

    cout << " ----------------------------------------------" << endl << endl;
}

int Client::listenTerminal(){
    int userNum;

    cout << "Número a ser somado: ";
    cin >> userNum;
    return userNum;
}

void Client::discoverServer(){
    
    char buf[SIZE_BUFFER]; 
    int ackReceived = -1;

    packet discoverPacket;
    discoverPacket.type = DESC;

    cout << "Tentando encontrar o servidor..." << endl;
    do{
        sockHandler.setBroadcastEnable(1);
        sockHandler.send(&discoverPacket, sizeof(discoverPacket), serverAdress, 4000);
        ackReceived = sockHandler.receive(buf, SIZE_BUFFER, serverAdress);
    }while (ackReceived <= 0);

    cout << "IP do servidor encontrado! " << endl;
    cout << "IP do servidor: " << serverAdress << endl << endl;

    cout << " ----------------------------------------------" << endl << endl;

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
