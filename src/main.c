#include <stdio.h>
#include <string.h>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/un.h>

#include <sys/wait.h>
#include <unistd.h>

#define BACKLOG_SZ 5
#define MSG_LEN 1000

int main(void) {
    int sd = socket(AF_INET, SOCK_STREAM, 0);
    printf("sock: %d\n", sd);
    
    struct sockaddr_in addr;
    socklen_t addr_len = sizeof(addr);
    // makes entire struct 0
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8080);
    addr.sin_addr.s_addr = INADDR_ANY;
    
    int bd = bind(sd, (const struct sockaddr*)&addr, sizeof(addr));
    printf("bind: %d\n", bd);

    int ld = listen(sd, BACKLOG_SZ);
    printf("listen: %d\n", ld);

    pid_t exec = fork();
    if(exec == 0) {
        sleep(1);
        // TODO: connect to parent
        int client_sd = socket(AF_INET, SOCK_STREAM, 0);
        printf("client socket: %d\n", client_sd);

        struct sockaddr_in client_addr;

        memset(&client_addr, 0, sizeof(client_addr));
        client_addr.sin_family = AF_INET;
        client_addr.sin_port = htons(8080);
        client_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

        int sc = connect(client_sd, (const struct sockaddr*)&addr, sizeof(addr));
        printf("connect: %d\n", sc);

        char buff[] = "Hello Server!\n";
        write(client_sd, buff, strnlen(buff, MSG_LEN));

        close(client_sd);
    } else {
        int ad = accept(sd, (struct sockaddr*)&addr, &addr_len);
        printf("accept: %d\n", ad);

        char buff[MSG_LEN];
        read(ad, buff, MSG_LEN);
        printf("message: %s\n", buff);
        wait(NULL);
        close(ad);
    }
    
    close(sd);
    return 0;
}
