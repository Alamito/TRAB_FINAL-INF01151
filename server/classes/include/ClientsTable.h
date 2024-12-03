#ifndef CLIENTS_TABLE_H
#define CLIENTS_TABLE_H

#include <iostream>
#include <string>
#include <list>
#include <mutex>
#include <chrono>
#include <thread>

/*
list clientData clients
addClient ()
updateClient()
*/


struct clientData {
    uint16_t lastReq;
    uint16_t lastSum;
    uint16_t totalSum; 
    std::string IP;
};

class ClientsTable 
{
    private:
        std::list<clientData> clients;
        std::mutex mutex_lock_clients; 

    public:
        /*constructor*/
        ClientsTable(): clients() {};//constructor

        /*getters*/
        std::list<clientData> getList() const { return clients;}
        /**/
        
        int addClient(clientData client);
        int deleteClient(std::string ip); //talvez nao precise
        int updateClient(std::string ip, int req, int value, int sum); //talvez nao precise
        clientData getClient(std::string ip); 


        void printTable();
};
#endif