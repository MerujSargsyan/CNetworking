#include <stdio.h>
#include <string.h>

#include <sys/socket.h>
#include <arpa/inet.h>

#include <unistd.h>

#define BACKLOG_SZ 5
#define MSG_LEN 50

int main(int argc, char** argv) {
    int client_sd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in client_addr; // NOTE: we do not bind because client binds to server

    // initialize socket
    memset(&client_addr, 0, sizeof(client_addr));
    client_addr.sin_family = AF_INET;
    client_addr.sin_port = htons(8080);
    client_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    // connect to server
    connect(client_sd, (const struct sockaddr*)&server_addr, sizeof(server_addr));

    char buff[] = "Sticking out your GYATT FOR THE RIZZLER!\n";
    write(client_sd, buff, MSG_LEN);

    close(client_sd);
}
