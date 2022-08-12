#include "clientBackend.h"

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

// create a client socket and connect to the server
void connectServer() {
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
    
    freeaddrinfo(server_addrinfo_list);
}

// receive players number from the server
struct PlayersInfo convertPlayersInfoMsg (char* playersInfoMsg) {
    // using socket recv to receive "2134"
    // 2 for the player, 1 for the ally and so on
    //char* receivePlayerInfo = recvState();
    struct PlayersInfo playersInfo;
    playersInfo.player = playersInfoMsg[0];
    playersInfo.ally = playersInfoMsg[1];
    playersInfo.enemy1 = playersInfoMsg[2];
    playersInfo.enemy2 = playersInfoMsg[3];
    free(playersInfoMsg); // this is okay? (works in my machine)
    return playersInfo;
}

// send command to the server
void sendAction (char* ret) {
    bytes_sent = send(socket_fd, ret, MAXBUFFERBYTES, 0);

    // send error
    socket_error(bytes_sent, "send()");
}

// receive the game state
char* recvState () {
    char buffer[MAXBUFFERBYTES];
    char *gameState = (char*)malloc(MAXBUFFERBYTES); // needs to be freed in client main!
    bytes_received = recv(socket_fd, buffer, MAXBUFFERBYTES-1, 0);
    
    // recv error
    socket_error(bytes_received, "recv()");
    
    strcpy(gameState, buffer);
    memset(buffer, 0, MAXBUFFERBYTES);
    return gameState;
}

// close socket
void closeSocket() {
    close(socket_fd);
}