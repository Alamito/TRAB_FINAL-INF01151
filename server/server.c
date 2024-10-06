#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>

#define PORT 9000
#define BUFFER_SIZE 1024

// Função para obter o endereço IP da máquina local
void get_local_ip(char *buffer, size_t buffer_len) {
    char hostname[1024];
    struct hostent *host_entry;

    // Obtém o nome do host (nome da máquina)
    if (gethostname(hostname, sizeof(hostname)) == -1) {
        perror("Erro ao obter o nome do host");
        exit(EXIT_FAILURE);
    }

    // Obtém as informações de rede do host
    if ((host_entry = gethostbyname(hostname)) == NULL) {
        perror("Erro ao obter informações do host");
        exit(EXIT_FAILURE);
    }

    // Converte o endereço IP para uma string e armazena no buffer
    strncpy(buffer, inet_ntoa(*((struct in_addr*) host_entry->h_addr_list[0])), buffer_len);
}

int main() {
    int sockfd;
    struct sockaddr_in servaddr, cliaddr;
    char buffer[BUFFER_SIZE];
    socklen_t len;

    // Criação do socket UDP
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Erro ao criar o socket");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));

    // Configuração do endereço do servidor
    servaddr.sin_family = AF_INET; // IPv4
    servaddr.sin_addr.s_addr = INADDR_ANY; // Aceita conexões de qualquer IP
    servaddr.sin_port = htons(PORT); // Porta do servidor

    // Bind do socket com o endereço do servidor
    if (bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("Erro ao fazer o bind");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("Servidor aguardando mensagens...\n");

    while (1) {
        len = sizeof(cliaddr);
        
        // Receber mensagem do cliente
        int n = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr *) &cliaddr, &len);
        buffer[n] = '\0'; // Null-terminar o buffer

        printf("Mensagem recebida do cliente: %s\n", buffer);

        // Obter o endereço IP local do servidor
        char local_ip[BUFFER_SIZE];
        get_local_ip(local_ip, sizeof(local_ip));

        // Criar a resposta contendo o IP e a porta
        snprintf(buffer, BUFFER_SIZE, "Servidor IP: %s", local_ip);

        // Enviar a resposta ao cliente
        sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr *) &cliaddr, len);
        printf("Resposta enviada ao cliente: %s\n", buffer);
    }

    // Fechar o socket
    close(sockfd);
    return 0;
}
