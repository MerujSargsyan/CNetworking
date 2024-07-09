#include <stdio.h>
#include <string.h>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/un.h>

#include <sys/wait.h>
#include <unistd.h>

int main(void) {
    int sd = socket(PF_LOCAL, SOCK_STREAM, 0);
    printf("sock: %d\n", sd);
    
    struct sockaddr_un addr;
    // makes entire struct 0
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_LOCAL;
    strncpy(addr.sun_path, "./server", sizeof(addr.sun_path) - 1);
    
    int bd = bind(sd, (const struct sockaddr*)&addr, sizeof(addr));

    printf("bind: %d\n", bd);

    pid_t exec = fork();
    if(exec == 0) {
        // TODO: connect to parent
        int sc = connect(sd, (const struct sockaddr*)&addr, sizeof(addr));
        printf("connect: %d\n", sc);
    } else {
        wait(NULL);
    }
    
    // TODO: close socket
    return 0;
}
