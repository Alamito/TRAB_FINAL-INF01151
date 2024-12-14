#include "Server.h"
#include <cstring>
#include <cstdlib>  
#include <time.h>


using namespace std; 

Server::Server(int port)
    : socketHandler(port),
      sumTable(),
      clientsTable()
{
    this->socketHandler.create();   
    cout << __DATE__ << " " << __TIME__ << "num_reqs 0 total_sum 0" << endl;
}

sockaddr_in Server::receiveMessage(packet * packetReceived_pt) {
    char buf[SIZE_BUFFER]; 
    sockaddr_in clientAddr;
    int received = -1;

    buf[0] = '\0';
    this->socketHandler.receive(buf, SIZE_BUFFER, &clientAddr);
    memcpy(packetReceived_pt, buf, sizeof(packet));

    if(buf[0] != '\0'){
        return clientAddr; // retorna as infos do client
    }      

    // A estrutura srcAddr está vazia
    clientAddr.sin_family = 0;
    clientAddr.sin_port = 0;
    clientAddr.sin_addr.s_addr = 0; 

    return clientAddr;
} 

void Server::sumRequisitionResponse(int value, int seqn, sockaddr_in * sockClient){
    
    int valueCopy = value; 
    int seqnCopy = seqn; 

    sockaddr_in* clientAddr = new sockaddr_in;
    // Copia os dados de sockClient para a nova estrutura
    *clientAddr = *sockClient;

    char clientIp[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(clientAddr->sin_addr), clientIp, INET_ADDRSTRLEN);


    /*busca cliente na tabela de clients*/
    clientData auxClient = clientsTable.getClient(clientIp);
        

    /*caso a requiquisao esteja repetida*/
    if(auxClient.lastReq >= seqnCopy){
        this->sendMessageAck(auxClient, clientAddr);
        delete clientAddr;
        return; 
    }


    /*envia Ack com novos campos*/
    auxClient.lastReq = seqnCopy;
    auxClient.lastSum = valueCopy;
    auxClient.totalSum = this->sumTable.updateTable(valueCopy);
    this->clientsTable.updateClient(clientIp, seqnCopy, valueCopy, this->sumTable.getSum()); 
    this->sendMessageAck(auxClient, clientAddr);
    

    /*printa as tabelas*/
    delete clientAddr;

    cout << __DATE__ << " " << __TIME__ << " client " << clientIp << " id_req " << seqnCopy << " total_sum " <<  auxClient.totalSum << endl;
}

void Server::discoverRequisitionResponse(sockaddr_in * sockClient){
    char clientIp[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(sockClient->sin_addr), clientIp, INET_ADDRSTRLEN);

    clientData client = {0,0,0,clientIp};


    /*manda o ack independentemente*/
    this->sendDiscoverAck(sockClient);
    this->clientsTable.addClient(client);
    //this->clientsTable.printTable();
}


void Server::sendMessageAck(clientData client, sockaddr_in * sockClient) {
    packet ackPacket;

    ackPacket.type = REQ_ACK;
    ackPacket.ack.total_sum = client.totalSum;
    ackPacket.ack.seqn = client.lastReq;
    ackPacket.ack.num_reqs = this->sumTable.getRequests();

    this->socketHandler.send(&ackPacket, sizeof(packet), sockClient);
}

void Server::sendDiscoverAck(sockaddr_in * sockClient) {
    packet ackPacket;
    ackPacket.type = DESC_ACK;

    this->socketHandler.send(&ackPacket, sizeof(packet), sockClient);
}


