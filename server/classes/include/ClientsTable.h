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
    int lastReq;
    int lastSum;
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
        int updateClient(std::string ip, int req, int sum); //talvez nao precise

        void printTable();
};
#endif