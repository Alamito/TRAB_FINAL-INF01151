#include "Client.h"
#include "Socket.h"
#include "packets.h"
#include <iostream>

using namespace std;

Client::Client()
    : serverAdress("255.255.255.255"), sockHandler(8080, serverAdress) { // Inicialização de sockHandler
    lastReq = 0;
    lastSum = 0;

    sockHandler.create();

    cout << " ----------------------------------------------" << endl;
    cout << "|      Cliente iniciado na porta 8080          |"  << endl;
    cout << " ----------------------------------------------" << endl;
}

void Client::sendSumRequisition(int numToSum) {
    packet sumPacket, packetReceived;
    sumPacket.req.value = numToSum;
    sumPacket.type = REQ;
    sumPacket.seqn = lastReq + 1;
    sockaddr_in ipServerAddr;
    int ackReceived = -1;

    char buf[SIZE_BUFFER];
    int waitedSeq = 0, waitedType = 0;

    while (!waitedSeq || !waitedType) {
        do {
            sockHandler.send(&sumPacket, sizeof(sumPacket));
            ackReceived = sockHandler.receive(buf, SIZE_BUFFER, &ipServerAddr);
        } while (ackReceived <= 0);

        memcpy(&packetReceived, buf, sizeof(packet));
        if (packetReceived.ack.seqn == lastReq + 1) {
            waitedSeq = 1;
        }
        if (packetReceived.type == REQ_ACK) {
            waitedType = 1;
        }
    }

    lastReq = packetReceived.ack.seqn;
    lastSum = packetReceived.ack.total_sum;

    //printf("Requisicao enviada ao servidor!\n");
    printf("Somatorio atual: %d \n", lastSum);
    //printf("Total de requisicoes enviadas ao servidor: %d\n\n", packetReceived.ack.num_reqs);

    //cout << " ----------------------------------------------" << endl << endl;
}

int Client::listenTerminal(){
    int userNum;

    cout << "Número a ser somado: ";
    cin >> userNum;
    return userNum;
}

void Client::discoverServer() {
    char buf[SIZE_BUFFER]; 
    int ackReceived = -1;
    sockaddr_in serverAddr;

    packet discoverPacket;
    discoverPacket.type = DESC;

    sockHandler.setReceiveTimeout(&discoverPacket, sizeof(discoverPacket));

    cout << "Tentando encontrar o servidor..." << endl;
    do {
        sockHandler.send(&discoverPacket, sizeof(discoverPacket));
        ackReceived = sockHandler.receive(buf, SIZE_BUFFER, &serverAddr);
    } while (ackReceived <= 0);

    char ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(serverAddr.sin_addr), ip, INET_ADDRSTRLEN);
    serverAdress = std::string(ip);

    cout << "IP do servidor encontrado! " << endl;
    cout << "IP do servidor: " << serverAdress << endl << endl;

    cout << " ----------------------------------------------" << endl << endl;
}

// Getters
string Client::getServerAdress(){
    return this->serverAdress;
}