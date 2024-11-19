#include <iostream>
#include <string>

#include <chrono>
#include <thread>

#include "Client.h"
#include "Message.h"

int main(){

    Client cliente;
    Message mensagem(0, "255.255.255.255", 8080, "1.1.1.1", 4000);
    int numToSum;

    while(1){
        if (cliente.getServerAdress().compare("255.255.255.255") == 0){
            cliente.discoverServer(&mensagem);
            cout << "Tá aqui" << endl;
        }
        else{
            numToSum = cliente.listenTerminal();
            cliente.sendSumRequisition(&mensagem, numToSum);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
}

    return 0;
}