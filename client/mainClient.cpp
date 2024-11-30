#include <iostream>
#include <string>

#include <chrono>
#include <thread>

#include "Client.h"

using namespace std;

int main(){

    Client cliente;
    int numToSum;

    while(1){
        if (cliente.getServerAdress().compare("255.255.255.255") == 0){
            cliente.discoverServer();
        }
        else{
            numToSum = cliente.listenTerminal();
            cliente.sendSumRequisition(numToSum);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
}

    // SocketClient client(8080, "127.0.0.1");
    // client.create();

    // packet data, received;
    // data.type = REQ;
    // client.send(&data, sizeof(data));


    // char buf[SIZE_BUFFER];
    // struct sockaddr_in serverAddr;
    // client.receive(buf, SIZE_BUFFER, &serverAddr);
    // memcpy(&received, buf, sizeof(packet));

    // cout << "Tipo do pacote recebido: " << received.type << endl;


    // return 0;
}