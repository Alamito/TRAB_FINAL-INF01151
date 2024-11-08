#include "Message.h"
#include <gtest/gtest.h>
#include <thread>
#include <chrono>

#define SENDER_PORT 8080
#define SENDER_IP "127.0.0.1"

#define RECIPIENT_PORT 8081 // Porta do servidor
#define RECIPIENT_IP "172.17.0.2" // IP do servidor  

TEST(MessageTest, SendMessageAndWaitAck) {
    int numberToSum = 42;

    // Dê uma pequena pausa para garantir que o servidor esteja pronto no terminal
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    try {
        // Cria a instância da mensagem e envia (que também recebe o ack)
        Message message(numberToSum, RECIPIENT_IP, RECIPIENT_PORT, SENDER_IP, SENDER_PORT);
        message.send();
        message.waitAck();
    } catch (const std::runtime_error& e) {
        FAIL() << "Exceção ocorrida: " << e.what();
    }
}


// Função main para executar os testes
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
