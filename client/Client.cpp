#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>

struct Data {
    int id;
    int value;
};

class Client {
private:
    int sockfd;
    struct sockaddr_in broadcast_addr;
    const std::string BROADCAST_IP = "255.255.255.255";
    const int PORT = 4000;
    const int BUFFER_SIZE = 1024;
    int broadcastPermission;       // 0 ou 1, com bool não funcionou
    std::string message;
    struct timeval timeout;        // Struct para definir o tempo limite

public:
    Client(const std::string& msg) : message(msg), broadcastPermission(1) { 
        if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
            perror("Erro ao criar o socket");
            exit(EXIT_FAILURE);
        }

        if (setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &broadcastPermission, sizeof(broadcastPermission)) < 0) {
            perror("Erro ao habilitar broadcast no socket");
            close(sockfd);
            exit(EXIT_FAILURE);
        }

        // Configura o timeout para recebimento
        timeout.tv_sec = 0;        // Segundos
        timeout.tv_usec = 10000;   // Microsegundos (0 para não usar)
        if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) < 0) {
            perror("Erro ao configurar timeout");
            close(sockfd);
            exit(EXIT_FAILURE);
        }

        memset(&broadcast_addr, 0, sizeof(broadcast_addr));
        broadcast_addr.sin_family = AF_INET;
        broadcast_addr.sin_port = htons(PORT);
        broadcast_addr.sin_addr.s_addr = inet_addr(BROADCAST_IP.c_str());
    }

    ~Client() {
        close(sockfd);
    }

    void sendBroadcastMessage() {
        socklen_t addr_len = sizeof(broadcast_addr);
        while (true) {
            if (sendto(sockfd, message.c_str(), message.length(), 0, (struct sockaddr *) &broadcast_addr, addr_len) < 0) {
                perror("Erro ao enviar mensagem de broadcast");
                close(sockfd);
                exit(EXIT_FAILURE);
            }
            std::cout << "Mensagem de broadcast enviada pelo cliente: " << message << std::endl;

            // Aguardando resposta (opcional, pode não haver resposta em um broadcast)
            char buffer[BUFFER_SIZE];
            int n = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr *) &broadcast_addr, &addr_len);
            if (n > 0) {
                buffer[n] = '\0';
                std::cout << "Resposta recebida pelo cliente: " << buffer << std::endl;
                break;
            }
            sleep(1);
        }
    }

    bool receiveResponse() {
        char buffer[BUFFER_SIZE];
        socklen_t addr_len = sizeof(broadcast_addr);
        int n = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr *) &broadcast_addr, &addr_len);
        if (n > 0) {
            buffer[n] = '\0';
            std::cout << "Resposta recebida pelo cliente: " << buffer << std::endl;
            return true;
        }
    }

    void sendData() {
        Data data;
        data.id = 1;
        std::cout << "";
        std::cin >> data.value;

        socklen_t addr_len = sizeof(broadcast_addr);
        if (sendto(sockfd, &data, sizeof(data), 0, (struct sockaddr *) &broadcast_addr, addr_len) < 0) {
            perror("Erro ao enviar dados");
            close(sockfd);
            exit(EXIT_FAILURE);
        }
        std::cout << "Dados enviados pelo cliente: id = " << data.id << ", value = " << data.value << std::endl;
    }
};

int main() {
    Client client("Mensagem de BROADCAST!");
    client.sendBroadcastMessage();
    if (client.receiveResponse()) {
        client.sendData();
    }
    return 0;
}
