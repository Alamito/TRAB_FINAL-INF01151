#include "Socket.h"
#include <gtest/gtest.h>
#include <thread>
#include <arpa/inet.h>
#include <unistd.h>

#define TEST_PORT 8080
#define TEST_IP "127.0.0.1"

// Função auxiliar para iniciar um servidor usando a classe Socket
void startTestServer() {
    Socket serverSocket(TEST_IP, TEST_PORT);

    try {
        serverSocket.create();
        serverSocket.bind();
        serverSocket.listen();
        std::cout << "Servidor: Escutando por conexões..." << std::endl;

        // Aceita a conexão do cliente
        Socket clientSocket = serverSocket.accept();
        std::cout << "Servidor: Cliente conectado!" << std::endl;

        // Envia uma mensagem ao cliente
        const char* server_message = "Hello from server!";
        clientSocket.send(server_message, strlen(server_message));

        // Aguarda uma pequena pausa para permitir que o cliente receba dados
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        // Fecha a conexão com o cliente
        clientSocket.close();
        serverSocket.close();
        std::cout << "Servidor: Conexão encerrada." << std::endl;
    } catch (const std::runtime_error& e) {
        std::cerr << "Erro no servidor: " << e.what() << std::endl;
    }
}

// Teste para a criação e conexão de sockets
TEST(SocketTest, CreateAndConnect) {
    // Inicia o servidor em uma nova thread
    std::thread server_thread(startTestServer);

    // Dê uma pequena pausa para o servidor se inicializar antes da conexão
    std::this_thread::sleep_for(std::chrono::milliseconds(50));

    Socket clientSocket(TEST_IP, TEST_PORT);

    try {
        // Teste para a criação do socket do cliente
        clientSocket.create();
        std::cout << "Cliente: Socket criado." << std::endl;

        // Teste para a conexão com o servidor
        clientSocket.connect();
        std::cout << "Cliente: Conectado ao servidor." << std::endl;

        // Envia uma mensagem ao servidor
        const char* message = "Hello, server!";
        ASSERT_GT(clientSocket.send(message, strlen(message)), 0) << "Falha no envio da mensagem";

        // Buffer para receber dados do servidor
        char buffer[1024] = {0};
        ssize_t received_bytes = clientSocket.receive(buffer, sizeof(buffer));
        ASSERT_GT(received_bytes, 0) << "Falha ao receber resposta ou tempo esgotado";

        // Verifica se a mensagem recebida é a esperada
        EXPECT_STREQ(buffer, "Hello from server!");

        clientSocket.close();
    } catch (const std::runtime_error& e) {
        FAIL() << "Exceção ocorrida: " << e.what();
    }

    // Aguarda o término da thread do servidor
    if (server_thread.joinable()) {
        server_thread.join();
    }
}

// Teste para garantir que o socket é fechado adequadamente
TEST(SocketTest, CloseSocket) {
    Socket clientSocket(TEST_IP, TEST_PORT);
    clientSocket.create();
    clientSocket.close();
    ASSERT_NO_THROW(clientSocket.close()) << "Double close should not throw";
}
