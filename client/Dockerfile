# Usando uma imagem base do gcc
FROM gcc:latest

# Definindo o diretório de trabalho
WORKDIR /app

# Copiando o código do cliente para o container
COPY client.c ./

# Compilando o código do cliente
RUN gcc -o client client.c

# Comando para rodar o cliente
CMD ["./client"]

