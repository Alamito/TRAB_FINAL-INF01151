#include "ClientsTable.h"

using namespace std;

clientData ClientsTable::getClient(std::string ip){
    
    clientData clientData = {0,0,0,""}; 
    while (true) {        
        if (this->mutex_lock_clients.try_lock()) {
            for (auto& clients : this->clients) {
                if (clients.IP == ip){
                    this->mutex_lock_clients.unlock();
                    return clients;    
                }
            }  
            this->mutex_lock_clients.unlock();
            return {};
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));  
    }
}


int ClientsTable::addClient(clientData client) {
    
    while (true) {        
        if (this->mutex_lock_clients.try_lock()) {
            for (auto& clients : this->clients) {
                if (client.IP == clients.IP){
                    this->mutex_lock_clients.unlock();
                    return false;    
                }
            }
            this->clients.push_back(client);
            this->mutex_lock_clients.unlock();
            return true;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));  
    }
}

int ClientsTable::deleteClient(std::string ip) {
    while(true){
        if(this->mutex_lock_clients.try_lock()){ 
            this->clients.remove_if([&ip](const clientData& client) {
                return client.IP == ip;
            });
            this->mutex_lock_clients.unlock();
            return true; 
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));  
    }
}

int ClientsTable::updateClient(std::string ip, int req, int value, int sum) {
    while(true){
        if(this->mutex_lock_clients.try_lock()){ 
            for (auto& client : this->clients) {
                if (client.IP == ip) {
                    client.lastReq = req;
                    client.lastSum = value;
                    client.totalSum = sum; 
                }
            }
            this->mutex_lock_clients.unlock();
            return true; 
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));  

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
                cout << " | Last SeqNumber     | " << iterator->lastReq << endl;
                cout << " | Last value         | " << iterator->lastSum << endl;
                cout << " | Last totalSum      | " << iterator->totalSum << endl;
                cout << "  --------------------" << endl;

            }
            this->mutex_lock_clients.unlock();
            return; 
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));  

    }
}