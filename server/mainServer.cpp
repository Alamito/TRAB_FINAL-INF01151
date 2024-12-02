#include <thread>
#include "Server.h"
#include <cstring>

using namespace std;

int main() {
    Server server;
    packet packetReceived; 
    sockaddr_in srcAddr; /*estrutura que recebe os dados do client*/
    while(1){
        srcAddr = server.receiveMessage(&packetReceived);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));

        switch(packetReceived.type){
            case DESC: {
                //cout << "pacote de descoberta" << endl; 
                std::thread d(&Server::discoverRequisitionResponse, ref(server),&srcAddr);
                d.detach();
                //server.discoverRequisitionResponse(&srcAddr);

                break;
            }
            case REQ: {
                //cout << "pacote de requisicao" << endl; 
                std::thread t(&Server::sumRequisitionResponse, std::ref(server), packetReceived.req.value, packetReceived.seqn, &srcAddr);
                t.detach();
                //server.sumRequisitionResponse(packetReceived.req.value, packetReceived.seqn, &srcAddr); 

                break;
            }

            default: 
                cout << "pacote estranho" << endl;
                break; 
        }
    }
    return 0; 
}
