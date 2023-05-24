#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define DNS_SERVER "8.8.8.8"  
#define DNS_PORT 53           

struct DNSHeader {
    unsigned short id;
    unsigned short flags;
    unsigned short qdcount;
    unsigned short ancount;
    unsigned short nscount;
    unsigned short arcount;
};

struct DNSQuestion {
    unsigned short qtype;
    unsigned short qclass;
};

struct DNSResponse {
    unsigned short name;
    unsigned short type;
    unsigned short dnsclass;
    unsigned int ttl;
    unsigned short rdlength;
    unsigned char rdata[4];
};

void create_dns_query_packet(char *hostname, unsigned char *buf) {
    struct DNSHeader *dns = (struct DNSHeader *)buf;
    struct DNSQuestion *qinfo = (struct DNSQuestion *)(buf + sizeof(struct DNSHeader));
    
    dns->id = (unsigned short)htons(getpid());
    dns->flags = htons(0x0100);  
    dns->qdcount = htons(1);
    dns->ancount = 0;
    dns->nscount = 0;
    dns->arcount = 0;
    
    qinfo->qtype = htons(1);  
    qinfo->qclass = htons(1); 
    
    char *token;
    int index = 0;
    token = strtok(hostname, ".");
    
    while (token != NULL) {
        buf[index++] = strlen(token);
        memcpy(&buf[index], token, strlen(token));
        index += strlen(token);
        token = strtok(NULL, ".");
    }
    
    buf[index++] = 0; 
    buf[index++] = 0x01; 
    buf[index++] = 0x00; 
}

int main() {
    char hostname[100];
    printf("Enter the hostname to resolve: ");
    scanf("%s", hostname);    
    int sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sockfd < 0) {
        perror("Socket creation failed");
        return 1;
    }  
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(DNS_PORT);
    if (inet_pton(AF_INET, DNS_SERVER, &(server_addr.sin_addr)) <= 0) {
        perror("Invalid DNS server address");
        return 1;
    }
    unsigned char buf[65536];
    create_dns_query_packet(hostname, buf);
    
    if (sendto(sockfd, buf, sizeof(struct DNSHeader) + sizeof(struct DNSQuestion) + strlen(hostname) + 2, 0,
               (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("DNS query failed");
        return 1;
    }
    unsigned int server_addr_len = sizeof(server_addr
