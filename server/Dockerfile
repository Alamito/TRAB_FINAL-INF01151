# Usando uma imagem base do gcc
FROM gcc:latest

# Definindo o diretório de trabalho
WORKDIR /app

# Copiando o código do servidor para o container
COPY server.c ./

# Compilando o código do servidor
RUN gcc -o server server.c

RUN ls -l /app

# Comando para rodar o servidor
CMD ["./server"]
