#ifndef ___HASH_H
#define ___HASH_H

#include <stdint.h>
#include "Hash.h"

#define NSTEK_BUCKET_SIZE 10007
#define NSTEK_REV_ENDIAN(n) ((uint16_t)(((n) >> 8) | (n) << 8))

struct Traffics {
    uint32_t tx; // Transmitt 송신
    uint32_t rx; // Receive 수신
};

struct Tuples {
    uint32_t src_ip;
    uint32_t dst_ip;
    uint16_t src_port;
    uint16_t dst_port;
    uint8_t protocol;
};

struct Node {
    struct Tuples tuple;
    struct Node* next;
};

struct Bucket{
    struct Node* head;
    struct Traffics traffic;
    int count;
};

struct Bucket* hashTable; 

static uint32_t nstek_hashSession(struct Tuples tuple);

static int nstek_compareSession(struct Tuples a, struct Tuples b);

static struct Node* nstek_createNode(struct Tuples tuple);

static void nstek_createBucket(struct Tuples tuple, struct Traffics traffic);

static void nstek_removeSession(struct Tuples tuple);

static uint32_t nstek_searchSession(struct Tuples tuple);

static void nstek_display(void);

#endif