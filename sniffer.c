#include <pcap.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include "myheader.h"

/* This function will be invoked by pcap for each captured packet.
We can process each packet inside the function.
*/
void got_packet(u_char *args, const struct pcap_pkthdr *header, const u_char *packet) {
  printf("Got a packet\n");
  struct ethheader *eth=(struct ethheader *)packet;

  if(ntohs(eth->ether_type) == 0x800)
  {
        struct ipheader *ip = (struct ipheader *)(packet + sizeof(struct ethheader));
        printf("        From: %s\n", inet_ntoa(ip->iph_sourceip));
        printf("        To: %s\n", inet_ntoa(ip->iph_destip));

        switch(ip->iph_protocol) {
                case IPPROTO_TCP:
                {
                        struct tcpheader *tcp = (struct tcpheader*)(packet + sizeof(struct ethheader) + sizeof(struct ipheader));
                        printf("        Protocol: TCP\n");
                        printf("        Source port: %u\n", ntohs(tcp->tcph_srcport));
                        printf("        Destination port: %u\n", ntohs(tcp->tcph_destport));
                }
                        return;
                case IPPROTO_UDP:
                        printf("        Protocol: UDP\n");
                        return;
                case IPPROTO_ICMP:
                        printf("        Protocol: ICMP\n");
                        return;
                default:
                        printf("        Protocol: Others\n");
                        return;
        }

  }
}

int main() {
  pcap_t *handle;
  char errbuf[PCAP_ERRBUF_SIZE];
  struct bpf_program fp;
  char filter_exp[] = "host 10.9.0.5";
  bpf_u_int32 net;
  // Step 1: Open live pcap session on NIC with name eth3.
  // Students need to change "eth3" to the name found on their own
  // machines (using ifconfig). The interface to the 10.9.0.0/24
  // network has a prefix "br-" (if the container setup is used).
  handle = pcap_open_live("br-60011c6696bc", BUFSIZ, 1, 1000, errbuf);
  // Step 2: Compile filter_exp into BPF psuedo-code
  pcap_compile(handle, &fp, filter_exp, 0, net);
  if (pcap_setfilter(handle, &fp) !=0) {
    pcap_perror(handle, "Error:");
    exit(EXIT_FAILURE);
  }
  // Step 3: Capture packets
  pcap_loop(handle, -1, got_packet, NULL);
  pcap_close(handle); //Close the handle
  return 0;
}
