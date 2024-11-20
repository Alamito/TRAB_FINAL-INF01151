#include <iostream>
#include <string>

#include <chrono>
#include <thread>

#include "Client.h"

int main(){

    Client cliente;
    int numToSum;

    while(1){
        if (cliente.getServerAdress().compare("255.255.255.255") == 0){
            cliente.discoverServer();
            cout << "Tá aqui" << endl;
        }
        else{
            numToSum = cliente.listenTerminal();
            cliente.sendSumRequisition(numToSum);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
}

    return 0;
}