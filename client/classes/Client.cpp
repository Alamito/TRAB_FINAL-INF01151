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

    while (!waitedType){
        waitedType = 0;
        do {
            sockHandler.send(&sumPacket, sizeof(sumPacket));
            ackReceived = sockHandler.receive(buf, SIZE_BUFFER, &ipServerAddr);
        } while (ackReceived <= 0);

        memcpy(&packetReceived, buf, sizeof(packet));
        if (packetReceived.type == REQ_ACK) {
            if(packetReceived.ack.seqn == this->lastReq+1)
                waitedType = 1;
        }
    }

    this->lastReq = packetReceived.ack.seqn;
    this->lastSum = packetReceived.ack.total_sum;

    cout << __DATE__ << " " << __TIME__ << " " << "server " << serverAdress << " id_req " << lastReq << " value " << numToSum << " num_reqs " << packetReceived.ack.num_reqs << " total_sum " << lastSum << endl;

}

void Client::discoverServer() {
    char buf[SIZE_BUFFER]; 
    int ackReceived = -1;
    sockaddr_in serverAddr;

    packet discoverPacket;
    discoverPacket.type = DESC;

    do {
        sockHandler.send(&discoverPacket, sizeof(discoverPacket));
        ackReceived = sockHandler.receive(buf, SIZE_BUFFER, &serverAddr);
    } while (ackReceived <= 0);

    char ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(serverAddr.sin_addr), ip, INET_ADDRSTRLEN);
    serverAdress = std::string(ip);

    std::string data, hora;
    cout << __DATE__ << " " << __TIME__ << " " << "server_addr " << serverAdress << endl;
}

// Getters
string Client::getServerAdress(){
    return this->serverAdress;
}