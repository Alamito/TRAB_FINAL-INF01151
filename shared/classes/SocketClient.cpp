#include "Socket.h"



SocketClient::SocketClient(int portToSend)
    : port(portToSend), socketFd(-1), serverHost(gethostbyname("255.255.255.255")) {
    printf("Criando Socket para enviar para %d",portToSend);
    }

void SocketClient::create() {
	if ((this->socketFd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
		printf("ERROR opening socket");
	
    this->serv_addr.sin_family = AF_INET;     
	this->serv_addr.sin_port = htons(this->portToSend);    
   
    //this->serverHost = gethostbyname(this->ipToSend); 
	this->serv_addr.sin_addr = *((struct in_addr *)this->serverHost->h_addr);
	
    bzero(&(this->serv_addr.sin_zero), 8);  
}

void SocketClient::send(void* packetToSend){
    //n = sendto(sockfd, buffer, strlen(buffer), 0, (const struct sockaddr *) &serv_addr, sizeof(struct sockaddr_in));
	//if (n < 0) 
	//	printf("ERROR sendto");
    int n;

    n = sendto(this->socketFd, packetToSend, sizeof(packetToSend), 0, (struct sockaddr *) this->&serv_addr, sizeof(struct sockaddr_in));
    if (n < 0)
        printf("Erro no envio de pacote pelo cliente")
}

void SocketClient::receive(void* buf, size_t size, sockaddr_in* destinationAddr){

    int n = 0;
    char buf[sizeof(packet)];   //buf já é passado com parâmetro
    //struct sockaddr_in srcAddr;

    /* receive from socket */
	n = recvfrom(this->socketFd, buf, size, 0, (struct sockaddr *) &destinationAddr, sizeof(struct sockaddr_in));
	if (n < 0) 
		printf("ERROR on recvfrom");

    char ip_temp[INET_ADDRSTRLEN];    
    inet_ntop(AF_INET, &(destinationAddr.sin_addr), ip_temp, INET_ADDRSTRLEN);
    //senderIp = ip_temp;
	printf("Recebido um pacote de %s\n", ip_temp);

}

void SocketClient>>setServ_addr(){
    //IMPLEMENTAR
    return;
}