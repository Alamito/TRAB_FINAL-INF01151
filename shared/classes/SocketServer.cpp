#include "Socket.h"





SocketServer::SocketServer(int myPort)
    : myPort(myPort), socketFd(-1) {
    printf("Inicializando Socket para ouvir na porta: %d\n", myPort);
    }


void SocketServer::create() {

    /*como eh a criacao do socker para o servidor, no exemplo*/		
    if ((this->socketFd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) 
		printf("ERROR opening Server socket");

	this->serv_addr.sin_family = AF_INET;
	this->serv_addr.sin_port = htons(this->myPort);
	this->serv_addr.sin_addr.s_addr = INADDR_ANY;
	bzero(&(this->serv_addr.sin_zero), 8);    
	 
	if (bind(this->socketFd , (struct sockaddr *) &(this->serv_addr), sizeof(struct sockaddr)) < 0) 
		printf("ERROR on binding");
	
	this->clilen = sizeof(struct sockaddr_in);

}


void SocketServer::send(sockaddr_in* destinationAddr, void* packetToSend){
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
	n = sendto(this->sockfd, packetToSend, sizeof(packetToSend), 0, (struct sockaddr *) destinationAddr, sizeof(struct sockaddr)); 
	if (n < 0)
		printf("Nenhum dado enviado");


}


void socketServer::receive(void* buf, size_t size, sockaddr_in* srcAddr){
    int n = 0;
    char buf[sizeof(packet)];
    //struct sockaddr_in srcAddr;

    /* receive from socket */
	n = recvfrom(sockfd, buf, size, 0, (struct sockaddr *) srcAddr, sizeof(struct sockaddr_in));
	if (n < 0) 
		printf("ERROR on recvfrom");

    char ip_temp[INET_ADDRSTRLEN];    
    inet_ntop(AF_INET, &(srcAddr.sin_addr), ip_temp, INET_ADDRSTRLEN);
    //senderIp = ip_temp;
	printf("Received a datagram: %s, from: %s\n", buf, ip_temp);
}