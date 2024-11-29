#ifndef SOCKET_H
#define SOCKET_H

#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "packets.h"

class Socket {
    private:
        std::string ip;
        int port;
        int socketFd;
        int broadcastEnable;
        struct sockaddr_in serverAddr;

    public:
        Socket(const std::string& ip, int port, int broadcastEnable);
        void create();
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