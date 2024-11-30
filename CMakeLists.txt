cmake_minimum_required(VERSION 3.10)

# Nome do projeto
project(TrabalhoFinal)

# Configurar a versão do C++
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")

# Incluir o diretório dos headers e os arquivos fontes
include_directories(
    shared/include
    shared/classes/include
    client/classes/include
    server/classes/include
)

# Adicionar executável para o servidor
add_executable(Server
    server/mainServer.cpp
    shared/classes/SocketServer.cpp
)

# Adicionar executável para o cliente
add_executable(Client
    client/mainClient.cpp
    client/classes/Client.cpp
    shared/classes/SocketClient.cpp
)

#Linkar com o Google Test
#target_link_libraries(SocketTest gtest gtest_main pthread)