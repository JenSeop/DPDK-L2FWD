#include <stdlib.h>
#include <stdint.h>
#include "Hash.h"

struct Bucket* hashTable; 

int main(){
    hashTable = (struct Bucket *)malloc(NSTEK_BUCKET_SIZE * sizeof(struct Bucket));
    Traffics traffic = {400, 400};

    for (int i=0; i < 10; i++){
        Tuples tuple = { 3232235777 + i, 2886794753 - i, 5376, 20480, 6 + (i/10)};
        //struct Tuples tuple = { 3232235777, 2886794753, 53764, 20480, 6};
        nstek_createBucket(tuple, traffic);
    }
    // eq session 01
    for (int i=0; i < 5; i++){
        //struct Tuples tuple = { 3232235777 + i, 2886794753 - i, 53764, 20480, 6 + (i/10)};
        Tuples tuple = { 3232235777, 2886794753, 5376, 20480, 6};
        nstek_createBucket(tuple, traffic);
    }
    for (int i=0; i < 5; i++){
        //struct Tuples tuple = { 3232235777 + i, 2886794753 - i, 53764, 20480, 6 + (i/10)};
        Tuples tuple = { 2886794753, 3232235777, 5376, 20480, 6};
        nstek_createBucket(tuple, traffic);
    }
    // eq session 02
    for (int i=0; i < 5; i++){
        //struct Tuples tuple = { 3232235777 + i, 2886794753 - i, 53764, 20480, 6 + (i/10)};
        Tuples tuple = { 3232235786, 2886794753, 5376, 20480, 6};
        nstek_createBucket(tuple, traffic);
    }
    for (int i=0; i < 5; i++){
        //struct Tuples tuple = { 3232235777 + i, 2886794753 - i, 53764, 20480, 6 + (i/10)};
        struct Tuples tuple = { 2886794753, 3232235786, 5376, 20480, 6};
        nstek_createBucket(tuple, traffic);
    }

    nstek_display();

    for (int i=0; i < 10; i++){
        //struct Tuples tuple = { 3232235777 + i, 2886794753 - i, 53764, 20480, 6 + (i/10)};
        Tuples tuple = { 2886794744, 2886794753, 53764, 20480, 6};
        nstek_removeSession(tuple);
    }
}