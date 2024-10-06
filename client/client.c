#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>

#define BROADCAST_IP "255.255.255.255"  // Endereço de broadcast
#define PORT 9000                        // Porta do servidor
#define BUFFER_SIZE 1024                 // Tamanho do buffer para receber a resposta

int main() {
    int sockfd;
    struct sockaddr_in broadcast_addr;
    char *message = "Mensagem de broadcast!";
    char buffer[BUFFER_SIZE];
    int broadcastPermission = 1;  // Permissão para usar broadcast
    socklen_t addr_len = sizeof(broadcast_addr);

    // Criando o socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Erro ao criar o socket");
        exit(EXIT_FAILURE);
    }

    // Configurar o socket para permitir envio de pacotes de broadcast
    if (setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &broadcastPermission, sizeof(broadcastPermission)) < 0) {
        perror("Erro ao habilitar broadcast no socket");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // Configurando o endereço de broadcast
    memset(&broadcast_addr, 0, sizeof(broadcast_addr));
    broadcast_addr.sin_family = AF_INET;
    broadcast_addr.sin_port = htons(PORT);
    broadcast_addr.sin_addr.s_addr = inet_addr(BROADCAST_IP);  // Endereço de broadcast

    // Enviando mensagem de broadcast
    if (sendto(sockfd, message, strlen(message), 0, (struct sockaddr *) &broadcast_addr, addr_len) < 0) {
        perror("Erro ao enviar mensagem de broadcast");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("Mensagem de broadcast enviada: %s\n", message);

    // Aguardando resposta (opcional, pode não haver resposta em um broadcast)
    int n = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr *) &broadcast_addr, &addr_len);
    if (n > 0) {
        buffer[n] = '\0';
        printf("Resposta recebida: %s\n", buffer);
    }

    // Fechando o socket
    close(sockfd);

    return 0;
}
