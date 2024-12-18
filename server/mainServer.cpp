#include <thread>
#include "Server.h"
#include <cstring>

using namespace std;

int main(int argc, char* argv[]) {
    Server server(atoi(argv[1]));
    packet packetReceived; 
    sockaddr_in srcAddr; /*estrutura que recebe os dados do client*/
    while(1){
        packetReceived.type = 5; 
        packetReceived.seqn = -1; 
        srcAddr = server.receiveMessage(&packetReceived);


        if (srcAddr.sin_family == 0 &&
            srcAddr.sin_port == 0 &&
            srcAddr.sin_addr.s_addr == 0) {
            
            continue;
        }
        switch(packetReceived.type){
            case DESC: {
                std::thread d(&Server::discoverRequisitionResponse, ref(server),&srcAddr);
                d.detach();

                break;
            }
            case REQ: {
                std::thread t(&Server::sumRequisitionResponse, std::ref(server), packetReceived.req.value, packetReceived.seqn, &srcAddr);
                t.detach();

                break;
            }

            default: 
                cout << "pacote estranho" << endl;
                break; 
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    return 0; 
}
