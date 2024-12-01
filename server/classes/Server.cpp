#include "Server.h"
#include <cstring>
#include <cstdlib>  // Para system()


/*
  essa porta precisa ser passada como argumento na chamada 
  do terminal!!  
*/
const int myPORT = 8080;

using namespace std; 

Server::Server()
    : socketHandler(myPORT),
      sumTable(),
      clientsTable()
{
    
    this->socketHandler.create();
    
}

sockaddr_in Server::receiveMessage(packet * packetReceived_pt) {
    char buf[SIZE_BUFFER]; 
    sockaddr_in clientAddr;
    buf[0] = '\0';

    while(1){
        this->socketHandler.receive(buf, SIZE_BUFFER, &clientAddr);
        memcpy(packetReceived_pt, buf, sizeof(packet));

        if(buf[0] != '\0'){
            //char clientIp[INET_ADDRSTRLEN];
            //inet_ntop(AF_INET, &(clientAddr.sin_addr), clientIp, INET_ADDRSTRLEN);
            return clientAddr; // retorna as infos do client
        }
    }          
} 

void Server::sumRequisitionResponse(int value, int seqn, sockaddr_in * sockClient){
    
    
    char clientIp[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(sockClient->sin_addr), clientIp, INET_ADDRSTRLEN);

    /*busca cliente na tabela de clients*/
    clientData auxClient = clientsTable.getClient(clientIp);
        
    /*caso a requiquisao esteja repetida*/
    if(auxClient.lastReq >= seqn){
        cout << "requisicao repitida" << endl; 

        /*printa as tabelas*/
        this->clientsTable.printTable();
        this->sumTable.printTable();

        auxClient.lastReq = seqn;
        cout << endl << "client " << clientIp << " id_req " << seqn << " value " << value << " total sum " << auxClient.totalSum << endl;

    //    /*envia Ack com campos antigos*/
        this->sendMessageAck(auxClient, sockClient);
        return; 
    }


    /*envia Ack com novos campos*/
    auxClient.lastReq = seqn;
    auxClient.lastSum = value;
    auxClient.totalSum = this->sumTable.updateTable(value);
    this->sendMessageAck(auxClient, sockClient);
    

    /*printa as tabelas*/
    this->clientsTable.updateClient(clientIp, seqn, value, this->sumTable.getSum()); 
    this->clientsTable.printTable();
    this->sumTable.printTable();
    cout << endl << "client " << clientIp << " id_req " << seqn << " value " << value << " total sum " << auxClient.totalSum << endl;
}

void Server::discoverRequisitionResponse(sockaddr_in * sockClient){
    
    char clientIp[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(sockClient->sin_addr), clientIp, INET_ADDRSTRLEN);

    cout << "discoverRequisitionResponse to: " << clientIp << endl;
    clientData client = {0,0,0,clientIp};


    /*manda o ack independentemente*/
    this->sendDiscoverAck(sockClient);
    this->clientsTable.addClient(client);
    this->clientsTable.printTable();

    //system("clear");  // Limpa a tela no terminal
}


void Server::sendMessageAck(clientData client, sockaddr_in * sockClient) {

    //cout << endl << "Ip pra mandar: " << client.IP << endl;
    //cout << "Requisition Number: " << client.lastReq << endl;
    //cout << "Ultimo valor pedido: " << client.lastSum << endl; 
    //cout << "Soma total: " << client.totalSum << endl << endl;
    packet ackPacket = {0}; // Inicializa todos os campos com zero
    ackPacket.type = REQ_ACK;
    ackPacket.ack.total_sum = client.totalSum;
    ackPacket.ack.seqn = client.lastReq;
    ackPacket.ack.num_reqs = this->sumTable.getRequests();

    //std::cout << "Enviando ACK para IP: " << client.IP
    //          << ", Seqn: " << ackPacket.ack.seqn
    //          << ", Total Sum: " << ackPacket.ack.total_sum << std::endl;

    this->socketHandler.send(&ackPacket, sizeof(packet), sockClient);
}
    //nao precisa criar mais threads, a de soma do servidor ja eh uma thread


void Server::sendDiscoverAck(sockaddr_in * sockClient) {
    cout << "mandando mensagem de Discover de requisicao" << endl; 
    packet ackPacket;
    ackPacket.type = DESC_ACK;

    /*
    criando a estrutura que contem os dados do cliente 
    que recebera o pacote
    */
    this->socketHandler.send(&ackPacket, sizeof(packet), sockClient);
}


