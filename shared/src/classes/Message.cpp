#include "Message.h"
#include <string>
#include <cstdio>
#include <stdexcept>
#include <cstring>
#include <arpa/inet.h>

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

ServerResponse Message::waitAck() {
    char responseBuffer[sizeof(ServerResponse)];
    ssize_t ackReceived = -1;
    int retry = 0;

    while (ackReceived <= 0) {
        ackReceived = sender.receive(responseBuffer, sizeof(responseBuffer));

        if (ackReceived > 0) {
            printf("Cliente: Ack recebido\n");

            // Desserializa os dados recebidos no buffer para a struct ServerResponse
            ServerResponse response;
            memcpy(&response, responseBuffer, sizeof(ServerResponse));

            // Exibir os dados recebidos
            printf("Cliente: IP do servidor: %s\n", response.serverIp);
            printf("Cliente: Soma total: %f\n", response.totalSum);

            return response;  // Ack recebido com sucesso, retorna a struct
        }
        retry++;
        printf("Cliente: Retry %d\n", retry);
    }

    printf("Cliente: Tentativas esgotadas\n");
    return {"", 0.0};  // Retorna uma struct vazia indicando falha
}
