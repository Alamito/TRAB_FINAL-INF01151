#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>

class BroadcastClient {
private:
    int sockfd;
    struct sockaddr_in broadcast_addr;
    const std::string BROADCAST_IP = "255.255.255.255";
    const int PORT = 4000;
    const int BUFFER_SIZE = 1024;
    int broadcastPermission;       // 0 ou 1, com bool não funcionou
    std::string message;

    std::string getLocalIP() {
        char hostbuffer[256];
        char *IPbuffer;
        struct hostent *host_entry;

        // Recupera o nome do host
        if (gethostname(hostbuffer, sizeof(hostbuffer)) == -1) {
            perror("Erro ao obter o nome do host");
            exit(EXIT_FAILURE);
        }

        // Recupera o endereço IP usando o nome do host
        if ((host_entry = gethostbyname(hostbuffer)) == nullptr) {
            perror("Erro ao obter o endereço IP");
            exit(EXIT_FAILURE);
        }

        // Converte o endereço IP para uma string
        IPbuffer = inet_ntoa(*((struct in_addr*) host_entry->h_addr_list[0]));

        return std::string(IPbuffer);
    }

public:
    BroadcastClient(const std::string& msg) : message(msg), broadcastPermission(1) { 
        if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
            perror("Erro ao criar o socket");
            exit(EXIT_FAILURE);
        }

        if (setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &broadcastPermission, sizeof(broadcastPermission)) < 0) {
            perror("Erro ao habilitar broadcast no socket");
            close(sockfd);
            exit(EXIT_FAILURE);
        }

        memset(&broadcast_addr, 0, sizeof(broadcast_addr));
        broadcast_addr.sin_family = AF_INET;
        broadcast_addr.sin_port = htons(PORT);
        broadcast_addr.sin_addr.s_addr = inet_addr(BROADCAST_IP.c_str());

        // Adiciona o IP local à mensagem
        message += "(Cliente IP: " + getLocalIP() + ")";
    }

    ~BroadcastClient() {
        close(sockfd);
    }

    void sendMessage() {
        socklen_t addr_len = sizeof(broadcast_addr);
        if (sendto(sockfd, message.c_str(), message.length(), 0, (struct sockaddr *) &broadcast_addr, addr_len) < 0) {
            perror("Erro ao enviar mensagem de broadcast");
            close(sockfd);
            exit(EXIT_FAILURE);
        }
        std::cout << "Mensagem de broadcast enviada pelo cliente: " << message << std::endl;
    }

    void receiveResponse() {
        char buffer[BUFFER_SIZE];
        socklen_t addr_len = sizeof(broadcast_addr);
        int n = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr *) &broadcast_addr, &addr_len);
        if (n > 0) {
            buffer[n] = '\0';
            std::cout << "Resposta recebida pelo cliente: " << buffer << std::endl;
        }
    }
};

int main() {
    BroadcastClient client("Mensagem de BROADCAST!");
    client.sendMessage();
    client.receiveResponse();
    return 0;
}