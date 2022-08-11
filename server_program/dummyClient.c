#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#define SERVER_PORT "6954" //Server port number

int socket_fd;
typedef struct addrinfo* addrinfo_list;
//char buffer[MAXBUFFERBYTES];
addrinfo_list server_addrinfo_list;
ssize_t bytes_sent;
int bytes_received;

// return boolean when error
bool socket_error(int status, char* error_message) {
    if (status == -1) {
        perror(error_message);
        return true;
    }
    return false;
}

// exit when error
void exit_socket_error(int status, char* error_message) {
    if (status == -1) {
        perror(error_message);
        exit(1);
    }
}

// set to IPv4, TCP, flags=AI_PASSIVE, respectively
struct addrinfo init_server_hints() {
    struct addrinfo server_hints;
    memset(&server_hints, 0, sizeof server_hints);
    server_hints.ai_family = AF_INET;
    server_hints.ai_socktype = SOCK_STREAM;
    server_hints.ai_flags = AI_PASSIVE;
    return server_hints;
}

int main(){
    struct addrinfo server_hints = init_server_hints();

    int getaddrinfo_status = getaddrinfo(NULL, SERVER_PORT, &server_hints, &server_addrinfo_list);
    exit_socket_error(getaddrinfo_status, "getaddrinfo()");
    
    // Setup Socket to Server Application
    for (struct addrinfo* server_addrinfo = server_addrinfo_list; server_addrinfo != NULL; server_addrinfo = server_addrinfo->ai_next) {
        socket_fd = socket(server_addrinfo->ai_family, server_addrinfo->ai_socktype, server_addrinfo->ai_protocol);
        if (socket_error(socket_fd, "socket()")) {
            continue;
        }
        int connect_status = connect(socket_fd, server_addrinfo->ai_addr, server_addrinfo->ai_addrlen);
        if (socket_error(connect_status, "connect()")) {
            continue;
        }
        break;
    }

    char msg[2048];
    memset(msg, 0, 2048);
    int bytes = recv(socket_fd, msg, 2048, 0);
    if(bytes > 0){
        printf("%s", msg);
    }

    char buff[2048];
    while(true){
        memset(buff, 0, 2048);
        bytes = recv(socket_fd, buff, 2048, 0);
        if(strcmp(buff, "AllPlayersHere") == 0){
            break;
        }
        else if(bytes > 0){
            printf("%s", buff);
        }
    }

    while(true){
        memset(buff, 0, 2048);
        printf("enter a message:\n");
        scanf("%s", buff);
        if(strcmp(buff, "exit")==0){
            break;
        }
        send(socket_fd, buff, 2048, 0);
    }

    while(true){
        memset(buff, 0, 2048);
        bytes = recv(socket_fd, buff, 2048, 0);
        printf("%s", buff);
    }
    
    freeaddrinfo(server_addrinfo_list);
}