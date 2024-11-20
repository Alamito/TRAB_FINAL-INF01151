
#ifndef CLIENT_H
#define CLIENT_H

#include <string.h>

#include "Message.h"

using namespace std;

class Client{

    private:
        string ipAdress; //Próprio ip do cliente
        int lastReq;
        int lastSum;
        string serverAdress;

    public:
        Client();
        void sendSumRequisition(Message* mensagem, int numRequisition);
        int listenTerminal();
        void discoverServer(Message* mensagem);  

        string getServerAdress();

};

#endif