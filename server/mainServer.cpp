#include <thread>

#include "ClientsTable.h"
#include "SumTable.h"

using namespace std;

int main(){

    /*uso do new so quando quer a tabela usada em outros escopos*/
    /*vai pra pilha da main*/
    
    /* =============================== */
    /* testing clients Table*/
    ClientsTable clientsTable;

    clientData client;
    client.lastReq = 23; 
    client.lastSum = 1000;
    client.IP = "127.0.0.1";

    clientData client2;
    client2.lastReq = 23; 
    client2.lastSum = 1000;
    client2.IP = "127.0.0.2";

    thread newClientThread1(&ClientsTable::addClient, ref(clientsTable), ref(client));
    thread newClientThread2(&ClientsTable::addClient, ref(clientsTable), client2);
    thread newClientThread3(&ClientsTable::addClient, ref(clientsTable), client);
    thread newClientThread4(&ClientsTable::addClient, ref(clientsTable), client);

    //clientsTable.printTable();
    

    newClientThread1.join();
    newClientThread2.join();
    newClientThread3.join();
    newClientThread4.join();
    

    clientsTable.printTable();

    //thread clientRemoveThread(&ClientsTable::deleteClient, ref(clientsTable), "127.0.0.1");

    //clientRemoveThread.join();

    //clientsTable.deleteClient();
    //clientsTable.updateClient();

    thread updateThread1(&ClientsTable::updateClient, ref(clientsTable), client2.IP, 25, 6000);
    thread updateThread2(&ClientsTable::updateClient, ref(clientsTable), client.IP, 22, 10000);

    updateThread1.join(); 
    updateThread2.join();
    clientsTable.printTable();
    





    /* =============================== */
    

    /* =============================== */
    /*testing sum table*/
    /*
    SumTable sumTable; 

    thread newThread(&SumTable::updateTable,  ref(sumTable), 10);
    thread newThread2(&SumTable::updateTable, ref(sumTable), 10);
    thread newThread3(&SumTable::updateTable, ref(sumTable), 10);
    thread newThread4(&SumTable::updateTable, ref(sumTable), 10);
    
   
    thread t1(&SumTable::printTable, ref(sumTable));
    thread t2(&SumTable::printTable, ref(sumTable));
    thread t3(&SumTable::printTable, ref(sumTable));
    thread t4(&SumTable::printTable, ref(sumTable));
   
    newThread.join();
    newThread2.join();
    newThread3.join();
    newThread4.join();
    
    t1.join();
    t2.join();
    t3.join();
    t4.join();
    */
    /* =============================== */

    return 0; 
}