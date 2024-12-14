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