#include "ClientsTable.h"

using namespace std;

int ClientsTable::addClient(clientData client) {
    
    while (true) {        
        if (this->mutex_lock_clients.try_lock()) {
            cout << endl << "  ...adding client..." << endl << endl;
            this->clients.push_back(client);
            this->mutex_lock_clients.unlock();
            return true;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));  
    }
}

int ClientsTable::deleteClient(std::string ip) {
    while(true){
        if(this->mutex_lock_clients.try_lock()){ 
            cout << endl << "  ...deleting client..." << endl << endl;
            this->clients.remove_if([&ip](const clientData& client) {
                return client.IP == ip;
            });
            this->mutex_lock_clients.unlock();
            return true; 
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));  
    }
}

int ClientsTable::updateClient(std::string ip, int req, int sum) {
    while(true){
        if(this->mutex_lock_clients.try_lock()){ 
            cout << endl << "  ...updating client..." << endl << endl;
            for (auto& client : this->clients) {
                if (client.IP == ip) {
                    // Encontrou o cliente com o IP correspondente, atualiza os valores
                    client.lastReq = req;
                    client.lastSum = sum;
                    cout << endl << "  ...Client with IP " << ip << " updated: lastReq = " << req << ", lastSum = " << sum << "..." << endl;
                }
            }
            this->mutex_lock_clients.unlock();
            return true; 
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));  

    }
}


void ClientsTable::printTable() {
    while(true){
        if(this->mutex_lock_clients.try_lock()){ 
            cout << "  --------------------" << endl;
            cout << " |   Clients Table    |" << endl;
            cout << "  --------------------" << endl;

            for (auto iterator = this->clients.begin(); iterator != this->clients.end(); ++iterator) {
                cout << " | Client IP          | " << iterator->IP << endl;
                cout << " | Last Sum           | " << iterator->lastSum << endl;
                cout << " | Last request       | " << iterator->lastReq << endl;
                cout << "  --------------------" << endl;

            }
            this->mutex_lock_clients.unlock();
            return; 
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));  

    }
}