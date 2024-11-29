#ifndef SOCKET_H
#define SOCKET_H

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
        int socketFd;
        int portToSend; 
    	struct hostent *serverHost;
        struct sockaddr_in serv_addr;

    public: 
        SocketClient(int portToSend); 
        void create(); 
        void send();
        void receive();

        void setServ_addr();    //IMPLEMENTAR
}; 


class SocketServer {
    private: 
        int socketFd;
        struct sockaddr_in serv_addr;
        int myPort;

    public:
        SocketServer(int myPort);
        void create(); 
        void send(std::string& destinationIp, int destinationPort, packet packetToSend);
        void receive(oid* buf, size_t size, std::string& senderIp);
};


class Socket {
    private:


        /*ambos*/
        int socketFd;

 
        //  int port;

        /*server*/
        struct sockaddr_in cli_addr;
        int myPort;
        //int recipientIp;



        int myIp; 
        socklen_t cliLen; 
        
        
        //struct sockaddr_in serverAddr;


    public:
        Socket(int port = 4000, int broadcastEnable = 1);
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