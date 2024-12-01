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
        SocketServer socketHandler; 
        SumTable sumTable; 
        ClientsTable clientsTable; 
        
    public:
        /*constructor*/
        Server(int port); 
        sockaddr_in receiveMessage(packet * packetReceived_pt);
        void sumRequisitionResponse(int value, int seqn, sockaddr_in * sockClient);
        void discoverRequisitionResponse(sockaddr_in * sockClient);
        void sendMessageAck(clientData client, sockaddr_in * sockClient);
        void sendDiscoverAck(sockaddr_in * sockClient);

};


#endif
