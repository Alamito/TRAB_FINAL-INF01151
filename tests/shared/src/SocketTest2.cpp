#include "Socket.h"
#include <iostream>
#include <string>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8080
#define BACKLOG 5

void run_server() {
    std::cout << "[SERVER] Inicializando servidor em " << SERVER_IP << ":" << SERVER_PORT << std::endl;
    Socket server(SERVER_IP, SERVER_PORT);

    try {
        server.create();
        std::cout << "[SERVER] Socket criado com sucesso. FD: " << server.getSocketFd() << std::endl;

        server.bind();
        std::cout << "[SERVER] Vinculação (bind) realizada. Aguardando conexões na porta " << SERVER_PORT << std::endl;

        server.listen(BACKLOG);
        std::cout << "[SERVER] Em modo de escuta com backlog de " << BACKLOG << std::endl;

        Socket client_socket = server.accept();
        std::cout << "[SERVER] Conexão aceita. Cliente conectado." << std::endl;

        char buffer[1024] = {0};
        ssize_t bytes_received = client_socket.receive(buffer, sizeof(buffer));
        std::cout << "[SERVER] Mensagem recebida: " << buffer << " (" << bytes_received << " bytes)" << std::endl;

        client_socket.close();
        std::cout << "[SERVER] Conexão com cliente encerrada." << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "[SERVER] Erro: " << e.what() << std::endl;
    }

    server.close();
    std::cout << "[SERVER] Servidor encerrado." << std::endl;
}

void run_client() {
    std::cout << "[CLIENT] Inicializando cliente e tentando se conectar a " << SERVER_IP << ":" << SERVER_PORT << std::endl;
    Socket client(SERVER_IP, SERVER_PORT);

    try {
        client.create();
        std::cout << "[CLIENT] Socket criado com sucesso. FD: " << client.getSocketFd() << std::endl;

        client.connect();
        std::cout << "[CLIENT] Conexão estabelecida com o servidor " << SERVER_IP << ":" << SERVER_PORT << std::endl;

        std::string message = "Hello, Server!";
        ssize_t bytes_sent = client.send(message.c_str(), message.size());
        std::cout << "[CLIENT] Mensagem enviada: " << message << " (" << bytes_sent << " bytes)" << std::endl;

        client.close();
        std::cout << "[CLIENT] Conexão com o servidor encerrada." << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "[CLIENT] Erro: " << e.what() << std::endl;
    }
}

int main() {
    int choice;
    std::cout << "Escolha o modo:\n1. Servidor\n2. Cliente\n> ";
    std::cin >> choice;

    if (choice == 1) {
        run_server();
    } else if (choice == 2) {
        run_client();
    } else {
        std::cout << "Opção inválida!" << std::endl;
    }

    return 0;
}
