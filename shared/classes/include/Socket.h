#ifndef SOCKET_H
#define SOCKET_H

#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "packets.h"
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <netdb.h>

class Socket {
    private:
        std::string ip;
        int port;
        int socketFd;
        
        int broadcastEnable;
        struct sockaddr_in serverAddr;
        

    public:
        struct sockaddr_in servaddr, cliaddr;
        int sockfd;
        Socket(const std::string& ip = "127.0.0.1", int port = 4000, int broadcastEnable = 1);
        void create();
        void createSocketToServer();
        void setBroadcastEnable(int broadcastEnable); // Adicione esta linha
        void bind();  // Agora simplificado para apenas UDP
        int send(const void* data, size_t size, const std::string& destIp, int destPort) const;
        int receive(void* buffer, size_t size, std::string& senderIp) const;
        void close();
        ~Socket();

        void getLocalIp(char *buffer, size_t buffer_len); 
        int getPort() const { return port; }
        int getSocketFd() const { return socketFd; }
};

#endif // SOCKET_H
