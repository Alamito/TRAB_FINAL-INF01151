#ifndef SOCKET_H
#define SOCKET_H

#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

class Socket {
    private:
        std::string ip;
        int port;
        int socketFd;
        struct sockaddr_in serverAddr;

    public:
        Socket(const std::string& ip = "127.0.0.1", int port = 8080);
        void create();
        void bind();  // Agora simplificado para apenas UDP
        ssize_t send(const void* data, size_t size, const std::string& destIp, int destPort) const;
        ssize_t receive(void* buffer, size_t size, std::string& senderIp) const;
        void close();
        ~Socket();

        std::string getIp() const { return ip; }
        int getPort() const { return port; }
        int getSocketFd() const { return socketFd; }
};

#endif // SOCKET_H