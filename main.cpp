#include <iostream>
#include <server.h>
#include <client.h>

int main(int arg, char const *argv[]){
    server_start();
    start_client();

    return 0;
}