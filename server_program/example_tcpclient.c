#include <stdbool.h> // socket_error
#include <string.h> // memset 
#include <stdio.h> // fprintf
#include <stdlib.h> // exit 
#include <unistd.h> // close file descriptor 
#include <time.h> // rtt

#include <sys/types.h> // getaddrinfo, connect
#include <sys/socket.h> // getaddrinfo, connect
#include <netdb.h> // getaddrinfo


#define SERVER_PORT "4200"
#define MAXBUFFERBYTES 100 
#define MAXBUFFERLENGTH 99 // to manually place end of line at 100

typedef struct addrinfo* addrinfo_list;


bool socket_error(int status, char* error_message) {
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

double diff_timespec(const struct timespec *time1, const struct timespec *time0) {
    return (time1->tv_sec - time0->tv_sec) + (time1->tv_nsec - time0->tv_nsec) / 1000000000.0;
}


int main() {

    // Setup Server Application Address 
    addrinfo_list server_addrinfo_list; // linked list of addrinfo

    struct addrinfo server_hints = init_server_hints();

    int getaddrinfo_status = getaddrinfo(NULL, SERVER_PORT, &server_hints, &server_addrinfo_list);
    if (getaddrinfo_status != 0) {
        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(getaddrinfo_status));
        exit(1);
    }

    // Setup Socket to Server Application
    int socket_fd; 

    for (struct addrinfo* server_addrinfo = server_addrinfo_list; server_addrinfo != NULL; server_addrinfo = server_addrinfo->ai_next) {
        // Setup Socket
        socket_fd = socket(server_addrinfo->ai_family, server_addrinfo->ai_socktype, server_addrinfo->ai_protocol);
        if (socket_error(socket_fd, "socket()")) {
            continue;
        }
        // Connect to Server
        int connect_status = connect(socket_fd, server_addrinfo->ai_addr, server_addrinfo->ai_addrlen);
        if (socket_error(connect_status, "connect()")) {
            continue;
        }
        break;
    }

    // Send Initialization
    char message[] = "Hello, world!";

    // Receive Initialization 
	char buffer[MAXBUFFERBYTES];

    // rtt
    struct timespec ts1; // rtt
    struct timespec ts2;  // rtt

    clock_gettime(CLOCK_REALTIME, &ts1);  // rtt

    ssize_t number_of_bytes_sent = send(socket_fd, message, strlen(message), 0);
    int number_of_bytes_received = recv(socket_fd, buffer, MAXBUFFERLENGTH, 0);
    
    clock_gettime(CLOCK_REALTIME, &ts2);  // rtt
    printf("rtt: %f \n", diff_timespec(&ts2, &ts1)); // rtt

    // send error
    if (! socket_error(number_of_bytes_sent, "send()")) {
        printf("number_of_bytes_sent: %zd \n", number_of_bytes_sent);
    }

    // recv error
    if (socket_error(number_of_bytes_received, "recv()")) {
        exit(1);
    }

    buffer[number_of_bytes_received] = '\0'; // Escape Sequence or Null
	printf("client: received '%s' \n", buffer);

    // Clean up
    freeaddrinfo(server_addrinfo_list);
    close(socket_fd);
}
