#ifndef ___HASH_H
#define ___HASH_H

#include "Hash.h"
#include <stdint.h>

#define NSTEK_BUCKET_SIZE 10007
#define NSTEK_REV_ENDIAN(n) ((uint16_t)(((n) >> 8) | (n) << 8))

typedef struct Traffics {
    uint32_t tx; // Transmitt 송신
    uint32_t rx; // Receive 수신
}Traffics;

typedef struct Tuples {
    uint32_t src_ip;
    uint32_t dst_ip;
    uint16_t src_port;
    uint16_t dst_port;
    uint8_t protocol;
}Tuples;

typedef struct Node {
    struct Tuples tuple;
    struct Node* next;
}Node;

typedef struct Bucket{
    struct Node* head;
    struct Traffics traffic;
    int count;
}Bucket;

Bucket* hashTable; 

uint32_t nstek_hashSession(struct Tuples tuple);

int nstek_compareSession(struct Tuples a, struct Tuples b);

struct Node* nstek_createNode(struct Tuples tuple);

void nstek_createBucket(struct Tuples tuple, struct Traffics traffic);

void nstek_removeSession(struct Tuples tuple);

uint32_t nstek_searchSession(struct Tuples tuple);

void nstek_display(void);

#endif