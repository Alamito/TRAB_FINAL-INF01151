#include "Server.h"
#include <cstring>
#include <cstdlib>  
#include <time.h>
#include <unistd.h>

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

    // Envia broadcast para achar coordenador
    this->socketHandler.sendBroadcast(&electionPacket, sizeof(packet));

    // Configura um timeout inicial
    struct timeval timeout;
    timeout.tv_sec = 2; // Tempo total de espera em segundos
    timeout.tv_usec = 0;

    sockaddr_in responseAddr;

    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(this->socketHandler.getSocketFd(), &readfds);

    bool leaderFound = false;

    while (timeout.tv_sec > 0 || timeout.tv_usec > 0) {
        // Configurações do select
        struct timeval currentTimeout = timeout;
        int activity = select(this->socketHandler.getSocketFd() + 1, &readfds, NULL, NULL, &currentTimeout);

        if (activity > 0) {
            int coordinatorResponse = this->socketHandler.receive(buf, sizeof(packet), &responseAddr);
            if (coordinatorResponse > 0) {
                packet *receivedPacket = (packet *)buf;

                // Verifica se a mensagem não veio do próprio servidor
                bool sameIP = strcmp(inet_ntoa(responseAddr.sin_addr), inet_ntoa(this->socketHandler.getServAddr().sin_addr)) == 0;
                //ol samePID = receivedPacket->senderPID == this->PID;
                printf("IP do próprio servidor: %s\n", inet_ntoa(this->socketHandler.getServAddr().sin_addr));
                printf("IP do servidor que enviou a mensagem: %s\n", inet_ntoa(responseAddr.sin_addr));
                printf("Type: %d\n", receivedPacket->type);
                if (receivedPacket->type == COORDINATOR && !sameIP) {   
                    // Coordenador encontrado
                    printf("Coordenador encontrado: PID %d, IP %s\n", receivedPacket->senderPID, inet_ntoa(responseAddr.sin_addr));
                    this->setIsLeader(false);
                    this->setCoordinatorIP(inet_ntoa(responseAddr.sin_addr));
                    this->setCoordinatorPID(receivedPacket->senderPID);
                    leaderFound = true;
                    return;
                }

                if (receivedPacket->type == DISC_LEADER && sameIP) {
                    printf("Mensagem do próprio IP, ignorando\n");
                    // Continue esperando no loop sem se proclamar líder ainda
                }
            }
        } else if (activity == 0) {
            break;
        } else {
            perror("Erro no select");
            break;
        }

        // Atualiza o tempo restante para o próximo loop
        timeout.tv_sec = currentTimeout.tv_sec;
        timeout.tv_usec = currentTimeout.tv_usec;
    }

    // Se nenhuma resposta válida foi encontrada, se proclama líder
    if (!leaderFound) {
        printf("Timeout expirado: Coordenador não encontrado. Me proclamando Líder! PID %d, IP: %s\n", this->PID, inet_ntoa(this->socketHandler.getServAddr().sin_addr));
        this->setIsLeader(true);
    }
}


void Server::sendCoordinatorMessage(sockaddr_in * sockClient) {
    packet coordinatorPacket;
    coordinatorPacket.type = COORDINATOR;
    coordinatorPacket.senderPID = this->PID;

    printf("Salvando IP do backup\n");
    this->backupsIPs.push_back(inet_ntoa(sockClient->sin_addr));
    
    printf("Enviando mensagem de coordenador\n");

    // Envia mensagem de coordenador
    this->socketHandler.send(&coordinatorPacket, sizeof(packet), sockClient);
}

void Server::sendBackup() {
    packet backupPacket;
    backupPacket.type = BACKUP;
    backupPacket.backupData = this->sumTable.getSum();

    printf("Enviando mensagem de backup para os IPs configurados:\n");

    for (const std::string &ip : this->backupsIPs) {
        sockaddr_in destAddr;
        memset(&destAddr, 0, sizeof(destAddr));
        destAddr.sin_family = AF_INET;
        destAddr.sin_port = htons(this->socketHandler.getPort()); // Porta do servidor de destino
        inet_pton(AF_INET, ip.c_str(), &destAddr.sin_addr);

        printf("Backup sendo enviado para IP: %s\n", ip.c_str());
        this->socketHandler.send(&backupPacket, sizeof(packet), &destAddr);
    }
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


