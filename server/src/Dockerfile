# Usando uma imagem base do gcc
FROM gcc:12.2.0

# Definindo o diretório de trabalho
WORKDIR /app

# Copiando o código do servidor para o container
COPY server.cpp ./

# Compilando o código do servidor
RUN g++ -o server server.cpp

# Comando para rodar o servidor
CMD ["./server"]
