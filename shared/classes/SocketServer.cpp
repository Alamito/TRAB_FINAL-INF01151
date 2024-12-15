#include "Socket.h"

SocketServer::SocketServer(int myPort)
    : myPort(myPort), socketFd(-1) {
    }


void SocketServer::create() {
    if ((this->socketFd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) 
		printf("ERROR opening Server socket\n");

	this->serv_addr.sin_family = AF_INET;
	this->serv_addr.sin_port = htons(this->myPort);
	this->serv_addr.sin_addr.s_addr = INADDR_ANY;
	bzero(&(this->serv_addr.sin_zero), 8);    
	 
	if (bind(this->socketFd , (struct sockaddr *) &(this->serv_addr), sizeof(struct sockaddr)) < 0) 
		printf("ERROR on binding\n");
	
}


void SocketServer::send(void* packetToSend, size_t size, sockaddr_in* destinationAddr){
	int n;
	n = sendto(this->socketFd, packetToSend, size, 0, (struct sockaddr *) destinationAddr, sizeof(struct sockaddr)); 
	if (n < 0)
		printf("Nenhum dado enviado\n");
}

void SocketServer::sendBroadcast(void* packetToSend, size_t size) {
    sockaddr_in broadcastAddr;
    broadcastAddr.sin_family = AF_INET;
    broadcastAddr.sin_port = htons(this->myPort);  // Porta de destino
    broadcastAddr.sin_addr.s_addr = INADDR_BROADCAST;  // Endereço de broadcast

    // Garante que o socket está habilitado para broadcast
    int broadcastEnable = 1;
    if (setsockopt(socketFd, SOL_SOCKET, SO_BROADCAST, &broadcastEnable, sizeof(broadcastEnable)) < 0) {
        perror("Erro ao habilitar SO_BROADCAST");
        return;
    }

    // Envia o pacote via broadcast
    int n = sendto(this->socketFd, packetToSend, size, 0, 
                   (struct sockaddr*)&broadcastAddr, sizeof(broadcastAddr));
    if (n < 0) {
        perror("Erro ao enviar pacote via broadcast");
    } else {
        printf("Broadcast enviado com sucesso (%d bytes)\n", n);
    }
}


int SocketServer::receive(void* buf, size_t size, sockaddr_in* srcAddr){
    int n = 0;

	unsigned int lengthSockaddr;
	lengthSockaddr = sizeof(struct sockaddr_in);
	n = recvfrom(this->socketFd, buf, size, 0, (struct sockaddr *) srcAddr, &lengthSockaddr);
	if (n < 0) 
		printf("ERROR on recvfrom");

    char ip_temp[INET_ADDRSTRLEN];    
    inet_ntop(AF_INET, &(srcAddr->sin_addr), ip_temp, INET_ADDRSTRLEN);

	return n;	
}

int SocketServer::getSocketFd() {
	return this->socketFd;
}

int SocketServer::getPort() {
	return this->myPort;
}

struct sockaddr_in SocketServer::getServAddr() {
	return this->serv_addr;
}