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
    if(sockfd < 0) {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    //  binding a socket
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(1234);
    addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("Error binding socket");
        exit(EXIT_FAILURE);
    }

    // listen to incoming connection
    if(listen(sockfd, 5) < 0){
        perror("Error listening for connection");
        exit(EXIT_FAILURE);
    }

    // 
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    int clientfd = accept(sockfd, (struct sockaddr *)&client_addr, &client_len);

    if(clientfd < 0){
        perror("Error accepting connection");
        exit(EXIT_FAILURE);
    }

    // sending and receiving data
    char buffer[1024];
    int num_bytes = recv(clientfd, buffer, sizeof(buffer), 0);
    if(num_bytes < 0){
        perror("Error receiving data");
        exit(EXIT_FAILURE);
    }

    std::cout << "Received message: " << buffer << std::endl;

    const char  *msg = "Hello from server";
    num_bytes = send(clientfd, msg, strlen(msg), 0);

    if (num_bytes < 0){
        perror("Error sending data");
        exit(EXIT_FAILURE);
    }

    close(clientfd);
    close(sockfd);


}

