#include <iostream>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <ifaddrs.h>

#define PORT 8081
#define BUFFER_SIZE 1024

struct ServerResponse {
    char serverIp[INET_ADDRSTRLEN];
    double totalSum;
};

// Função para obter o IP do servidor
void getIp(char* ipBuffer, const char* interface = "eth0") {
    struct ifaddrs *ifAddrStruct = nullptr;
    getifaddrs(&ifAddrStruct);

    for (struct ifaddrs *ifa = ifAddrStruct; ifa != nullptr; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr && ifa->ifa_addr->sa_family == AF_INET) { // Interface IPv4
            if (strcmp(ifa->ifa_name, interface) == 0) { // Substitua "eth0" pela interface desejada
                inet_ntop(AF_INET, &((struct sockaddr_in *)ifa->ifa_addr)->sin_addr, ipBuffer, INET_ADDRSTRLEN);
                break;
            }
        }
    }
    if (ifAddrStruct != nullptr) freeifaddrs(ifAddrStruct);
}

int main() {
    int serverSocket;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t addrLen = sizeof(clientAddr);
    int buffer;  // Buffer para receber o número

    // Criação do socket UDP
    serverSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (serverSocket < 0) {
        perror("Erro ao criar o socket");
        return -1;
    }

    // Inicializa a estrutura clientAddr com zeros
    memset(&clientAddr, 0, sizeof(clientAddr));

    printf("clientAddr antes de chamar o recvfrom: %s\n", inet_ntoa(clientAddr.sin_addr));    

    // Configuração do endereço do servidor
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);

    // Bind do socket
    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Erro ao realizar o bind");
        close(serverSocket);
        return -1;
    }
    std::cout << "Servidor UDP com IP " << inet_ntoa(serverAddr.sin_addr) << " aguardando na porta " << PORT << "..." << std::endl;

    // Receber mensagem do cliente (listen)
    ssize_t receivedBytes = recvfrom(serverSocket, &buffer, sizeof(buffer), 0, (struct sockaddr*)&clientAddr, &addrLen);
    printf("clientAddr depois de receber o recvfrom: %s\n", inet_ntoa(clientAddr.sin_addr));    //recvfrom recebe no header o endereço e porta do cliente.
    if (receivedBytes < 0) {
        perror("Erro ao receber dados");
    } else if (receivedBytes != sizeof(buffer)) {
        std::cerr << "Erro: Tamanho de dados recebidos inesperado." << std::endl;
    } else {
        // Converte o valor para a ordem de bytes do host
        int receivedNumber = ntohl(buffer);
        std::cout << "Servidor: Mensagem recebida: " << receivedNumber << std::endl;

        // Enviar o ServerResponse para o cliente
        ServerResponse responseToClient;
        getIp(responseToClient.serverIp);  // Obter o IP do servidor
        responseToClient.totalSum = receivedNumber;
        
        ssize_t sentBytes = sendto(serverSocket, &responseToClient, sizeof(responseToClient), 0, (struct sockaddr*)&clientAddr, addrLen);
        if (sentBytes < 0) {
            perror("Erro ao enviar o ACK");
        } else {
            std::cout << "Servidor: Ack enviado para o cliente." << std::endl;
        }
    }

    // Fechar o socket
    close(serverSocket);
    return 0;
}
