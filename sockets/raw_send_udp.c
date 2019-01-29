/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2018.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* raw_send_udp.c

   Send a UDP datgram over an IPv4 raw socket.
*/

#define _GNU_SOURCE
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <assert.h>
#include "tlpi_hdr.h"

#define IPv4_HEADER_LEN 20      /* Length of IPv4 header (assumes
                                   no options are present) */
#define UDP_HEADER_LEN  8       /* Length of UDP header */

/* Compute the Internet Checksum, as per RFC 1071 */

static uint16_t
checksum(uint16_t *addr, int count)
{
    uint32_t sum = 0;

    /* Sum up 2-byte values until none or only one byte left */

    while (count > 1) {
        sum += *addr++;
        count -= 2;
    }

    /* Add left-over byte, if any */

    if (count > 0)
        sum += *(uint8_t *) addr;

    /* Fold 32-bit sum to 16 bits */

    while (sum >> 16)
        sum = (sum & 0xffff) + (sum >> 16);

    /* Checksum is ones' complement of sum */

    return (uint16_t) ~sum;
}

/* Calculate the IPv4 UDP checksum. The checksum is calculated
   based on a "pseudoheader" as well as the content of the UDP
   datagram itself. As described in RFC 768, the pseudoheader
   consists mainly of fields from the IP header:

          0      7 8     15 16    23 24    31
         +--------+--------+--------+--------+
         |          source address           |
         +--------+--------+--------+--------+
         |        destination address        |
         +--------+--------+--------+--------+
         |  zero  |protocol|   UDP length    |
         +--------+--------+--------+--------+
*/

static uint16_t
udpChecksum(struct ip *iphdr, struct udphdr *udphdr)
{
    char buf[IP_MAXPACKET];
    int checksumLen;            /* Number of bytes to checksum */
    struct udphdr *udpDgram;

    const int UDP_PSEUDOHEADER_SIZE = 12;
    struct pseudoheader {
        struct in_addr  srcAddress;
        struct in_addr  dstAddress;
        uint8_t         zero;
        uint8_t         protocol;
        uint16_t        udpLength;
    };
    struct pseudoheader *phdr;

    /* Sanity check the size of our pseudoheader structure */

    assert(sizeof(struct pseudoheader) == UDP_PSEUDOHEADER_SIZE);

    phdr = (struct pseudoheader *) buf;
    udpDgram = (struct udphdr *) (buf + sizeof(struct pseudoheader));

    /* Populate the pseudoheader */

    phdr->srcAddress = iphdr->ip_src;
    phdr->dstAddress = iphdr->ip_dst;
    phdr->zero = 0;
    phdr->protocol = iphdr->ip_p;
    phdr->udpLength = udphdr->len;

    /* Copy UDP datagram into buffer */

    memcpy(udpDgram, udphdr, udphdr->len);

    /* The checksum calculation that follows will include
       the UDP checksum field itself. For the purposes of
       that calculation, this field must be zero. */

    udpDgram->check = 0;

    checksumLen = sizeof(struct pseudoheader) + ntohs(udphdr->len);

    /* Zero fill to 16-bit multiple, as required by RFC 768 */

    if (checksumLen % 2) {
        buf[checksumLen] = 0;
        checksumLen++;
    }

    printf("Buffer length for UDP checksum = %d\n", checksumLen);

    return checksum((uint16_t *) buf, checksumLen);
}

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
buildIpHeader(struct ip *iphdr, size_t datagramLen,
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
    iphdr->ip_p = IPPROTO_UDP;  /* Transport-layer protocol (UDP) */

    iphdr->ip_src = (getIPv4Addr(srcIpStr)).sin_addr;
    iphdr->ip_dst = dstIpAddr.sin_addr;

    /* Note: the kernel automatically fills in the 'ip_len' (total
       length) and 'ip_sum' (checksum) fields for us (see raw(7)) */

}

/* Populate the header fields and data of the UDP datagram at the
   location pointed to by 'udphdr' */

static void
buildUdpDatagram(struct udphdr *udphdr, struct ip *iphdr,
                 char *srcPort, char *dstPort, char *data)
{
    uint16_t udpDataLen;
    uint8_t *udpData;

    /* Place data into UDP datagram, immediately after UDP header */

    udpData = (uint8_t *) udphdr + UDP_HEADER_LEN;
    udpDataLen = strlen(data);
    memcpy(udpData, data, udpDataLen);

    udphdr->source = htons(atoi(srcPort));      /* Source port */
    udphdr->dest = htons(atoi(dstPort));        /* Destination port */

    udphdr->len = htons(UDP_HEADER_LEN + udpDataLen);
                                        /* UDP datagram length */
    printf("UDP header length = %d\n", ntohs(udphdr->len));

    /* Calculate UDP checksum */

    udphdr->check = udpChecksum(iphdr, udphdr);
}

int
main(int argc, char **argv)
{
    void *ipDatagram;
    struct ip *iphdr;
    struct udphdr *udphdr;
    int sockfd;
    char dstIpStr[INET_ADDRSTRLEN];
    char *dstHost, *srcIpStr, *dstPort, *srcPort, *data;
    struct sockaddr_in dstIpAddr;
    size_t datagramLen;

    if (argc < 6)
        usageErr("%s src-IP dest-host src-port "
                "dest-port data\n", argv[0]);

    srcIpStr =  argv[1];
    dstHost =   argv[2];
    srcPort =   argv[3];
    dstPort =   argv[4];
    data =      argv[5];

    ipDatagram = malloc(IP_MAXPACKET);
    if (ipDatagram == NULL)
        errExit("malloc-ipDatagram");

    datagramLen = IPv4_HEADER_LEN + UDP_HEADER_LEN + strlen(data);

    /* The IP header sits at the start of the datagram */

    iphdr = ipDatagram;

    /* The UDP datagram, starting with the header, sits
       immediately after the IP header */

    udphdr = (struct udphdr *) ((char *) ipDatagram + IPv4_HEADER_LEN);

    dstIpAddr = getIPv4Addr(dstHost);

    if (inet_ntop(AF_INET, &dstIpAddr.sin_addr, dstIpStr,
                INET_ADDRSTRLEN) == NULL)
        errExit("inet_ntop");

    printf("Destination IP: %*s\n", INET_ADDRSTRLEN, dstIpStr);

    /********* IPv4 header **********/

    buildIpHeader(iphdr, datagramLen, srcIpStr, dstIpAddr);

    /********* UDP datagram **********/

    buildUdpDatagram(udphdr, iphdr, srcPort, dstPort, data);

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
