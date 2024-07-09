#include <stdio.h>
#include <string.h>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/un.h>

#include <sys/wait.h>
#include <unistd.h>

#define BACKLOG_SZ 5

int main(void) {
    int sd = socket(AF_INET, SOCK_STREAM, 0);
    printf("sock: %d\n", sd);
    
    struct sockaddr_in addr;
    // makes entire struct 0
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8080);
    addr.sin_addr.s_addr = INADDR_ANY;
    
    int bd = bind(sd, (const struct sockaddr*)&addr, sizeof(addr));
    printf("bind: %d\n", bd);

    int ld = listen(sd, BACKLOG_SZ);
    printf("listen: %d\n", ld);

    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);

    int ad = accept(sd, (struct sockaddr*)&client_addr, &client_addr_len);
    printf("accept: %d\n", ad);

    pid_t exec = fork();
    if(exec == 0) {
        // TODO: connect to parent
        int client_sd = socket(AF_INET, SOCK_STREAM, 0);
        struct sockaddr_in new_client_addr;
        memset(&new_client_addr, 0, sizeof(new_client_addr));
        new_client_addr.sin_family = AF_INET;
        new_client_addr.sin_port = htons(8080);
        // loopback allows local connection
        new_client_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

        int sc = connect(client_sd, (const struct sockaddr*)&client_addr, client_addr_len);
        printf("connect: %d\n", sc);
    } else {
        wait(NULL);
    }
    
    // TODO: close socket
    return 0;
}
