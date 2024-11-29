#include "Socket.h"


/*Construtor recebe apenas a porta onde estara rodando o servidor, sendo que o IP
inicial e "255.255.255.255", que e o IP de broadcast*/
SocketClient::SocketClient(int portToSend)
    : portToSend(portToSend), socketFd(-1){//, serverHost(gethostbyname("255.255.255.255")) {
    printf("Criando Socket para enviar para %d",portToSend);
    }


/*Metodo que inicializa o socket e define todas as informacoes do destinatario na
struct serv_addr*/
void SocketClient::create() {
	if ((this->socketFd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
		printf("ERROR opening socket");
	
    this->serv_addr.sin_family = AF_INET;     
	this->serv_addr.sin_port = htons(this->portToSend);    
   
    //this->serverHost = gethostbyname(this->ipToSend); 
    if (inet_pton(AF_INET, "143.54.55.46", &serv_addr.sin_addr) <= 0)
        printf("Endereço de IP invalido");
	//this->serv_addr.sin_addr = *((struct in_addr *)this->serverHost->h_addr);
	
    bzero(&(this->serv_addr.sin_zero), 8);  
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
void SocketClient::receive(void* buf, size_t size, sockaddr_in* destinationAddr){

    int n = 0;
    //char buf[sizeof(packet)];   //buf já é passado com parâmetro
    unsigned int lengthDestinationAddr;
	lengthDestinationAddr = sizeof(struct sockaddr_in);

    /* receive from socket */
	n = recvfrom(this->socketFd, buf, size, 0, (struct sockaddr *) destinationAddr, &lengthDestinationAddr);
	if (n < 0) 
		printf("Erro no recvfrom");

    char ip_temp[INET_ADDRSTRLEN];    
    inet_ntop(AF_INET, &(destinationAddr->sin_addr), ip_temp, INET_ADDRSTRLEN);
    //senderIp = ip_temp;
	printf("Recebido um pacote de %s\n", ip_temp);

}

void SocketClient::setServ_addr(){
    //IMPLEMENTAR
    return;
}