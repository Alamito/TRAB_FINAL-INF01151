#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>

class Server {
private:
    int sockfd;
    struct sockaddr_in servaddr, cliaddr;
    const int PORT = 4000;
    const int BUFFER_SIZE = 1024;

public:
    Server() {
        if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
            perror("Erro ao criar o socket");
            exit(EXIT_FAILURE);
        }

        memset(&servaddr, 0, sizeof(servaddr));
        memset(&cliaddr, 0, sizeof(cliaddr));

        servaddr.sin_family = AF_INET;
        servaddr.sin_addr.s_addr = INADDR_ANY;
        servaddr.sin_port = htons(PORT);

        if (bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
            perror("Erro ao fazer o bind");
            close(sockfd);
            exit(EXIT_FAILURE);
        }

        std::cout << "Servidor aguardando mensagens..." << std::endl;
    }

    ~Server() {
        close(sockfd);
    }

    void run() {
        char buffer[BUFFER_SIZE];
        socklen_t len = sizeof(cliaddr);
        while (true) {
            int n = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr *) &cliaddr, &len);
            buffer[n] = '\0';

            char client_ip[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &(cliaddr.sin_addr), client_ip, INET_ADDRSTRLEN);

            std::cout << "Mensagem recebida pelo servidor de " << client_ip << ": " << buffer << std::endl;

            char local_ip[BUFFER_SIZE];
            getLocalIp(local_ip, sizeof(local_ip));

            snprintf(buffer, BUFFER_SIZE, "(Servidor IP: %s)", local_ip);
            sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr *) &cliaddr, len);

            std::cout << "Resposta enviada ao cliente: " << buffer << std::endl;
        }
    }

private:
    void getLocalIp(char *buffer, size_t buffer_len) {
        char hostname[1024];
        struct hostent *host_entry;

        if (gethostname(hostname, sizeof(hostname)) == -1) {
            perror("Erro ao obter o nome do host");
            exit(EXIT_FAILURE);
        }

        if ((host_entry = gethostbyname(hostname)) == NULL) {
            perror("Erro ao obter informações do host");
            exit(EXIT_FAILURE);
        }

        strncpy(buffer, inet_ntoa(*((struct in_addr*) host_entry->h_addr_list[0])), buffer_len);
    }
};

int main() {
    Server server;
    server.run();
    return 0;
}
