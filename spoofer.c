#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#include "myheader2.h"

#define SRC_IP "8.8.8.8"
#define DEST_IP "10.9.0.5"

unsigned short in_cksum(unsigned short *buf,int length);
void send_raw_ip_packet(struct ipheader* ip);

int main() {
   char buffer[PACKET_LEN];
   memset(buffer, 0, PACKET_LEN);

   struct icmpheader *icmp = (struct icmpheader *) (buffer + sizeof(struct ipheader));
   icmp->icmp_type = 8;

   icmp->icmp_chksum = 0;
   icmp->icmp_chksum = in_cksum((unsigned short *)icmp,sizeof(struct icmpheader));


   struct ipheader *ip = (struct ipheader *) buffer;
   ip->iph_ver = 4;
   ip->iph_ihl = 5;
   ip->iph_tos = 16;
   ip->iph_ident = htons(54321);
   ip->iph_ttl = 64;
   ip->iph_sourceip.s_addr = inet_addr(SRC_IP);
   ip->iph_destip.s_addr = inet_addr(DEST_IP);
   ip->iph_protocol = IPPROTO_ICMP;
   ip->iph_len = htons(sizeof(struct ipheader) + sizeof(struct icmpheader));

   send_raw_ip_packet(ip);
   return 0;
}

void send_raw_ip_packet(struct ipheader* ip) {
  struct sockaddr_in dest_info;
  struct sockaddr_in src_info;
  int enable = 1;

  int sock = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);
  setsockopt(sock, IPPROTO_IP, IP_HDRINCL, &enable, sizeof(enable));

  dest_info.sin_family = AF_INET;
  dest_info.sin_addr = ip->iph_destip;
  src_info.sin_family = AF_INET;
  src_info.sin_addr = ip->iph_sourceip;

  printf("Sending spoofed IP packet...\n");
  sendto(sock, ip, ntohs(ip->iph_len), 0, (struct sockaddr *)&dest_info, sizeof(dest_info));
  if(sendto(sock,ip,ntohs(ip->iph_len), 0, (struct sockaddr *)&dest_info, sizeof(dest_info)) < 0)
  {
    perror("PACKET NOT SENT\n");
    return;
  }
  else {
    printf("\n----------------------------------------------------------\n");
    printf("    From: %s\n", inet_ntoa(ip->iph_sourceip));
    printf("    To: %s\n", inet_ntoa(ip->iph_destip));
    printf("\n----------------------------------------------------------\n");
  }
  close(sock);
}

unsigned short in_cksum(unsigned short *buf, int length) {
        unsigned short *w = buf;
        int nleft = length;
        int sum = 0;
        unsigned short temp = 0;

        while(nleft > 1) {
                sum += *w++;
                nleft -= 2;
        }

        if(nleft == 1) {
                *(u_char *)(&temp) = *(u_char *)w;
                sum += temp;
        }

        sum = (sum >> 16) + (sum & 0xffff);
        sum += (sum >> 16);
        return (unsigned short)(~sum);
}
