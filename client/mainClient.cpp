#include <iostream>
#include <string>
#include <stdio.h>

#include <chrono>
#include <thread>

#include "Client.h"

using namespace std;

int main(int argc, char* argv[]){

    Client cliente(atoi(argv[1]));
    int numToSum;
    char numToSumChar[20];

    while(1){
        //printf("cliente.getServerAdress(): %s\n", cliente.getServerAdress().c_str());
        if (cliente.getServerAdress().compare("255.255.255.255") == 0){
            cliente.discoverServer();
        }
        else{
            //numToSum = cliente.listenTerminal();
            while (scanf("%d", &numToSum) == 1){
                cliente.sendSumRequisition(numToSum);
            }
        }
        //std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    }
}
