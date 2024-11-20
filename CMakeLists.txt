cmake_minimum_required(VERSION 3.10)

# Nome do projeto
project(TrabalhoFinal)

# Configurar a versão do C++
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")

# Incluir o Google Test como subdiretório
# add_subdirectory(tests/googletest)

# Incluir o diretório dos headers e os arquivos fontes
include_directories(
    shared/include
    shared/classes/include
    client/classes/include
    server/classes/include
)

#add_executable(SocketTest 
#    tests/shared/src/SocketTest.cpp 
#    shared/src/classes/Socket.cpp
#)
#
#add_executable(SocketTest2 
#    tests/shared/src/SocketTest2.cpp 
#    shared/src/classes/Socket.cpp
#)

add_executable(Tables
    server/mainServer.cpp
    server/classes/ClientsTable.cpp
    server/classes/SumTable.cpp
)


#Linkar com o Google Test
#target_link_libraries(SocketTest gtest gtest_main pthread)
