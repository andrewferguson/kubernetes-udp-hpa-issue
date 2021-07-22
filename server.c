// Server side implementation of UDP client-server model
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
  
#define MAXLINE 1024
  
// Driver code
int main(int argc, char const *argv[]) {
    if(argc != 3) {
        printf("RUN: ./server <IP_ADDRESS> <PORT>\n");
        return 1;
    }

    char * ip_address = (char *) argv[1];
    int PORT = atoi(argv[2]);

    int sockfd;
    char buffer[MAXLINE];
    char hello[MAXLINE];
    sprintf(hello, "Hello from %s", ip_address);
    struct sockaddr_in servaddr, cliaddr;
      
    // Creating socket file descriptor
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }
      
    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));
      
    // Filling server information
    servaddr.sin_family    = AF_INET; // IPv4
    servaddr.sin_addr.s_addr = inet_addr(ip_address);
    servaddr.sin_port = htons(PORT);
      
    // Bind the socket with the server address
    if ( bind(sockfd, (const struct sockaddr *)&servaddr, 
            sizeof(servaddr)) < 0 )
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
      

    while (1) {
    int len, n;
  
    len = sizeof(cliaddr);  //len is value/resuslt
  
    n = recvfrom(sockfd, (char *)buffer, MAXLINE, 
                MSG_WAITALL, ( struct sockaddr *) &cliaddr,
                &len);
    buffer[n] = '\0';
    printf("Client : %s\n", buffer);
    long long unsigned int x = rand();
    for (int i = 0; i <= 10000; i++) {
        x *= rand();
    }
    sendto(sockfd, (const char *)hello, strlen(hello), 
        MSG_CONFIRM, (const struct sockaddr *) &cliaddr,
            len);
    printf("Hello message sent.\n"); 
    }
      
    return 0;
}
