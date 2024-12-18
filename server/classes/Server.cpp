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
    this->PID = getpid(); 
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

void Server::findCoordinatorMessage() {
    packet electionPacket;
    electionPacket.type = DISC_LEADER;
    electionPacket.senderPID = this->PID;

    char buf[SIZE_BUFFER];

    // envia broadcast para achar coordenador
    this->socketHandler.sendBroadcast(&electionPacket, sizeof(packet));

    // Configura um timeout para aguardar resposta
    struct timeval timeout;
    timeout.tv_sec = 2; // Tempo de espera em segundos
    timeout.tv_usec = 0;

    sockaddr_in responseAddr;

    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(this->socketHandler.getSocketFd(), &readfds);

    int coordinatorResponse = this->socketHandler.receive(buf, sizeof(packet), &responseAddr); 
    packet * receivedPacket = (packet *) buf;

    if (coordinatorResponse > 0 && receivedPacket->senderPID != this->PID) {       
        if (receivedPacket->type == COORDINATOR) {
            printf("Coordenador encontrado: PID %d, IP %s\n", receivedPacket->senderPID, inet_ntoa(responseAddr.sin_addr));
            this->setIsLeader(false);
            this->setCoordinatorIP(inet_ntoa(responseAddr.sin_addr));
            this->setCoordinatorPID(receivedPacket->senderPID);
        }
    } else {
        printf("Coordenador não encontrado, estou me proclamando Líder! PID %d, IP %s\n", this->PID, inet_ntoa(responseAddr.sin_addr));
        this->setIsLeader(true);
    }
}

void Server::sendCoordinatorMessage(sockaddr_in * sockClient) {
    packet coordinatorPacket;
    coordinatorPacket.type = COORDINATOR;
    coordinatorPacket.senderPID = this->PID;
    
    printf("Enviando mensagem de coordenador\n");

    // Envia mensagem de coordenador
    this->socketHandler.send(&coordinatorPacket, sizeof(packet), sockClient);
}

void Server::sendBackup(sockaddr_in * sockClient) {
    packet backupPacket;
    backupPacket.type = BACKUP;
    //backupPacket.senderPID = this->PID;  

    backupPacket.backupData = this->sumTable.getSum();

    printf("Enviando mensagem de backup para: %s\n", inet_ntoa(sockClient->sin_addr));

    // Envia mensagem de backup
    this->socketHandler.send(&backupPacket, sizeof(packet), sockClient);
}

bool Server::leaderTimeout(){
    return false;
}

// Definições das funções
void Server::setIsLeader(bool isLeader) {
    this->isLeader = isLeader;
}

bool Server::getIsLeader() {
    return this->isLeader;
}

void Server::setCoordinatorPID(int coordinatorPID) {
    this->coordinatorPID = coordinatorPID;
}

void Server::setCoordinatorIP(string coordinatorIP) {
    this->coordinatorIP = coordinatorIP;
}


