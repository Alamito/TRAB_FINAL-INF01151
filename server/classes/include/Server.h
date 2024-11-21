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
        std::string receiveMessage(packet * packetReceived_pt);
        void sumRequisitionResponse(int value, int numReq, string clientIp);
        void discoverRequisitionResponse(const std::string& clientIp);
        void sendMessageAck(clientData client);
        void sendDiscoverAck(const std::string& clientIp);

};


#endif