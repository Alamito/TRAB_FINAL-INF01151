#include "Server.h"
#include <cstring>
//criar o server passando IP e porta

std::string myIP = "127.0.0.1"; // Como std::string, não como #define
const int myPORT = 4000;

using namespace std; 

Server::Server()
    : socketHandler(myIP, myPORT),
      sumTable(),
      clientsTable()
{
    this->socketHandler.create(); 
    this->socketHandler.bind(); 
}

std::string Server::receiveMessage(packet * packetReceived_pt) {

    char buf[SIZE_BUFFER]; 
    string clientIp; 

    buf[0] = '\0';

    while(1){
        this->socketHandler.receive(buf, SIZE_BUFFER, clientIp);
        memcpy(packetReceived_pt, buf, sizeof(packet));

        if(buf[0] != '\0'){
            printf("tipo = %i", packetReceived_pt->type); 
            printf("value = %i", packetReceived_pt->req.value); 
            return clientIp; //retorna de quem recebeu a mensagem
        }
    }          
}    

void Server::sumRequisitionResponse(int value, int numReq, string clientIp){
    this->sumTable.updateTable(value);
    
    this->sendMessageAck(clientIp);
    
    this->clientsTable.updateClient(clientIp, numReq, this->sumTable.getSum()); 

    //uint16_t seqn;      //Número de sequência que está sendo feito o ack
    //uint16_t num_reqs;  // Quantidade de requisições
    //uint16_t total_sum; 
}

void Server::discoverRequisitionResponse(const std::string& clientIp){
    clientData client = {0,0,clientIp};
    this->sendDiscoverAck(clientIp);
    this->clientsTable.addClient(client);
}


void Server::sendMessageAck(const std::string& clientIp) {

}
    //nao precisa criar mais threads, a de soma do servidor ja eh uma thread
void Server::sendDiscoverAck(const std::string& clientIp, ) {

}



