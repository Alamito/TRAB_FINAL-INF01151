# Usando uma imagem base do gcc
FROM gcc:12.2.0

# Definindo o diretório de trabalho
WORKDIR /app

# Copiando o código do servidor para o container
COPY server.c ./

# Compilando o código do servidor
RUN gcc -o server server.c

# Comando para rodar o servidor
CMD ["./server"]
