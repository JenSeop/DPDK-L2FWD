#include <stdio.h>
#include <stdint.h>

#define HASH_TABLE_SIZE 256
#define REV_ENDIAN(n) ((uint16_t)(((n) >> 8) | (n) << 8))

uint32_t hash(uint32_t src_ip, uint32_t dst_ip, uint16_t src_port, uint16_t dst_port, uint8_t proto)
{
    uint32_t session = src_ip + dst_ip + src_port + dst_port + proto;
    uint32_t hash = (session >> 0) + (session >> 8) + (session >> 16) + (session >> 24);

    return hash % HASH_TABLE_SIZE;
}

int main(void)
{
    uint32_t src_ip = 0xFEFFFFFE; // 198.19.1.1
    uint32_t dst_ip = 0xDFFFFFFE; // 198.19.1.2
    uint8_t proto = 6;
    uint16_t src_port = 1024;
    uint16_t dst_port = 1025;

    printf("IP dst = %d.%d.%d.%d, IP src = %d.%d.%d.%d, port dst = %u, port src = %u, proto = %u\n",
        (dst_ip>>24) & 0XFF, // dst ip
        (dst_ip>>16) & 0XFF,
        (dst_ip>>8) & 0xFF,
        (dst_ip>>0) & 0xFF,
        (src_ip>>24) & 0xFF, // src ip
        (src_ip>>16) & 0xFF,
        (src_ip>>8) & 0xFF,
        (src_ip>>0) & 0xFF,
        dst_port, // dst port
        src_port, // src port
        proto // proto
    );
    printf("hash = %u\n",hash(src_ip,dst_ip,src_port,dst_port,proto));
}