#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include "client.h"
#include <iostream>

void start_client(){
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if(sockfd < 0) {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    // connect to the server

    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(12346);
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);

    if(connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Error connectingto server");
        exit(EXIT_FAILURE);
    }

    // send and receive data

    const char *msg = "Hello from client";

    int num_bytes = send(sockfd, msg, strlen(msg), 0);

    if (num_bytes < 0){
        perror("Error sending daa");
        exit(EXIT_FAILURE);
    }

    char buffer[1024];
    num_bytes = recv(sockfd, buffer, sizeof(buffer), 0);
    if(num_bytes < 0){
        perror("Error receiving data");
        exit(EXIT_FAILURE);
    }

    std::cout << buffer << std::endl;

    close(sockfd);
}