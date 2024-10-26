#include "Socket.h"

#include <stdexcept>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define DEFAULT_IP "127.0.0.1"
#define DEFAULT_PORT 8080

Socket::Socket(const std::string& ip, int port)
    : ip(ip), port(port), socketFd(-1) {}

void Socket::create() {
    socketFd = socket(AF_INET, SOCK_STREAM, 0);
    if (socketFd < 0) throw std::runtime_error("Failed to create socket");
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    if (inet_pton(AF_INET, ip.c_str(), &serverAddr.sin_addr) <= 0)
        throw std::runtime_error("Invalid IP address");
}

void Socket::connect() {
    if (::connect(socketFd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0)
        throw std::runtime_error("Connection failed");
}

void Socket::bind() {
    if (::bind(socketFd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0)
        throw std::runtime_error("Bind failed");
}

void Socket::listen(int backlog) {   // backlog = limite de conexões que podem estar em fila aguardando para serem aceitas pela função accept()
    if (::listen(socketFd, backlog) < 0)
        throw std::runtime_error("Listen failed");
}

Socket Socket::accept() {
    int new_socketFd = ::accept(socketFd, nullptr, nullptr);
    if (new_socketFd < 0)
        throw std::runtime_error("Accept failed");

    Socket new_socket;
    new_socket.socketFd = new_socketFd;
    return new_socket;
}

ssize_t Socket::send(const void* data, size_t size) const {
    return ::send(socketFd, data, size, 0);
}

ssize_t Socket::receive(void* buffer, size_t size) const {
    return ::recv(socketFd, buffer, size, 0);
}

void Socket::close() {
    if (socketFd >= 0) {
        ::close(socketFd);
        socketFd = -1;
    }
}

Socket::~Socket() {
    close();
}

