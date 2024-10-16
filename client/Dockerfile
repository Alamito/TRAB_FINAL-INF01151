# Usando uma imagem base do gcc
FROM gcc:12.2.0

# Definindo o diretório de trabalho
WORKDIR /app

# Copiando o código do cliente para o container
COPY Client.cpp ./

# Compilando o código do cliente
RUN g++ -o Client Client.cpp

# Comando para rodar o cliente
CMD ["./Client"]

