#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024

void error(const char *message) {
    perror(message);
    exit(1);
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Usage: %s <server_ip> <server_port> <file_name>\n", argv[0]);
        exit(1);
    }

    const char *server_ip = argv[1];
    int server_port = atoi(argv[2]);
    const char *file_name = argv[3];

    
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
        error("Failed to create socket");

    
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_port);
    if (inet_pton(AF_INET, server_ip, &server_addr.sin_addr) <= 0)
        error("Invalid server IP address");

    
    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
        error("Failed to connect to the server");

    
    FILE *file = fopen(file_name, "rb");
    if (file == NULL)
        error("Failed to open file");

   
    char buffer[BUFFER_SIZE];
    size_t bytesRead;
    while ((bytesRead = fread(buffer, 1, BUFFER_SIZE, file)) > 0) {
        if (send(sock, buffer, bytesRead, 0) < 0)
            error("Failed to send file");
    }

   
    fclose(file);

   
    close(sock);

    printf("File transfer complete.\n");

    return 0;
}
