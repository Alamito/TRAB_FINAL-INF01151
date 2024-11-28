#include "Server.h"
#include <cstring>
#include <cstdlib>  // Para system()

//criar o server passando IP e porta

std::string myIP = "172.17.0.1"; // Como std::string, não como #define
const int myPORT = 4000;

using namespace std; 

Server::Server()
    : socketHandler(myIP, myPORT),
      sumTable(),
      clientsTable()
{
    this->socketHandler.createSocketToServer();
    // this->socketHandler.setBroadcastEnable(0);
    // this->socketHandler.bind(); 
}

void Server::run() {
    char buffer[BUFFER_SIZE];
    socklen_t len = sizeof(this->socketHandler.cliaddr);

    packet packetReceived;
    while (true) {
        
        int n = recvfrom(this->socketHandler.sockfd, &packetReceived, sizeof(packetReceived), 0, (struct sockaddr *) &this->socketHandler.cliaddr, &len);

        char client_ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &(this->socketHandler.cliaddr.sin_addr), client_ip, INET_ADDRSTRLEN);

        std::cout << "Mensagem recebida pelo servidor de " << client_ip << ": id = " << packetReceived.type << std::endl;

        if (packetReceived.type == DESC) {
            // A mensagem que o cliente enviou é um broadcast
            char local_ip[BUFFER_SIZE];
            // getLocalIp(local_ip, sizeof(local_ip));
            cout << "mandando mensagem de Discover de requisicao" << endl; 
            packet ackPacket;
            ackPacket.type = DESC_ACK;

            // snprintf(buffer, BUFFER_SIZE, "%s", "Broadcast\0");
            sendto(this->socketHandler.sockfd, &ackPacket, sizeof(packet), 0, (struct sockaddr *) &this->socketHandler.cliaddr, len);

            std::cout << "Broadcast! Resposta enviada ao cliente: " << buffer << std::endl;
        } else {
            //A mensagem que o cliente enviou é um dado
            std::cout << "Dado recebido do cliente: " << packetReceived.req.value << std::endl;
            // snprintf(buffer, BUFFER_SIZE, "%s", "Broadcast\0");
            packet ackPacket = {0}; // Inicializa todos os campos com zero
            ackPacket.type = REQ_ACK;

            sendto(this->socketHandler.sockfd, &ackPacket, sizeof(packet), 0, (struct sockaddr *) &this->socketHandler.cliaddr, len);
            std::cout << "enviado" << std::endl;
        }

    }
}

std::string Server::receiveMessage(packet * packetReceived_pt) {
    char buf[SIZE_BUFFER]; 
    string clientIp; 

    buf[0] = '\0';

    while(1){
        this->socketHandler.receive(buf, SIZE_BUFFER, clientIp);
        // memcpy(packetReceived_pt, buf, sizeof(packet));

        if(buf[0] != '\0'){
            return clientIp; //retorna de quem recebeu a mensagem
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


        this->sendMessageAck(auxClient);
        return; 
    }

    auxClient.lastReq = seqn;
    auxClient.lastSum = value;
    auxClient.totalSum = this->sumTable.updateTable(value);
    
    this->sendMessageAck(auxClient);
    
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
    this->sendDiscoverAck(clientIp);
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

    this -> socketHandler.send(&ackPacket, sizeof(packet), client.IP, 4000);

}
    //nao precisa criar mais threads, a de soma do servidor ja eh uma thread


void Server::sendDiscoverAck(const std::string& clientIp) {
    cout << "mandando mensagem de Discover de requisicao" << endl; 
    packet ackPacket;
    ackPacket.type = DESC_ACK;

    this -> socketHandler.send(&ackPacket, sizeof(packet), clientIp, 4000);

}


