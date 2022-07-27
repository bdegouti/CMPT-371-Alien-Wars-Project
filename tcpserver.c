#include <stdio.h>
#include <errno.h> // Errors 
#include <stdlib.h>

#include <sys/types.h>
#include <sys/socket.h> 


struct addrinfo * init_myaddr() {
    struct sockaddr_in myv4addr;
    
    myv4addr.sin_family = AF_INET;
    myv4addr.sin_port = htons(6969);
    inet_pton(AF_INET, AI_PASSIVE, myv4addr.sin_addr);


    getaddrinfo(AI_PASSIVE)

    struct addrinfo hints;
    memset(&hints, 0, sizeof hints);
    hints.ai_family = 
}

int main() {

    int socketfd = socket(PF_INET, SOCK_STREAM, 0); 
    if (socketfd == -1) {
        perror("socket");
        exit(1);
    }

    struct addrinfo * myaddr = init_myaddr(); 
    bind(socketfd, sockaddr, sockaddrlen);

    printf("seems to work\n");

}
