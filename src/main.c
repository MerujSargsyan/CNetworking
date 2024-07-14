#include <stdio.h>
#include <string.h>

#include <sys/socket.h>
#include <arpa/inet.h>

#include <sys/wait.h>
#include <unistd.h>

#define BACKLOG_SZ 5
#define MSG_LEN 50
#define PORT 8080

int main(void) {
    int server_socket = socket(AF_INET, SOCK_STREAM, 0); // create a socket for the server
    
    struct sockaddr_in server_addr;
    socklen_t addr_len = sizeof(server_addr);

    /* Initializes socket */
    // makes entire struct 0
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // binds the server_socket to the adress struct
    bind(server_socket, (const struct sockaddr*)&server_addr, sizeof(server_addr));

    listen(server_socket, BACKLOG_SZ);

    sstruct sockaddr_in client_addr;
    socklen_t addr_len = sizeof(client_addr);
    int client_socket = accept(addr_len, (struct sockaddr*)&client_addr, &client_addr);

    // TODO: hangle clients
    
    close(server_socket);
    return 0;
}
