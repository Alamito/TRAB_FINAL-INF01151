
#ifndef CLIENT_H
#define CLIENT_H

#include <string>

#include "Socket.h"

using namespace std;

#define BROADCAST_IP "255.255.255.255"

class Client{

    private:
        string ipAdress; //Próprio ip do cliente
        int lastReq;
        int lastSum;
        string serverAdress;

        SocketClient sockHandler;

    public:
        Client();
        void sendSumRequisition(int numToSum);
        int listenTerminal();
        void discoverServer();
        //void sendSumRequisition(Message* mensagem, int numRequisition);
        // int listenTerminal();
        // void discoverServer(Message* mensagem);  

        string getServerAdress();
        void setServerAdress(string serverAdress);

};

#endif
