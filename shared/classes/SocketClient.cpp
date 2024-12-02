#include "Socket.h"
#include <chrono>

/*Construtor recebe apenas a porta onde estara rodando o servidor, sendo que o IP
inicial e "255.255.255.255", que e o IP de broadcast*/
SocketClient::SocketClient(int portToSend, std::string destinationIp)
    : portToSend(portToSend), destinationIp(destinationIp), socketFd(-1){//, serverHost(gethostbyname("255.255.255.255")) {
    printf("Criando Socket para enviar para porta: %d, ip: %s\n",portToSend, destinationIp.c_str());
    }
/*Metodo que inicializa o socket e define todas as informacoes do destinatario na
struct serv_addr*/
void SocketClient::create() {
	if ((this->socketFd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
		printf("ERROR opening socket");
	
    this->serv_addr.sin_family = AF_INET;     
	this->serv_addr.sin_port = htons(this->portToSend);    
   
    //this->serverHost = gethostbyname(this->ipToSend); 
    if (inet_pton(AF_INET, this->destinationIp.c_str(), &serv_addr.sin_addr) <= 0)
        printf("Endereço de IP invalido");

    int broadcastEnable = 1; 
    setsockopt(socketFd, SOL_SOCKET, SO_BROADCAST, &broadcastEnable, sizeof(broadcastEnable));
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
int SocketClient::receive(void* buf, size_t size, sockaddr_in* destinationAddr){

    int n = 0;
    //char buf[sizeof(packet)];   //buf já é passado com parâmetro
    unsigned int lengthDestinationAddr;
	lengthDestinationAddr = sizeof(struct sockaddr_in);

    /* receive from socket */
	n = recvfrom(this->socketFd, buf, size, 0, (struct sockaddr *) destinationAddr, &lengthDestinationAddr);
	// if (n < 0) 
	// 	printf("Erro no recvfrom");

    char ip_temp[INET_ADDRSTRLEN];    
    inet_ntop(AF_INET, &(destinationAddr->sin_addr), ip_temp, INET_ADDRSTRLEN);
    //senderIp = ip_temp;
	printf("Recebido um pacote de %s\n", ip_temp);

    return n;

}

void SocketClient::setReceiveTimeout(const void* testPacket, size_t packetSize) {
    char recvBuffer[SIZE_BUFFER];

    // Configurar um timeout inicial fixo (2 segundos) para evitar bloqueio
    struct timeval initialTimeout;
    initialTimeout.tv_sec = 2; // Timeout de 2 segundos
    initialTimeout.tv_usec = 0;
    if (setsockopt(this->socketFd, SOL_SOCKET, SO_RCVTIMEO, &initialTimeout, sizeof(initialTimeout)) < 0) {
        std::cerr << "Erro ao configurar o timeout inicial!" << std::endl;
        return;
    }

    // Enviar pacote de teste para calcular o RTT
    auto start = std::chrono::high_resolution_clock::now();
    this->send(const_cast<void*>(testPacket), packetSize);

    // Receber resposta diretamente com recv
    int receivedBytes = recv(this->socketFd, recvBuffer, sizeof(recvBuffer), 0);
    if (receivedBytes <= 0) {
        std::cerr << "Erro: Nenhuma resposta recebida para calcular o RTT! Usando timeout padrão.\n";

        // Configurar um timeout padrão (10 ms)
        struct timeval defaultTimeout;
        defaultTimeout.tv_sec = 0;
        defaultTimeout.tv_usec = 10000; // 10 milliseconds
        setsockopt(this->socketFd, SOL_SOCKET, SO_RCVTIMEO, &defaultTimeout, sizeof(defaultTimeout));
        return;
    }

    auto end = std::chrono::high_resolution_clock::now();
    // Calcular RTT
    double rtt = std::chrono::duration<double, std::milli>(end - start).count();
    double timeoutMs = rtt * 3;

    // Configurar o timeout calculado
    struct timeval timeout;
    timeout.tv_sec = static_cast<int>(timeoutMs / 1000);
    timeout.tv_usec = static_cast<int>((static_cast<long>(timeoutMs) % 1000) * 1000);

    if (setsockopt(this->socketFd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) < 0) {
        std::cerr << "Erro ao configurar o timeout do socket!" << std::endl;
        return;
    }
}

void SocketClient::setServ_addr(){
    //IMPLEMENTAR
    return;
}