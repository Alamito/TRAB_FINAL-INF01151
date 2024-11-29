
#ifndef CLIENT_H
#define CLIENT_H

#include <string.h>

#include "Socket.h"

using namespace std;

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
        // void sendSumRequisition(Message* mensagem, int numRequisition);
        // int listenTerminal();
        // void discoverServer(Message* mensagem);  

        string getServerAdress();

};

#endif