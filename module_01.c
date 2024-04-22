/* IPv4 */
struct rte_ether_hdr *eth_hdr;
struct rte_ipv4_hdr *ipv4_hdr;
void *ipv4_src;
/* TCP */
struct rte_tcp_hdr *tcp_hdr;

/* IPv4 & TCP Information Generator */
eth_hdr = rte_pktmbuf_mtod(m, struct rte_ether_hdr *);
ipv4_src = (uint8_t *)eth_hdr + sizeof(struct rte_ether_hdr);

if (eth_hdr->ether_type == rte_cpu_to_be_16(RTE_ETHER_TYPE_IPV4)) {
    ipv4_hdr = (struct rte_ipv4_hdr *)ipv4_src;
    tcp_hdr = (struct tcp_hdr *)((unsigned char *) ipv4_hdr + sizeof(struct rte_ipv4_hdr));
    if(rte_ipv4_hdr_len(ipv4_hdr) == sizeof(struct rte_ipv4_hdr)){
        uint32_t dst_ip = ipv4_hdr->dst_addr;
        uint32_t src_ip = ipv4_hdr->src_addr;
        uint8_t proto = ipv4_hdr->next_proto_id;
        uint16_t dst_port = tcp_hdr->dst_port;
        uint16_t src_port = tcp_hdr->src_port;

        printf("IP dst = %d.%d.%d.%d, IP src = %d.%d.%d.%d, port dst = %u, port src = %u, proto = %u\n",
                (dst_ip>>0) & 0XFF, // dst ip
                (dst_ip>>8) & 0XFF,
                (dst_ip>>16) & 0xFF,
                (dst_ip>>24) & 0xFF,
                (src_ip>>0) & 0xFF, // src ip
                (src_ip>>8) & 0xFF,
                (src_ip>>16) & 0xFF,
                (src_ip>>24) & 0xFF,
                REV_ENDIAN(dst_port), // dst port
                REV_ENDIAN(src_port), // src port
                proto // proto
            );
        }
    }
}

