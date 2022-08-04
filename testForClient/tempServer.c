// this is for client-clientbackend example to test client works
// So, please note that the server logic here might be wrong
// please note the comment in line 90

#include <stdio.h>
#include <errno.h> 
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h> 
#include <netdb.h>

#define SERVER_PORT "6969"
#define MAXBUFFERLENGTH 100

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
    server_hints.ai_flags = AI_PASSIVE;
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
    // socekt create and set-up for server
    // socekt create and set-up for server
    // socekt create and set-up for server
    struct addrinfo * server_addr_list = init_server_addr_list();
    int socketfd = socket(PF_INET, SOCK_STREAM, 0); 
    exit_if_error(socketfd, "socket()");
    int newSK;
    int str_len = 0;
    char message[MAXBUFFERLENGTH];
    for(struct addrinfo * server_addr = server_addr_list; server_addr != NULL; server_addr = server_addr->ai_next) {
        int bind_status = bind(socketfd, server_addr->ai_addr, server_addr->ai_addrlen);
        if (is_socket_error(bind_status, "bind()")) {
            continue;
        }
        break;
    }
    int listen_status = listen(socketfd, 5);
    exit_if_error(listen_status, "listen()");
    struct sockaddr_in clt_adr;
    socklen_t clt_sz;
    clt_sz = sizeof(clt_adr);
    // socekt create and set-up for server ends
    // socekt create and set-up for server ends
    // socekt create and set-up for server ends

    // sending: player, ally, enemy1, enemy2
    char* playersInfo = "2134";
    if((newSK = accept(socketfd, (struct sockaddr*)&clt_adr, &clt_sz)) == -1) {
        printf("accept err\n");
    }
    else {
        send(newSK, playersInfo, 4,0);
    }
    

    // receive and send message
    // print received message
    // send received message to client
    while(1) {
        //if((newSK = accept(socketfd, (struct sockaddr*)&clt_adr, &clt_sz)) == -1) {printf("accept err\n");}
        while((str_len = recv(newSK, message, MAXBUFFERLENGTH,0)) != 0){
            // recv message printed twice
            // one from the client
            // the other with empty
            // so print only the message is not empty
            if(strlen(message) != 0) {
                printf("In server = %s", message);
                send(newSK, message, MAXBUFFERLENGTH-1,0);
            }
            memset(message, 0, MAXBUFFERLENGTH);
        }
   }
    close(newSK);
    return 0;
}