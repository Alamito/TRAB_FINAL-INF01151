#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_PORT 8080
#define SERVER_IP "127.0.0.1"
#define BUFFER_SIZE 1024

int main() {
    int sockfd;
    struct sockaddr_in servaddr;
    char buffer[BUFFER_SIZE];
    socklen_t len;

    // Criação do socket UDP
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Erro ao criar o socket");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));

    // Configuração do endereço do servidor
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERVER_PORT);
    servaddr.sin_addr.s_addr = inet_addr(SERVER_IP);

    for (int i = 0; i < 10; i++) {
        sprintf(buffer, "%d", i);
        printf("Enviando %d\n", i);

        // Envia o número para o servidor
        sendto(sockfd, buffer, strlen(buffer), 0, (const struct sockaddr *) &servaddr, sizeof(servaddr));

        // Recebe a resposta do servidor
        len = sizeof(servaddr);
        int n = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr *) &servaddr, &len);
        buffer[n] = '\0';

        printf("Resposta do servidor: %s\n", buffer);
    }

    // Fecha o socket
    close(sockfd);
    return 0;
}
