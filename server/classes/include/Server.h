#ifndef __SERVER_H__
#define __SERVER_H__

#include "Socket.h"
#include "packets.h"
#include "SumTable.h"
#include "ClientsTable.h"
#include <vector>
#include <string>

using namespace std; 



class Server 
{
    private:
        SocketServer socketHandler; 
        SumTable sumTable; 
        ClientsTable clientsTable; 
        bool isLeader;  
        pid_t coordinatorPID; 
        string coordinatorIP;   
        pid_t PID;
        std::vector<string> backupsIPs;//array de backupsIP somente para o lider
        
    public:
        /*constructor*/
        Server(int port); 
        sockaddr_in receiveMessage(packet * packetReceived_pt);
        void sumRequisitionResponse(int value, int seqn, sockaddr_in * sockClient);
        void discoverRequisitionResponse(sockaddr_in * sockClient);
        void sendMessageAck(clientData client, sockaddr_in * sockClient);
        void sendDiscoverAck(sockaddr_in * sockClient);
        void findCoordinatorMessage();
        void sendCoordinatorMessage(sockaddr_in * sockClient);
        bool getIsLeader();
        bool leaderTimeout();      
        void setIsLeader(bool isLeader); 
        void setCoordinatorPID(int coordinatorPID);
        void setCoordinatorIP(string coordinatorIP);    
        void sendBackup();


};


#endif