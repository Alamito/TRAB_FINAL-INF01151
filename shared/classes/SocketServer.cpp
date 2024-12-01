#include "Socket.h"

SocketServer::SocketServer(int myPort)
    : myPort(myPort), socketFd(-1) {
    printf("Inicializando SocketServer para ouvir na porta: %d\n", myPort);
    }


void SocketServer::create() {
	//printf("Entrou no create\n");

    /*como eh a criacao do socker para o servidor, no exemplo*/		
    if ((this->socketFd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) 
		printf("ERROR opening Server socket\n");

	this->serv_addr.sin_family = AF_INET;
	this->serv_addr.sin_port = htons(this->myPort);
	this->serv_addr.sin_addr.s_addr = INADDR_ANY;
	bzero(&(this->serv_addr.sin_zero), 8);    
	 
	if (bind(this->socketFd , (struct sockaddr *) &(this->serv_addr), sizeof(struct sockaddr)) < 0) 
		printf("ERROR on binding\n");
	
	//this->clilen = sizeof(struct sockaddr_in);
	//printf("Saiu do create\n");
}


void SocketServer::send(void* packetToSend, size_t size, sockaddr_in* destinationAddr){
	printf("Entrou no send\n");
	/* send to socket */
	//n = sendto(sockfd, "Got your message\n", 17, 0,(struct sockaddr *) &cli_addr, sizeof(struct sockaddr));
	//if (n  < 0) 
	//	printf("ERROR on sendto");

	int n;

	//Cria uma struct com os dados do destinatário
	/*struct sockaddr_in destinationAddr;
	memset(&destinationAddr, 0, sizeof(destinationAddr));
	destAddr.sin_family = AF_INET;
	destAddr.sin_port = htons(destinationPort);
	if (inet_pton(AF_INET, destinationIp.c_str(), &destinationAddr.sin_addr) <= 0){
		printf("Endereço IP de destino inválido\n");
		return;
	}*/
	n = sendto(this->socketFd, packetToSend, size, 0, (struct sockaddr *) destinationAddr, sizeof(struct sockaddr)); 
	if (n < 0)
		printf("Nenhum dado enviado\n");

	printf("bytes enviados para o cliente: %d\n", n);		
}


int SocketServer::receive(void* buf, size_t size, sockaddr_in* srcAddr){
	//printf("Entrou no receive");
    int n = 0;
    //char buf[sizeof(packet)];
    //struct sockaddr_in srcAddr;

	unsigned int lengthSockaddr;
	lengthSockaddr = sizeof(struct sockaddr_in);
    /* receive from socket */
	n = recvfrom(this->socketFd, buf, size, 0, (struct sockaddr *) srcAddr, &lengthSockaddr);
	if (n < 0) 
		printf("ERROR on recvfrom");

    char ip_temp[INET_ADDRSTRLEN];    
    inet_ntop(AF_INET, &(srcAddr->sin_addr), ip_temp, INET_ADDRSTRLEN);
    //senderIp = ip_temp;
	//printf("Received a datagram from: %s\n", ip_temp);

	return n;	
}
