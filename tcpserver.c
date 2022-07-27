#include <stdio.h>
#include <errno.h> // Errors 
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


#include <sys/types.h>
#include <sys/socket.h> 
#include <netdb.h> // getaddrinfo, gai_strerror


#define SERVER_PORT "6969"

void exit_if_error(int status, char* error_message) {
    if (status == -1) {
        perror(error_message);
        exit(1);
    }

}

bool is_socket_error(int status, char* error_message) {
    if (status == -1) {
        perror(error_message);
        return true;
    }
    return false;
}

struct addrinfo init_server_hints() {
    struct addrinfo server_hints;
    memset(&server_hints, 0, sizeof server_hints);
    server_hints.ai_family = AF_INET;
    server_hints.ai_socktype = SOCK_STREAM;
    server_hints.ai_flags = AI_PASSIVE; // pass NULL to nodename
    return server_hints;
}

struct addrinfo * init_server_addr_list() {
    struct addrinfo hints = init_server_hints();
    struct addrinfo * server_addr_list;

    int getaddrinfo_status = getaddrinfo(NULL, SERVER_PORT, &hints, &server_addr_list);
    exit_if_error(getaddrinfo_status, "getaddrinfo()");

    return server_addr_list;
}


int main() {

    // server_addr_list 
    struct addrinfo * server_addr_list = init_server_addr_list();

    // socket 
    int socketfd = socket(PF_INET, SOCK_STREAM, 0); 
    exit_if_error(socketfd, "socket()");

    // bind socket and server_addr
    for(struct addrinfo * server_addr = server_addr_list; server_addr != NULL; server_addr = server_addr->ai_next) {
        int bind_status = bind(socketfd, server_addr->ai_addr, server_addr->ai_addrlen);
        if (is_socket_error(bind_status, "bind()")) {
            continue;
        }
        break; // if we get here, we must have connected successfully
    }

    // listen()
    int listen_status = listen(socketfd, 5);
    exit_if_error(listen_status, "listen()");

    // accept 4 client connects - TODO 
    // init 4 processes to handle 4 clients 
    socklen_t client0_addr_size, client1_addr_size, client2_addr_size, client3_addr_size;
    int client0_socket, client1_socket, client2_socket, client3_socket; 

    // while number_of_connections != 4
        // accept() 

    printf("seems to work\n");
}
