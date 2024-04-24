#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define BUCKET_SIZE 10007 // 20011 10007 100003
#define REV_ENDIAN(n) ((uint16_t)(((n) >> 8) | (n) << 8))

#define PKT 10000
int pkt = PKT;
int loss = PKT;

struct Bucket* hashTable = NULL; 

struct Tuples {
    uint32_t src_ip;
    uint32_t dst_ip;
    uint16_t src_port;
    uint16_t dst_port;
    uint8_t protocol;
};

struct Node {
    uint32_t id;
    struct Tuples tuple;
    struct Node* next;
};

struct Bucket{
    struct Node* head;
    int count;
};

uint32_t hashSession(struct Tuples tuple){
    uint32_t hash = 5381;
    hash = ((hash << 5) + hash) ^ (tuple.src_ip<<24) ^ (tuple.dst_ip<<24);
    hash = ((hash << 5) + hash) ^ (tuple.src_ip<<16) ^ (tuple.dst_ip<<16);
    hash = ((hash << 5) + hash) ^ (tuple.src_ip<<8) ^ (tuple.dst_ip<<8);
    hash = ((hash << 5) + hash) ^ (tuple.src_ip<<0) ^ (tuple.dst_ip<<0);
    hash = ((hash << 5) + hash) ^ (tuple.src_port) ^ (tuple.protocol);
    hash = ((hash << 5) + hash) ^ (tuple.dst_port) ^ (tuple.protocol);

    return hash % BUCKET_SIZE;
}

struct Node* createNode(uint32_t id, struct Tuples tuple){
    struct Node* newNode;

    newNode = (struct Node *)malloc(sizeof(struct Node));

    newNode->id = id;
    newNode->tuple = tuple;
    newNode->next = NULL;

    return newNode;
}

void add(uint32_t id, struct Tuples tuple){
    
    uint32_t hashIndex = hashSession(tuple);
    
    struct Node* newNode = createNode(id, tuple);
    
    loss--;

    if (hashTable[hashIndex].count == 0){
        hashTable[hashIndex].count = 1;
        hashTable[hashIndex].head = newNode;
    }
    else{
        newNode->next = hashTable[hashIndex].head;
        hashTable[hashIndex].head = newNode;
        hashTable[hashIndex].count++;
    }
}

void remove_id(uint32_t id, struct Tuples tuple){
    uint32_t hashIndex = hashSession(tuple);
    
    int flg = 0;
    
    struct Node* node;
    struct Node* before;
    
    node = hashTable[hashIndex].head;
    
    while (node)
    {
        if (node->id == id){
            
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
    
    if (flg)
        printf("\n [ %d ] deleted.\n", id);
    else
        printf("\n [ %d ] donst exist.\n", id);
}

void search(uint32_t id, struct Tuples tuple){
    uint32_t hashIndex = hashSession(tuple);
    struct Node* node = hashTable[hashIndex].head;
    int flg = 0;
    while (node)
    {
        if (node->id == id){
            flg = 1;
            break;
        }
        node = node->next;
    }
    if (flg)
        printf("\n id [ %d ]\n", node->id);
    else
        printf("\n dont exist. \n");
}

void display(){
    
    struct Node* iterator;
    printf("\n========= display start ========= \n");
    for (int i = 0; i<BUCKET_SIZE; i++){
        int flg = 0;
        iterator = hashTable[i].head;
        if(hashTable[i].count)
            printf("\nBucket[%d] :\n", i);
        while (iterator != NULL)
        {
            printf("Session[%u]\tsrc_ip %d.%d.%d.%d\tdst_ip %d.%d.%d.%d\tsrc_port %u\tdst_port %u\tproto %u\n",
                hashSession(iterator->tuple),

                (iterator->tuple.src_ip>>24) & 0XFF,(iterator->tuple.src_ip>>16) & 0XFF,
                (iterator->tuple.src_ip>>8) & 0XFF,(iterator->tuple.src_ip>>0) & 0XFF,

                (iterator->tuple.dst_ip>>24) & 0XFF,(iterator->tuple.dst_ip>>16) & 0XFF,
                (iterator->tuple.dst_ip>>8) & 0XFF,(iterator->tuple.dst_ip>>0) & 0XFF,

                REV_ENDIAN(iterator->tuple.src_port),REV_ENDIAN(iterator->tuple.dst_port),
                iterator->tuple.protocol
            );
            iterator = iterator->next;
            flg++;
        }
    }
    printf("\n========= display complete ========= \n");
    printf("%d\n",loss);
}

int main(){
    
    hashTable = (struct Bucket *)malloc(BUCKET_SIZE * sizeof(struct Bucket));
    
    for (int i=0; i < pkt; i++){
        struct Tuples tuple = { 3232235777 + i, 2886794753 - i, 53764, 20480, 6 + (i/10)};
        //struct Tuples tuple = { 3232235777, 2886794753, 53764, 20480, 6};
        add(i, tuple);
    }

for (int i=0; i < 100; i++){
        //struct Tuples tuple = { 3232235777 + i, 2886794753 - i, 53764, 20480, 6 + (i/10)};
        struct Tuples tuple = { 3232235777, 2886794753, 53764, 20480, 6};
        add(i, tuple);
    }

    display();
}