# Usando uma imagem base do GCC
FROM gcc:12.2.0

# Instalando CMake
RUN apt-get update && apt-get install -y cmake

# Definindo o diretório de trabalho
WORKDIR /app

# Copiando todo o código-fonte e o CMakeLists.txt para o container
COPY . .

# Garantindo que o diretório de build existe e está limpo
RUN rm -rf build && mkdir build

# Mudando para o diretório de build
WORKDIR /app/build

# Configurando o projeto com CMake (referenciando o diretório pai, onde está o CMakeLists.txt)
RUN cmake ..

# Compilando o projeto
RUN make

# Comando para rodar o servidor
CMD ["./Server", "8080"]
