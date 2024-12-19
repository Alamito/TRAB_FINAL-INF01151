#include <thread>
#include "Server.h"
#include <cstring>

using namespace std;

int main(int argc, char* argv[]) {
    Server server(atoi(argv[1])); // Inicializa o servidor na porta fornecida
    packet packetReceived;
    sockaddr_in srcAddr; /* Estrutura que recebe os dados do client */

    server.setIsLeader(false); // Inicialmente, o servidor não é líder
    server.findCoordinatorMessage(); // Encontra o coordenador
    
    while (true) {
        packetReceived.type = 5; 
        packetReceived.seqn = -1; 

        
        // Recebe mensagens de outros servidores ou clientes
        srcAddr = server.receiveMessage(&packetReceived);
        
        // Ignora mensagens inválidas
        if (srcAddr.sin_family == 0 &&
            srcAddr.sin_port == 0 &&
            srcAddr.sin_addr.s_addr == 0) {
            continue;
        }

        // Processa diferentes tipos de mensagens
        switch (packetReceived.type) {

            case DISC_LEADER: {
                // Mensagem de descoberta de líder
                if (server.getIsLeader()) {
                    // Responde com a mensagem de coordenador
                    server.sendCoordinatorMessage(&srcAddr);
                    //std::thread e(&Server::sendCoordinatorMessage, ref(server), &srcAddr);
                    //e.detach();
                }
                break;
            }

            case COORDINATOR: {
                // Mensagem de coordenador (novo líder eleito)
                // server.sendCoordinatorMessage();
                printf("Mensagem do coordenador\n");
                break;
            }

            // case ELECTION: {
            //     // Verifica se o ID do servidor atual é maior que o emissor
            //     if (server.getId() > packetReceived.senderId) {
            //         // Envia uma mensagem de resposta ao emissor da eleição
            //         server.sendElectionAnswer(packetReceived.senderId);
            //     }
            //     // Continua o processo de eleição
            //     //std::thread e(&Server::electLeader, ref(server));
            //     //e.detach();
            //     break;
            // }

            // case ELECTION_ANSWER: {
            //     // Mensagem de resposta à eleição
            //     server.registerElectionAnswer(packetReceived.senderId);
            //     break;
            // }  

            case BACKUP: {
                if (!server.getIsLeader()) {
                    printf("Backup recebido: %d\n", packetReceived.backupData);
                }
                break;
            }

            case DESC: {
                // Requisição de descoberta (ex.: cliente ou backup)
                std::thread d(&Server::discoverRequisitionResponse, ref(server), &srcAddr);
                d.detach();
                break;
            }

            case REQ: {
                // Requisição de soma de um cliente
                std::thread t(&Server::sumRequisitionResponse, ref(server), packetReceived.req.value, packetReceived.seqn, &srcAddr);
                t.detach();
                if (server.getIsLeader()) {
                    //Envia mensagens de backup para os servidores
                    server.sendBackup();
                    //d::thread b(&Server::sendBackup, ref(server), &srcAddr);
                    //detach();
                }
                break;
            }

            default: 
                cout << "Pacote estranho recebido." << endl;
                break; 
        }

        // Verifica timeout para líder (apenas servidores backup devem fazer isso)
        if (!server.getIsLeader() && server.leaderTimeout()) {
            cout << "Líder falhou. Iniciando nova eleição." << endl;
            //server.sendElectionMessage(); // Envia mensagens de eleição
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(1)); // Pequeno delay no loop principal
    }

    return 0;
}


