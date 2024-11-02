#include "Message.h"
#include <string>
#include <cstdio>
#include <stdexcept>

Message::Message(int numberToSum, const std::string& recipientIp, int recipientPort, const std::string& senderIp, int senderPort)
    : numberToSum(numberToSum) {
    recipient = Socket(recipientIp, recipientPort);  
    sender = Socket(senderIp, senderPort);         
}

void Message::send() {
    sender.create();
    sender.bind();  // Garante que o socket do cliente (sender) está vinculado à porta correta
    printf("Cliente: Socket criado e vinculado à porta %d\n", sender.getPort());

    int netOrderNumber = htonl(numberToSum);  // Converte para Big-endian
    try {
        // Especifica o IP e a porta do destinatário ao chamar send
        ssize_t numberOfReadedBytes = sender.send(&netOrderNumber, sizeof(netOrderNumber), recipient.getIp(), recipient.getPort());
        printf("Cliente: Mensagem enviada: %d para %s:%d\n", numberToSum, recipient.getIp().c_str(), recipient.getPort());
        printf("Cliente: numberOfReadedBytes: %ld bytes\n", numberOfReadedBytes);

    } catch (const std::runtime_error& e) {
        printf("Cliente: Erro ao enviar: %s\n", e.what());
        throw;
    }
}

void Message::waitAck() {
    try {
        char ackBuffer[4];
        ssize_t ackReceived = sender.receive(ackBuffer, sizeof(ackBuffer));
        printf("Cliente: ackReceived: %ld\n", ackReceived);
        if (ackReceived > 0) {
            ackBuffer[ackReceived] = '\0';  // Garante que a string esteja terminada
            printf("Cliente: Ack recebido: %s\n", ackBuffer);
        } else if (ackReceived == 0) {
            printf("Cliente: Timeout ao receber o ack\n");
        } else {
            printf("Cliente: Falha ao receber o ack\n");
        }
    } catch (const std::runtime_error& e) {
        printf("Cliente: Erro: %s\n", e.what());
        throw;
    }
}
