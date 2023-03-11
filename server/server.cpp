#include <stdio.h>
#include <string.h> //strlen
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>    //close
#include <arpa/inet.h> //close
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros
#include <iostream>
#include "server.h"

#define TRUE 1
#define FALSE 0
#define PORT 8888

void server_start()
{
    int opt = TRUE;
    int master_socket, addrlen, new_sockect, client_socket[30], max_clients = 30, activity, i, valread, sd;

    int max_sd;

    struct sockaddr_in address;

    char buffer[1025];

    // set of socket descriptor
    fd_set readfds;

    // a message
    char *message = "ECHO Daemon v1.o \r\n";

    for (i = 0; i < max_clients; i++)
    {
        client_socket[i] = 0;
    }

    // create master scoket
    if ((master_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    //  set master socket to allow multiple connections,
    // just a good habit

    if (setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0)
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    // type of socket created
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // bind the socket to localhost port 8888
    if (bind(master_socket, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    std::cout << "Listerner on port " << PORT << std::endl;

    if (listen(master_socket, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    // accept the incomming connection
    addrlen = sizeof(address);
    std::cout << "Waiting for connections ..." << std::endl;

    while (TRUE)
    {
        //  clear the socket set
        FD_ZERO(&readfds);

        // add master socket to set
        FD_SET(master_socket, &readfds);
        max_sd = master_socket;

        for (i = 0; i < max_clients; i++)
        {

            // socket descriptor
            sd = client_socket[i];

            // if valid socket descriptor then ad to read list
            if (sd > 0)
                FD_SET(sd, &readfds);

            // highest file descriptor number, need it for the select function
            if (sd > max_sd)
            {
                max_sd = sd;
            }
        }

        // wait for an activity on one of the sockets, timeout is Null
        // so wait indefinately

        activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);

        if ((activity < 0) && (errno != EINTR))
        {
            printf("select error");
        }

        if (FD_ISSET(master_socket, &readfds))
        {
            if ((new_sockect = accept(master_socket, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
            {
                perror("Accept");
                exit(EXIT_FAILURE);
            }

            std::cout << "New_connection, socked fd is " << new_sockect << " ip is : " << inet_ntoa(address.sin_addr) << " port : " << ntohs(address.sin_port) << std::endl;

            if (send(new_sockect, message, strlen(message), 0) != strlen(message))
            {
                perror("send");
            }

            std::cout << "Message sent succssfully" << std::endl;

            for (i = 0; i < max_clients; i++)
            {

                if (client_socket[i] == 0)
                {
                    client_socket[i] = new_sockect;
                    std::cout << "Adding to list of sockets as " << i << std::endl;
                    break;
                }
            }
        }

        // else its some IO operation on some other socket

        for (i = 0; i < max_clients; i++){
            sd = client_socket[i];

            if(FD_ISSET(sd, &readfds))
            {
                // check if it was for closing and also read the \incoming message

                if ((valread = read(sd, buffer, 1024)) == 0)
                {
                    getpeername(sd, (struct sockaddr*)&address, (socklen_t *)&addrlen);

                    printf("Host disconnnected, ip %s, port %d \n", inet_ntoa(address.sin_addr), ntohs(address.sin_port));

                    //  Close the socket and mark as 0 in list for reuse
                    close(sd);
                    client_socket[i] = 0;
                } 
                // Echo back the message that came in
                else
                {
                    // set the sting terminating NULL byte on the end
                    //  of the data read
                    buffer[valread] = '\0';
                    send(sd, buffer, strlen(buffer), 0);
                }
            }
        }
    }
}