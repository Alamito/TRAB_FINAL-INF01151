#include "Client.h"
#include "Socket.h"
#include "packets.h"
#include <iostream>
#include <time.h>

using namespace std;

Client::Client(int port)
    : serverAdress("255.255.255.255"), sockHandler(port, serverAdress) { // Inicialização de sockHandler
    lastReq = 0;
    lastSum = 0;

    sockHandler.create();

    /*cout << " ----------------------------------------------" << endl;
    cout << "|      Cliente iniciado na porta " << port << "          |"  << endl;
    cout << " ----------------------------------------------" << endl;*/
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

    while (!waitedType){//(!waitedSeq || !waitedType) {
        //waitedSeq = 0;
        waitedType = 0;
        do {
            sockHandler.send(&sumPacket, sizeof(sumPacket));
            ackReceived = sockHandler.receive(buf, SIZE_BUFFER, &ipServerAddr);
        } while (ackReceived <= 0);

        memcpy(&packetReceived, buf, sizeof(packet));
        // if (packetReceived.ack.seqn == lastReq + 1) {
        //     waitedSeq = 1;
        // }
        if (packetReceived.type == REQ_ACK) {
            if(packetReceived.ack.seqn == this->lastReq+1)
                waitedType = 1;
        }
        //std::cout << "seqn " << packetReceived.ack.seqn << std::endl;
    }

    this->lastReq = packetReceived.ack.seqn;
    this->lastSum = packetReceived.ack.total_sum;

    //printf("Requisicao enviada ao servidor!\n");
    //printf("Somatorio atual: %d \n", lastSum);
    //printf("Total de requisicoes enviadas ao servidor: %d\n\n", packetReceived.ack.num_reqs);

    //cout << "Sum_server " << lastSum << " seqn " << lastReq << endl;
    cout << __DATE__ << " " << __TIME__ << " " << "server " << serverAdress << " id_req" << lastReq << " value " << numToSum << " num_reqs " << packetReceived.ack.num_reqs << " total_sum " << lastSum << endl;

    //cout << " ----------------------------------------------" << endl << endl;
}

void Client::discoverServer() {
    char buf[SIZE_BUFFER]; 
    int ackReceived = -1;
    sockaddr_in serverAddr;

    packet discoverPacket;
    discoverPacket.type = DESC;

    //sockHandler.setReceiveTimeout(&discoverPacket, sizeof(discoverPacket));

    //cout << "Tentando encontrar o servidor..." << endl;
    do {
        sockHandler.send(&discoverPacket, sizeof(discoverPacket));
        ackReceived = sockHandler.receive(buf, SIZE_BUFFER, &serverAddr);
        //cout << "Reenviando descoberta..." << endl;
    } while (ackReceived <= 0);

    char ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(serverAddr.sin_addr), ip, INET_ADDRSTRLEN);
    serverAdress = std::string(ip);

    //cout << "IP do servidor encontrado! " << endl;
    //cout << "IP do servidor: " << serverAdress << endl << endl;

    //cout << " ----------------------------------------------" << endl << endl;
    std::string data, hora;
    cout << __DATE__ << " " << __TIME__ << " " << "server_addr " << serverAdress << endl;
}

// Getters
string Client::getServerAdress(){
    return this->serverAdress;
}