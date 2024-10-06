#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int acc = 0;

int process_number(int number) {
    acc += number;

    return acc;
}

int main() {
    int sockfd;
    struct sockaddr_in servaddr, cliaddr;
    char buffer[BUFFER_SIZE];
    socklen_t len;
    int n;

    // Criação do socket UDP
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Erro ao criar o socket");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));

    // Configuração do endereço do servidor
    servaddr.sin_family = AF_INET; // IPv4
    servaddr.sin_addr.s_addr = INADDR_ANY; // Endereço IP local
    servaddr.sin_port = htons(PORT); // Porta do servidor

    // Bind do socket com o endereço do servidor
    if (bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("Erro ao fazer o bind");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("Servidor UDP aguardando requisições na porta %d...\n", PORT);

    while (1) {
        len = sizeof(cliaddr);

        // Recebe a mensagem do cliente
        n = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr *) &cliaddr, &len);
        buffer[n] = '\0'; // Null-terminar o buffer

        // Converte a mensagem recebida em um número inteiro
        int received_number = atoi(buffer);
        printf("Número recebido: %d\n", received_number);

        // Processa o número
        int processed_number = process_number(received_number);

        // Envia a resposta de volta ao cliente
        sprintf(buffer, "%d", processed_number);
        sendto(sockfd, buffer, strlen(buffer), 0, (const struct sockaddr *) &cliaddr, len);
        printf("Variavel acumuladora = %d\n", processed_number);
    }

    // Fecha o socket
    close(sockfd);
    return 0;
}
