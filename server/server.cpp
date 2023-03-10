#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include "server.h"
#include <iostream> 
#define PORT 8080


void server_start(){
    // creating a socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    std::cout << "We are up" << std::endl;
    if(sockfd < 0) {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    //  binding a socket
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(12345);
    addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("Error binding socket");
        exit(EXIT_FAILURE);
    }
}

