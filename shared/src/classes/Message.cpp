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
    char ackBuffer[4];
    ssize_t ackReceived = -1;
    int retry = 0;

    while (ackReceived <= 0) {
        ackReceived = sender.receive(ackBuffer, sizeof(ackBuffer));
        printf("Cliente: ackReceived: %ld\n", ackReceived);

        if (ackReceived > 0) {
            if (ackReceived < sizeof(ackBuffer)) {
                ackBuffer[ackReceived] = '\0';  // Garante que a string esteja terminada
            } else {
                ackBuffer[sizeof(ackBuffer) - 1] = '\0';  // Evita estouro de buffer
            }
            printf("Cliente: Ack recebido: %s\n", ackBuffer);
            return;  // Ack recebido com sucesso, sai da função
        }
        retry++;
        printf("Cliente: Retry %d\n", retry);
    }

    printf("Cliente: Tentativas esgotadas\n");
}