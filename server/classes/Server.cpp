#include "Server.h"
#include <cstring>
#include <cstdlib>  // Para system()

//criar o server passando IP e porta

//std::string myIP = "172.17.0.1"; // Como std::string, não como #define
const int myPORT = 8080;

using namespace std; 

Server::Server()
    : socketHandler(myPORT),
      sumTable(),
      clientsTable()
{
    this->socketHandler.create();
    //this->socketHandler.setBroadcastEnable(0);
    //this->socketHandler.bind(); 
}

std::string Server::receiveMessage(packet * packetReceived_pt) {
    char buf[SIZE_BUFFER]; 
    sockaddr_in clientAddr;
    buf[0] = '\0';

    while(1){
        this->socketHandler.receive(buf, SIZE_BUFFER, &clientAddr);
        memcpy(packetReceived_pt, buf, sizeof(packet));

        if(buf[0] != '\0'){
            char clientIp[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &(clientAddr.sin_addr), clientIp, INET_ADDRSTRLEN);
            return std::string(clientIp); // retorna de quem recebeu a mensagem
        }
    }          
} 

void Server::sumRequisitionResponse(int value, int seqn, string clientIp){
    
    clientData auxClient = clientsTable.getClient(clientIp);
    auxClient.IP = clientIp;
        
    if(auxClient.lastReq == seqn){
        //cout << "requisicao repitida" << endl; 

        /*limpar tela*/
        //system("clear");  // Limpa a tela no terminal
        this->clientsTable.printTable();
        this->sumTable.printTable();
        cout << endl << "client " << clientIp << " id_req " << seqn << " value " << value << " total sum " << auxClient.totalSum << endl;


        //this->sendMessageAck(auxClient);
        return; 
    }

    auxClient.lastReq = seqn;
    auxClient.lastSum = value;
    auxClient.totalSum = this->sumTable.updateTable(value);
    
    //this->sendMessageAck(auxClient);
    
    this->clientsTable.updateClient(clientIp, seqn, value, this->sumTable.getSum()); 
    
    /*limpar tela*/
    //system("clear");  // Limpa a tela no terminal
    this->clientsTable.printTable();
    this->sumTable.printTable();
    cout << endl << "client " << clientIp << " id_req " << seqn << " value " << value << " total sum " << auxClient.totalSum << endl;
    //uint16_t seqn;      //Número de sequência que está sendo feito o ack
    //uint16_t num_reqs;  // Quantidade de requisições
    //uint16_t total_sum; 
}

void Server::discoverRequisitionResponse(const std::string& clientIp){
    clientData client = {0,0,0,clientIp};
    /*manda o ack independentemente*/
    //this->sendDiscoverAck(clientIp);
    this->clientsTable.addClient(client);

    //system("clear");  // Limpa a tela no terminal
    this->clientsTable.printTable();
}


void Server::sendMessageAck(clientData client) {

    //cout << endl << "Ip pra mandar: " << client.IP << endl;
    //cout << "Requisition Number: " << client.lastReq << endl;
    //cout << "Ultimo valor pedido: " << client.lastSum << endl; 
    //cout << "Soma total: " << client.totalSum << endl << endl;
    packet ackPacket = {0}; // Inicializa todos os campos com zero
    ackPacket.type = REQ_ACK;
    ackPacket.ack.total_sum = client.totalSum;
    ackPacket.ack.seqn = client.lastReq;
    ackPacket.ack.num_reqs = this->sumTable.getRequests();

    sockaddr_in clientAddr;
    inet_pton(AF_INET, client.IP.c_str(), &clientAddr.sin_addr);
    clientAddr.sin_family = AF_INET;
    clientAddr.sin_port = htons(myPORT);

    std::cout << "Enviando ACK para IP: " << client.IP
          << ", Seqn: " << ackPacket.ack.seqn
          << ", Total Sum: " << ackPacket.ack.total_sum << std::endl;


    this->socketHandler.send(&ackPacket, sizeof(packet), &clientAddr);

}
    //nao precisa criar mais threads, a de soma do servidor ja eh uma thread


void Server::sendDiscoverAck(const std::string& clientIp) {
    cout << "mandando mensagem de Discover de requisicao" << endl; 
    packet ackPacket;
    ackPacket.type = DESC_ACK;

    sockaddr_in clientAddr;
    inet_pton(AF_INET, clientIp.c_str(), &clientAddr.sin_addr);
    clientAddr.sin_family = AF_INET;
    clientAddr.sin_port = htons(myPORT);

    std::cout << "Enviando ACK para IP: " << clientIp
        << ", Seqn: " << ackPacket.ack.seqn
        << ", Total Sum: " << ackPacket.ack.total_sum << std::endl;

    this->socketHandler.send(&ackPacket, sizeof(packet), &clientAddr);
}


