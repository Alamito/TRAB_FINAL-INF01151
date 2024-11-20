#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

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
	unsigned int length;
	struct sockaddr_in serv_addr, from;
	struct hostent *server;
					
	char buffer[256];
	packet pacote;
	pacote.type = 2;
	pacote.req.value = 600;
	pacote.seqn = 5;
	printf("%d", pacote.req.value);
	if (argc < 2) {
		fprintf(stderr, "usage %s hostname\n", argv[0]);
		exit(0);
	}							
	server = gethostbyname(argv[1]);
	if (server == NULL) {
		fprintf(stderr,"ERROR, no such host\n");
		exit(0);
	}	
									
	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
		printf("ERROR opening socket");
										
	serv_addr.sin_family = AF_INET;     
	serv_addr.sin_port = htons(PORT);    
	serv_addr.sin_addr = *((struct in_addr *)server->h_addr);
	bzero(&(serv_addr.sin_zero), 8);  

	/*printf("Enter the message: ");
	bzero(buffer, 256);
	fgets(buffer, 256, stdin);*/

	//n = sendto(sockfd, buffer, strlen(buffer), 0, (const struct sockaddr *) &serv_addr, sizeof(struct sockaddr_in));
	n = sendto(sockfd, &pacote, sizeof(pacote), 0, (const struct sockaddr *) &serv_addr, sizeof(struct sockaddr_in));
	if (n < 0) 
		printf("ERROR sendto");
																			
	length = sizeof(struct sockaddr_in);
	n = recvfrom(sockfd, buffer, 256, 0, (struct sockaddr *) &from, &length);
	if (n < 0)
		printf("ERROR recvfrom");

	printf("Got an ack: %s\n", buffer);
																							
	close(sockfd);
																								return 0;
}