#include <iostream>
#include <server.h>
#include <client.h>
#include <thread>

int main(int arg, char const *argv[]){

    std::thread server_thread(server_start);
    std::thread client_thread(start_client);


    server_thread.join();
    client_thread.join();
    return 0;
}