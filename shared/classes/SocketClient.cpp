#include "Socket.h"
#include <chrono>
#include <sys/time.h>

/*Construtor recebe apenas a porta onde estara rodando o servidor, sendo que o IP
inicial e "255.255.255.255", que e o IP de broadcast*/
SocketClient::SocketClient(int portToSend, std::string destinationIp)
    : portToSend(portToSend), destinationIp(destinationIp), socketFd(-1){
    }

/*Metodo que inicializa o socket e define todas as informacoes do destinatario na
struct serv_addr*/
void SocketClient::create() {
	if ((this->socketFd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
		printf("ERROR opening socket");
	
    this->serv_addr.sin_family = AF_INET;     
	this->serv_addr.sin_port = htons(this->portToSend);    
   
    if (inet_pton(AF_INET, this->destinationIp.c_str(), &serv_addr.sin_addr) <= 0)
        printf("Endereço de IP invalido");

    int broadcastEnable = 1; 
    setsockopt(socketFd, SOL_SOCKET, SO_BROADCAST, &broadcastEnable, sizeof(broadcastEnable));
	
    bzero(&(this->serv_addr.sin_zero), 8);  

    struct timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = 10000;
    if (setsockopt(this->socketFd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) < 0)
        std::cout << "Deu Ruim" << std::endl; 


}

/*Metodo de envio de pacote. Recebe um pacote a ser enviado (tipo packet) e o envia 
para o endeço definido na struct serv_addr*/
void SocketClient::send(void* packetToSend, size_t size){
    int n;

    n = sendto(this->socketFd, packetToSend, size, 0, (struct sockaddr *) &this->serv_addr, sizeof(struct sockaddr_in));
    if (n < 0)
        printf("Erro no envio de pacote pelo cliente");
}

/*Recebe um pacote.
Armazena os dados recebidos em buf e o endereço do remetente em destinationAddr*/
int SocketClient::receive(void* buf, size_t size, sockaddr_in* destinationAddr){

    int n = 0;
    unsigned int lengthDestinationAddr;
	lengthDestinationAddr = sizeof(struct sockaddr_in);

	n = recvfrom(this->socketFd, buf, size, 0, (struct sockaddr *) destinationAddr, &lengthDestinationAddr);

    char ip_temp[INET_ADDRSTRLEN];    
    inet_ntop(AF_INET, &(destinationAddr->sin_addr), ip_temp, INET_ADDRSTRLEN);

    return n;

}
