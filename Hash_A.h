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

void nstek_headerTest(void)
{
    printf("Hello NSTEK\n");
}

uint32_t nstek_hashSession(Tuples tuple)
{
    uint32_t hash = 5381;
    hash = ((hash << 5) + hash) ^ (tuple.src_ip<<24) ^ (tuple.dst_ip<<24);
    hash = ((hash << 5) + hash) ^ (tuple.src_ip<<16) ^ (tuple.dst_ip<<16);
    hash = ((hash << 5) + hash) ^ (tuple.src_ip<<8) ^ (tuple.dst_ip<<8);
    hash = ((hash << 5) + hash) ^ (tuple.src_ip<<0) ^ (tuple.dst_ip<<0);
    hash = ((hash << 5) + hash) ^ (tuple.src_port) ^ (tuple.protocol);
    hash = ((hash << 5) + hash) ^ (tuple.dst_port) ^ (tuple.protocol);

    return hash % NSTEK_BUCKET_SIZE;
}

int nstek_compareSession(Tuples a, Tuples b)
{
    return (
            (
                (a.src_ip == b.src_ip) && (a.dst_ip == b.dst_ip) ||
                (a.src_ip == b.dst_ip) && (a.dst_ip == b.src_ip)
            ) &&
            (a.src_port == b.src_port) &&
            (a.dst_port == b.dst_port) &&
            (a.protocol == b.protocol)
        );
}

struct Node* nstek_createNode(Tuples tuple)
{
    struct Node* newNode;

    newNode = (struct Node *)malloc(sizeof(struct Node));

    newNode->tuple = tuple;
    newNode->next = NULL;

    return newNode;
}

void nstek_createBucket(Tuples tuple, Traffics traffic)
{
    uint32_t hashIndex = nstek_hashSession(tuple);
    struct Node* newNode = nstek_createNode(tuple);

    // Open addressing for other sessions
    if(hashTable[hashIndex].head && nstek_compareSession(tuple, hashTable[hashIndex].head->tuple) == 0)
        while(hashTable[hashIndex].head)
            hashIndex = hashIndex + 1 % NSTEK_BUCKET_SIZE;
    // If it is the same session, chaining is done.

    if (hashTable[hashIndex].count == 0){
        hashTable[hashIndex].count = 1;
        hashTable[hashIndex].head = newNode;
    }
    else{
        newNode->next = hashTable[hashIndex].head;
        hashTable[hashIndex].head = newNode;
        hashTable[hashIndex].count++;
    }

    // tx, rx calculater
    hashTable[hashIndex].traffic.tx += traffic.tx;
    hashTable[hashIndex].traffic.rx += traffic.rx;
}

void nstek_removeSession(Tuples tuple)
{
    uint32_t hashIndex = nstek_hashSession(tuple);
    
    int flg = 0;
    
    struct Node* node;
    struct Node* before;
    
    node = hashTable[hashIndex].head;
    
    while (node)
    {
        if (nstek_hashSession(node->tuple) == hashIndex){
            
            if (node == hashTable[hashIndex].head){
                hashTable[hashIndex].head = node->next;
            }
            else{
                before->next = node->next;
            }
            
            hashTable[hashIndex].count--;
            free(node);
            flg = 1;
        }
        before = node;
        node = node->next;
    }
}

uint32_t nstek_searchSession(Tuples tuple)
{
    uint32_t hashIndex = nstek_hashSession(tuple);
    struct Node* node = hashTable[hashIndex].head;
    int flg = 0;
    while (node)
    {
        if (nstek_hashSession(tuple) == hashIndex)
            return hashIndex;
        node = node->next;
    }
    
    return 0;
}

void nstek_display(void)
{
    struct Node* iterator;
    uint32_t firstSession = 0;
    uint32_t secondSesion = 0;
    uint32_t txTotal = 0;
    uint32_t rxTotal = 0;
    
    printf("\n+---------------------------------------------------------------------------------------------------------------------------------------+\n");
    printf("| Session\tSource IP\t\tDestination IP\t\tSource Port\tDestination Port\tProtocol\tTX\tRX\t|");
    printf("\n+---------------------------------------------------------------------------------------------------------------------------------------+\n");
    
    for (int i = 1; i<NSTEK_BUCKET_SIZE; i++){
        iterator = hashTable[i].head;
        secondSesion = hashTable[i].count - 1;

        if(hashTable[i].count)
        {
            txTotal += hashTable[i].traffic.tx;
            rxTotal += hashTable[i].traffic.rx;
            printf("| %d\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t%u\t%u\t|\n", i, hashTable[i].traffic.tx, hashTable[i].traffic.rx);
        }

        for(int j = 0; iterator; j++)
        {
            //if(j == firstSession || j == secondSesion)
                printf("|\t\t%d.%d.%d.%d\t\t%d.%d.%d.%d\t\t%u\t\t%u\t\t\t%u\t\t\t\t|\n",
                    nstek_hashSession(iterator->tuple),

                    (iterator->tuple.src_ip>>24) & 0XFF,(iterator->tuple.src_ip>>16) & 0XFF,
                    (iterator->tuple.src_ip>>8) & 0XFF,(iterator->tuple.src_ip>>0) & 0XFF,

                    (iterator->tuple.dst_ip>>24) & 0XFF,(iterator->tuple.dst_ip>>16) & 0XFF,
                    (iterator->tuple.dst_ip>>8) & 0XFF,(iterator->tuple.dst_ip>>0) & 0XFF,

                    NSTEK_REV_ENDIAN(iterator->tuple.src_port),NSTEK_REV_ENDIAN(iterator->tuple.dst_port),
                    iterator->tuple.protocol
                );
            iterator = iterator->next;
        }

        if(hashTable[i].count)
            printf("+---------------------------------------------------------------------------------------------------------------------------------------+\n");
    }
    printf("( Generated total TX - %u, RX - %u )\n",txTotal ,rxTotal);
}

#endif