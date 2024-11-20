#ifndef __SERVER_H__
#define __SERVER_H__

#include "Socket.h"
#include "packets.h"
#include "SumTable.h"
#include "ClientsTable.h"

using namespace std; 


class Server 
{
    private:
        Socket socketHandler; 
        SumTable sumTable; 
        ClientsTable clientsTable; 
        
    public:
        /*constructor*/
        Server(); 
        int receiveMessage(); 
};





#endif