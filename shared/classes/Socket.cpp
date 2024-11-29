#include "Socket.h"
#include <stdexcept>
#include <unistd.h>
#include <cstring>
#include <string>
#include <iostream>

Socket::Socket(const std::string& ip, int port, int broadcastEnable)
    : ip(ip), port(port), socketFd(-1), broadcastEnable(broadcastEnable) {
    printf("Inicializando Socket com IP: %s, Porta: %d\n", ip.c_str(), port);
    }

void Socket::create() {
    if ((socketFd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("Falha ao criar socket");
        throw std::runtime_error("Failed to create socket");
    }

	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(port);
	serverAddr.sin_addr.s_addr = INADDR_ANY;
	bzero(&(serverAddr.sin_zero), 8);     
}

void Socket::setBroadcastEnable(int broadcastEnable){
    this -> broadcastEnable = broadcastEnable;
    if (broadcastEnable == 1){
        serverAddr.sin_addr.s_addr = inet_addr("255.255.255.255");
        }
    else{
        serverAddr.sin_addr.s_addr = INADDR_ANY;
    }
    if (setsockopt(socketFd, SOL_SOCKET, SO_BROADCAST, &this->broadcastEnable, sizeof(this->broadcastEnable)) < 0){     //broadcast
        perror("Falha ao criar socket Broadcast");
        throw std::runtime_error("Failed to create socket");
    }

    printf("Broadcast ativado: %d\n", broadcastEnable);
}

void Socket::bind() {
    if (::bind(socketFd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0)
        throw std::runtime_error("Failed to bind socket to port");
}

int Socket::send(const void* data, size_t size, const std::string& destIp, int destPort) const {
    if (socketFd < 0) {
        throw std::runtime_error("Socket não inicializado corretamente");
    }

    struct sockaddr_in destAddr;
    memset(&destAddr, 0, sizeof(destAddr));
    destAddr.sin_family = AF_INET;
    destAddr.sin_port = htons(destPort);

    if (destIp.empty()) {
        throw std::runtime_error("Endereço IP de destino está vazio");
    }

    if (inet_pton(AF_INET, destIp.c_str(), &destAddr.sin_addr) <= 0) {
        throw std::runtime_error("Endereço IP de destino inválido");
    }

    // Imprime o endereço armazenado em destAddr
    char ip[INET_ADDRSTRLEN];
    if (inet_ntop(AF_INET, &destAddr.sin_addr, ip, INET_ADDRSTRLEN) == nullptr) {
        throw std::runtime_error("Falha ao converter o IP de destAddr para string");
    }

    printf("Tentando enviar dados:\n");
    printf("  - socketFd: %d\n", socketFd);
    printf("  - destIp: %s\n", destIp.c_str());
    printf("  - destPort: %d\n", destPort);
    printf("  - broadcastEnable: %d\n", broadcastEnable);
    printf("  - data size: %zu\n", size);
    printf("Mensagem enviada para IP: %s, Porta: %d\n", ip, ntohs(destAddr.sin_port));

    int bytesSent = sendto(socketFd, data, size, 0,(struct sockaddr *) &destAddr, sizeof(destAddr));
    printf("bytesSent: %d\n", bytesSent);

    if (bytesSent == -1) {
        perror("Erro ao enviar mensagem com sendto");
        printf("  - errno: %d\n", errno);
    }
    return bytesSent;
}


int Socket::receive(void* buffer, size_t size, std::string& senderIp) const {
 
    struct sockaddr_in srcAddr;
    socklen_t addrLen = sizeof(srcAddr);

    struct timeval timeout;
    timeout.tv_sec = 5;
    timeout.tv_usec = 0;
    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(socketFd, &readfds);
    srcAddr.sin_port = htons(this->port);

    printf("entrando no select dentro do receive\n");

    int selectResult = select(socketFd + 1, &readfds, nullptr, nullptr, &timeout);
    printf("passou do select dentro do receive\n");
    if (selectResult == -1) {
        perror("Erro ao usar select");
        return -1;
    } else if (selectResult == 0) {
        printf("Timeout ao esperar pela mensagem no socket %d.\n", socketFd);
        return 0;
    }
    
    int bytesReceived = recvfrom(socketFd, buffer, size, 0, (struct sockaddr*)&srcAddr, &addrLen);  //listen
    printf("mensagem recebida da porta: %d\n", ntohs(srcAddr.sin_port));
    printf("passou do recvfrom dentro do receive\n");
    //Obtem o ip de quem enviou a mensagem
    char ip_temp[INET_ADDRSTRLEN];
    
    inet_ntop(AF_INET, &(srcAddr.sin_addr), ip_temp, INET_ADDRSTRLEN);
    senderIp = ip_temp;

    if (bytesReceived < 0) {
        perror("Erro ao receber a mensagem");
    } else if (bytesReceived == 0) {
        printf("Conexão fechada ao receber a mensagem.\n");
    } else {
        printf("Mensagem recebida com sucesso (%d bytes).\n", bytesReceived);
        //printa mensagem
        printf("Mensagem recebida: %s\n", (char*)buffer);
    }

    return bytesReceived;

}

void Socket::close() {
    if (socketFd >= 0) {
        ::close(socketFd);
        socketFd = -1;
    }
}

Socket::~Socket() {
    close();
}