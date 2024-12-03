# Usando uma imagem base do gcc
FROM gcc:12.2.0

# Definindo o diretório de trabalho
WORKDIR /app

# Copiando o código do servidor e do cliente para o container
COPY server/classes/Server.cpp ./
COPY server/mainServer.cpp ./
COPY server/classes/include/SumTable.h ./
COPY server/classes/include/ClientsTable.h ./
COPY server/classes/include/Server.h ./
COPY shared/classes/include/Socket.h ./
COPY shared/include/packets.h ./
COPY shared/classes/SocketClient.cpp ./
COPY shared/classes/SocketServer.cpp ./
COPY server/classes/ClientsTable.cpp ./
COPY server/classes/SumTable.cpp ./
COPY client/classes/include/Client.h ./
COPY client/classes/Client.cpp ./
COPY client/mainClient.cpp ./


# Compilando o código do servidor e do cliente
RUN g++ -o mainServer mainServer.cpp Server.cpp ClientsTable.cpp SumTable.cpp SocketServer.cpp SocketClient.cpp -lpthread
RUN g++ -o mainClient mainClient.cpp Client.cpp SocketClient.cpp -lpthread

# Configurando o comando padrão (a ser substituído ao rodar o contêiner)
CMD ["./mainServer"]
