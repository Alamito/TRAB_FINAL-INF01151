#ifndef __PACKET_H__
#define __PACKET_H__

#define SIZE_BUFFER 1024
#define IP_SIZE 16

#define DESC 1
#define REQ 2
#define DESC_ACK 3
#define REQ_ACK 4

struct requisicao {
    uint64_t value; // Valor da requsição
};

struct requisicao_ack {
    uint64_t seqn;      //Número de sequência que está sendo feito o ack
    uint64_t num_reqs;  // Quantidade de requisições
    uint64_t total_sum; // Valor da soma agregada até o momento
};

typedef struct __packet {
    uint64_t type;      // Tipo do pacote (DESC | REQ | DESC_ACK | REQ_ACK )
    uint64_t seqn;      //Número de sequência de uma requisição
    
    union {
        struct requisicao req;
        struct requisicao_ack ack;
    };
} packet;


#endif
