# Versão mínima do CMake
cmake_minimum_required(VERSION 3.10)

# Nome do projeto
project(MyProject)

# Define o padrão C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Diretório de build
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build)

# Adiciona a biblioteca compartilhada
add_library(shared STATIC
    shared/src/classes/Message.cpp
    shared/src/classes/Socket.cpp
)

# Inclui os diretórios de cabeçalhos da biblioteca compartilhada
target_include_directories(shared PUBLIC shared/src/classes/include)

# Client
add_executable(client
    client/src/classes/Client.cpp
    client/src/mainClient.cpp
)

# Inclui os diretórios de cabeçalhos do client
target_include_directories(client PUBLIC client/src/classes/include)

# Linka a biblioteca compartilhada ao client
target_link_libraries(client shared)

# Server
add_executable(server
    server/src/classes/tables/ClientsTable.cpp
    server/src/classes/tables/SumTable.cpp
    server/src/classes/Server.cpp
    server/src/mainServer.cpp
)

# Inclui os diretórios de cabeçalhos do server
target_include_directories(server PUBLIC server/src/classes/include)

# Linka a biblioteca compartilhada ao server
target_link_libraries(server shared)

# Testes
enable_testing()

# Encontra o Google Test
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

# Adiciona os executáveis de teste
add_executable(MessageTest tests/shared/src/MessageTest.cpp)

# Inclui os diretórios de cabeçalhos para os testes
target_include_directories(MessageTest PUBLIC shared/src/classes/include)

# Linka a biblioteca compartilhada e o Google Test aos testes
target_link_libraries(MessageTest shared ${GTEST_LIBRARIES} pthread)


# Adiciona os testes para o CTest
add_test(NAME MessageTest COMMAND MessageTest)