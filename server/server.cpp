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
}

