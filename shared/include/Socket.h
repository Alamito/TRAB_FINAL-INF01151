// Socket.h

#ifndef SOCKET_H
#define SOCKET_H

#include <string>
#include <sys/socket.h>    // Inclui definições de socket
#include <netinet/in.h>    // Inclui sockaddr_in
#include <arpa/inet.h>     // Inclui funções para manipulação de IPs
class Socket {

    private:
    std::string ip;
    int port;
    int socketFd;   //um identificador inteiro atribuído pelo sistema operacional quando um socket é criado. 
    struct sockaddr_in serverAddr;  // serverAddr é uma estrutura sockaddr_in que agrupa IP e porta, além de especificar o tipo de protocolo.

    public:
        Socket(const std::string& ip = "127.0.0.1", int port = 8080);
        void create();
        void connect();
        void bind();
        void listen(int backlog = 5);
        Socket accept();
        ssize_t send(const void* data, size_t size) const;
        ssize_t receive(void* buffer, size_t size) const;
        void close();
        ~Socket();

        //getters
        std::string getIp() const { return ip; }
        int getPort() const { return port; }
        int getSocketFd() const { return socketFd; }
};

#endif // SOCKET_H
