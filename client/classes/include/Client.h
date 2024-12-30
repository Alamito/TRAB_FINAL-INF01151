
#ifndef CLIENT_H
#define CLIENT_H

#include <string>

#include "Socket.h"

using namespace std;

class Client{

    private:
        string ipAdress; //Próprio ip do cliente
        int lastReq;
        uint64_t lastSum;
        string serverAdress;

        SocketClient sockHandler;

    public:
        Client(int port);
        void sendSumRequisition(int numToSum);
        int isServerAlive();
        int sendAliveMessage();
        void discoverServer();
        //void sendSumRequisition(Message* mensagem, int numRequisition);
        // int listenTerminal();
        // void discoverServer(Message* mensagem);  

        string getServerAdress();
        void setServerAdress(string serverAdress);

};

#endif
