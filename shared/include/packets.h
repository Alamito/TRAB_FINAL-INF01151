#ifndef __PACKET_H__
#define __PACKET_H__

#define SIZE_BUFFER 1024


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
    }
} packet;


#endif