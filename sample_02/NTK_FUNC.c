#include "NTK_FUNC.h"

ntk_ipv4_tuples ntk_ipv4_gen(struct rte_mbuf *m)
{
    struct rte_ether_hdr *eth_hdr;
    struct rte_ipv4_hdr *ipv4_hdr;
    struct rte_tcp_hdr *tcp_hdr;
    struct ntk_ipv4_tuples *ipv4_tup;
    void *ipv4_src;

    eth_hdr = rte_pktmbuf_mtod(m, struct rte_ether_hdr *);
    ipv4_src = (uint8_t *)eth_hdr + sizeof(struct rte_ether_hdr);

    if(eth_hdr->ether_type == rte_cpu_to_be_16(RTE_ETHER_TYPE_IPV4))
    {
        ipv4_hdr = (struct rte_ipv4_hdr *)ipv4_src;
        tcp_hdr = (struct tcp_hdr *)((unsigned char *) ipv4_hdr + sizeof(struct rte_ipv4_hdr));

        if(rte_ipv4_hdr_len(ipv4_hdr) == sizeof(struct rte_ipv4_hdr))
        {
            ipv4_tup->ip_src = ipv4_hdr->src_addr;
            ipv4_tup->ip_dst = ipv4_hdr->dst_addr;
            ipv4_tup->port_src = tcp_hdr->src_port;
            ipv4_tup->port_dst = tcp_hdr->dst_port;
            ipv4_tup->proto = ipv4_hdr->next_proto_id;

            return ipv4_tup;
        }
    }
}