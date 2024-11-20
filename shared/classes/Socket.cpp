#include "Socket.h"
#include <stdexcept>
#include <unistd.h>
#include <cstring>
#include <string>

Socket::Socket(const std::string& ip, int port)
    : ip(ip), port(port), socketFd(-1) {}


void Socket::create() {
    socketFd = socket(AF_INET, SOCK_DGRAM, 0);  // Cria um socket UDP

    if ((socketFd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("Falha ao criar socket");
        throw std::runtime_error("Failed to create socket");
    }

	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(port);
	serverAddr.sin_addr.s_addr = INADDR_ANY;
	bzero(&(serverAddr.sin_zero), 8);    
    
    int broadcastEnable = 1;                                                                                //Criação de socket UDP para 
    if (setsockopt(socketFd, SOL_SOCKET, SO_BROADCAST, &broadcastEnable, sizeof(broadcastEnable)) < 0){     //broadcast
        perror("Falha ao criar socket Broadcast");
        throw std::runtime_error("Failed to create socket");
    }
    
    
    //if (inet_pton(AF_INET, ip.c_str(), &serverAddr.sin_addr) <= 0) {
    //    throw std::runtime_error("Endereço IP inválido para o socket");
    //}
}

void Socket::bind() {
    if (::bind(socketFd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0)
        throw std::runtime_error("Failed to bind socket to port");
}

ssize_t Socket::send(const void* data, size_t size, const std::string& destIp, int destPort) const {
    if (socketFd < 0) {
        throw std::runtime_error("Socket não inicializado corretamente");
    }

    struct sockaddr_in destAddr;
    memset(&destAddr, 0, sizeof(destAddr));  // Inicializa a estrutura com zeros
    destAddr.sin_family = AF_INET;
    destAddr.sin_port = htons(destPort);

    if (inet_pton(AF_INET, destIp.c_str(), &destAddr.sin_addr) <= 0) {
        throw std::runtime_error("Endereço IP de destino inválido");
    }

    printf("Tentando enviar dados:\n");
    printf("  - socketFd: %d\n", socketFd);
    printf("  - destIp: %s\n", destIp.c_str());
    printf("  - destPort: %d\n", destPort);
    printf("  - data size: %zu\n", size);

    ssize_t bytesSent = sendto(socketFd, data, size, 0, (struct sockaddr*)&destAddr, sizeof(destAddr));
    if (bytesSent == -1) {
        perror("Erro ao enviar mensagem com sendto");
        printf("  - errno: %d\n", errno);
    }
    return bytesSent;
}

ssize_t Socket::receive(void* buffer, size_t size, std::string& senderIp) const {
    struct sockaddr_in srcAddr;
    socklen_t addrLen = sizeof(srcAddr);

    struct timeval timeout;
    timeout.tv_sec = 1;
    timeout.tv_usec = 0;
    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(socketFd, &readfds);

    int selectResult = select(socketFd + 1, &readfds, nullptr, nullptr, &timeout);
    if (selectResult == -1) {
        perror("Erro ao usar select");
        return -1;
    } else if (selectResult == 0) {
        //printf("Timeout ao esperar pela mensagem.\n");
        return 0;
    }

    ssize_t bytesReceived = recvfrom(socketFd, buffer, size, 0, (struct sockaddr*)&srcAddr, &addrLen);  //listen

    //Obtem o ip de quem enviou a mensagem
    char ip_temp[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(srcAddr.sin_addr), ip_temp, INET_ADDRSTRLEN);
    senderIp = ip_temp;


    if (bytesReceived < 0) {
        perror("Erro ao receber a mensagem");
    } else if (bytesReceived == 0) {
        printf("Conexão fechada ao receber a mensagem.\n");
    } else {
        //printf("Mensagem recebida com sucesso (%ld bytes).\n", bytesReceived);
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