#ifndef NTK_FUNC_H
#define NTK_FUNC_H

#include <stdalign.h>
#include <rte_byteorder.h>
#include <rte_stdatomic.h>
#include <rte_mbuf_core.h>
#include <rte_ether.h>
#include <rte_ip.h>
#include <rte_tcp.h>
#include <stdint.h>
#include <stdio.h>

typedef struct ntk_ipv4_tuples
{
    uint32_t src_ip;
    uint32_t dst_ip;
    uint16_t src_port;
    uint16_t dst_port;
    uint8_t proto;
} ntk_ipv4_tuples;

ntk_ipv4_tuples ntk_ipv4_gen(struct rte_mbuf *m);

#endif
