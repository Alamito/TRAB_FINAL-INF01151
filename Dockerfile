# Usando uma imagem base do gcc
FROM gcc:12.2.0

# Definindo o diretório de trabalho
WORKDIR /app

# Copiando o código do servidor para o container
COPY server/classes/Server.cpp ./
COPY server/mainServer.cpp ./
COPY server/classes/include/SumTable.h ./
COPY server/classes/include/ClientsTable.h ./
COPY server/classes/include/Server.h ./
COPY shared/classes/include/Socket.h ./
COPY shared/include/packets.h ./
COPY shared/classes/Socket.cpp ./
COPY server/classes/ClientsTable.cpp ./
COPY server/classes/SumTable.cpp ./

# Compilando o código do servidor
RUN g++ -o mainServer mainServer.cpp Server.cpp Socket.cpp ClientsTable.cpp SumTable.cpp -lpthread

# Expondo a porta 8080/udp
EXPOSE 8080/udp

# Comando para rodar o servidor
CMD ["./mainServer"]
