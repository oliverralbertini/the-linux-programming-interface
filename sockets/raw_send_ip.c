/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2018.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* raw_send_ip.c

   Send an IP datagram on a raw socket.
*/
#define _GNU_SOURCE
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <assert.h>
#include "tlpi_hdr.h"

#define IPv4_HEADER_LEN 20      /* Length of IPv4 header (assumes
                                   no options are present) */
#define UDP_HEADER_LEN  8       /* Length of UDP header */

/* Return the IPv4 address for a given host */

static struct sockaddr_in
getIPv4Addr(char *host)
{
    struct addrinfo hints, *res;
    struct sockaddr_in addr;
    int s;

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_canonname = NULL;
    hints.ai_addr = NULL;
    hints.ai_next = NULL;

    s = getaddrinfo(host, NULL, &hints, &res);
    if (s != 0)
        fatal("getaddrinfo() failed: %s", gai_strerror(s));

    addr = *(struct sockaddr_in *) res->ai_addr;
    freeaddrinfo(res);

    return addr;
}

/* Populate the fields of the IP header pointed to by 'iphdr' */

static void
buildIpHeader(struct ip *iphdr, char *protocol, size_t datagramLen,
              char *srcIpStr, struct sockaddr_in dstIpAddr)
{
    int mbz, dontFrag, moreFrags, fragOffset;

    /* 'ip_hl' is a 4-bit value that counts 32-bit words in header */

    iphdr->ip_hl = IPv4_HEADER_LEN / sizeof(uint32_t);

    iphdr->ip_v = 4;            /* IPv4 */
    iphdr->ip_tos = 0;          /* Type of service/DSCP+ECN */

    iphdr->ip_id = htons(0);    /* Identification; unused since
                                   there is no fragmentation */

    /* ip_off: three 1-bit flags + 13-bit fragment offset */

    mbz = 0;                    /* Must be zero */
    dontFrag = 0;               /* Don't fragment */
    moreFrags = 0;              /* More fragments */
    fragOffset = 0;
    iphdr->ip_off = htons((mbz << 15) + (dontFrag << 14) +
                          (moreFrags << 13) + fragOffset);

    iphdr->ip_ttl = 255;        /* 8-bit time-to-live */
    iphdr->ip_p = atoi(protocol);

    iphdr->ip_src = (getIPv4Addr(srcIpStr)).sin_addr;
    iphdr->ip_dst = dstIpAddr.sin_addr;

    /* Note: the kernel automatically fills in the 'ip_len' (total
       length) and 'ip_sum' (checksum) fields for us (see raw(7)) */

}

int
main(int argc, char **argv)
{
    void *ipDatagram;
    struct ip *iphdr;
    int sockfd;
    char dstIpStr[INET_ADDRSTRLEN];
    char *protocol, *dstHost, *srcIpStr, *data;
    struct sockaddr_in dstIpAddr;
    size_t datagramLen;

    if (argc < 5)
        usageErr("%s protocol src-IP dest-host data\n", argv[0]);

    protocol =  argv[1];
    srcIpStr =  argv[2];
    dstHost =   argv[3];
    data =      argv[4];

    ipDatagram = malloc(IP_MAXPACKET);
    if (ipDatagram == NULL)
        errExit("malloc-ipDatagram");

    datagramLen = IPv4_HEADER_LEN + strlen(data);

    /* The IP header sits at the start of the datagram */

    iphdr = ipDatagram;

    dstIpAddr = getIPv4Addr(dstHost);

    if (inet_ntop(AF_INET, &dstIpAddr.sin_addr, dstIpStr,
                INET_ADDRSTRLEN) == NULL)
        errExit("inet_ntop");

    printf("Destination IP: %*s\n", INET_ADDRSTRLEN, dstIpStr);

    /********* IPv4 header **********/

    buildIpHeader(iphdr, protocol, datagramLen, srcIpStr, dstIpAddr);

    /* Copy data into data segment */

    memcpy((char *) iphdr + IPv4_HEADER_LEN, data, strlen(data));

    /* Create the raw socket that we'll use to send the IP datagram */

    sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);
    if (sockfd == -1)
        errExit("socket");

    /* Send the IP datagram */

    /* The datagram already contains the destination IP address.
       However, the kernel must prepare the data-link layer header
       (probably an Ethernet header), and needs to know where to send
       the raw datagram. The kernel determines this information from
       the IP address supplied to the sendto() call. */

    printf("Sending datagram (%zd bytes)\n", datagramLen);

    if (sendto(sockfd, ipDatagram, datagramLen, 0,
                (struct sockaddr *) &dstIpAddr,
                sizeof(struct sockaddr)) == -1)
        errExit("sendto");

    close(sockfd);
    free(ipDatagram);
    exit(EXIT_SUCCESS);
}
