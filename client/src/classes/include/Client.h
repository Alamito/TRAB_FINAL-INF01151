
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
        void sendSumRequisition(Message* mensagem);
        int listenTerminal();
        void discoverServer(Message* mensagem);  

};

#endif