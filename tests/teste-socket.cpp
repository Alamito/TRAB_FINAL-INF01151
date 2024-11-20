#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <netdb.h>
#include <stdio.h>

#define PORT 4000
#define SIZE_BUFFER 1024
#define DESC 1
#define REQ 2
#define DESC_ACK 3
#define REQ_ACK 4

struct requisicao {
    uint16_t value; // Valor da requsição
};

struct requisicao_ack {
    uint16_t seqn;      //Número de sequência que está sendo feito o ack
    uint16_t num_reqs;  // Quantidade de requisições
    uint16_t total_sum; // Valor da soma agregada até o momento
};

typedef struct __packet {
    uint16_t type;      // Tipo do pacote (DESC | REQ | DESC_ACK | REQ_ACK )
    uint16_t seqn;      //Número de sequência de uma requisição
    
    union {
        struct requisicao req;
        struct requisicao_ack ack;
    };
} packet;



int main(int argc, char *argv[])
{
	int sockfd, n;
	socklen_t clilen;
	struct sockaddr_in serv_addr, cli_addr;
	char buf[1024];
		
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) 
		printf("ERROR opening socket");

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	bzero(&(serv_addr.sin_zero), 8);    
	 
	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(struct sockaddr)) < 0) 
		printf("ERROR on binding");
	
	clilen = sizeof(struct sockaddr_in);

	packet pacote;
	pacote.ack.total_sum = 100;
	pacote.type = 4;

	while (1) {
		/* receive from socket */
		n = recvfrom(sockfd, buf, 1024, 0, (struct sockaddr *) &cli_addr, &clilen);
		if (n < 0) 
			printf("ERROR on recvfrom");
		printf("Received a datagram: %s\n", buf);
		
		/* send to socket */
		n = sendto(sockfd, &pacote, sizeof(pacote), 0,(struct sockaddr *) &cli_addr, sizeof(struct sockaddr));
		if (n  < 0) 
			printf("ERROR on sendto");
	}
	
	close(sockfd);
	return 0;
}