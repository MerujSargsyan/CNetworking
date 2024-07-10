#include <stdio.h>
#include <string.h>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/un.h>

#include <sys/wait.h>
#include <unistd.h>

#define BACKLOG_SZ 5
#define MSG_LEN 50 

int main(void) {
    int server_socket = socket(AF_INET, SOCK_STREAM, 0); // create a socket for the server
    
    struct sockaddr_in server_addr;
    socklen_t addr_len = sizeof(server_addr);
    /* Initializes socket */
    // makes entire struct 0
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = INADDR_ANY;
    
    // binds the server_socket to the adress struct
    int bd = bind(server_socket, (const struct sockaddr*)&server_addr, sizeof(server_addr));

    int ld = listen(server_socket, BACKLOG_SZ);

    pid_t exec = fork();
    if(exec == 0) {
        sleep(1);

        // the client needs their own socket to connect
        int client_sd = socket(AF_INET, SOCK_STREAM, 0);

        struct sockaddr_in client_addr; // NOTE: we do not bind because client binds to server

        // initialize socket
        memset(&client_addr, 0, sizeof(client_addr));
        client_addr.sin_family = AF_INET;
        client_addr.sin_port = htons(8080);
        client_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

        // connect to server
        int sc = connect(client_sd, (const struct sockaddr*)&server_addr, sizeof(server_addr));

        char buff[] = "Hello Server!\n";
        write(client_sd, buff, strnlen(buff, MSG_LEN));

        close(client_sd);
    } else {
        int ad = accept(server_socket, (struct sockaddr*)&server_addr, &addr_len);

        char buff[MSG_LEN];
        read(ad, buff, MSG_LEN);
        printf("message: %s\n", buff);
        wait(NULL);
        close(ad);
    }
    
    close(server_socket);
    return 0;
}
