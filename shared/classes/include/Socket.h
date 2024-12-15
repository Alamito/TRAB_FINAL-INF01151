#ifndef SOCKET_H
#define SOCKET_H

#include <sys/types.h>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdexcept>
#include <unistd.h>
#include <cstring>
#include <iostream>

#include "packets.h"

class SocketClient {
    private: 
        int socketFd;       //Armazena os id do socket criado
        int portToSend;     //Porta que estara rodando o servidor
    	struct hostent *serverHost;     //Atraves da funçao getHostByName(std::string IPDestino), armazena o IP de envio de mensagem
        struct sockaddr_in serv_addr;   //Struct que contem todas as informaçoes do destinatario ()
        std::string destinationIp;     //IP de destino

    public: 
        SocketClient(int portToSend, std::string destinationIp); 
        void create(); 
        void send(void* packetToSend, size_t size);
        int receive(void* buf, size_t size, sockaddr_in* destinationAddr);
}; 


class SocketServer {
    private: 
        int socketFd;
        struct sockaddr_in serv_addr;
        int myPort;

    public:
        SocketServer(int myPort);
        void create(); 
        void send(void* packetToSend, size_t size, sockaddr_in* destinationAddr);
        void sendBroadcast(void* packetToSend, size_t size);
        int receive(void* buf, size_t size, sockaddr_in* srcAddr);
        int getSocketFd();
        int getPort();
        struct sockaddr_in getServAddr();
};

#endif // SOCKET_H