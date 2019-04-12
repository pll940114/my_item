#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int main(int argc, char *argv[])
{
    int serv_sock, clnt_sock;

    struct sockaddr_in serv_addr, clnt_addr;
    socklen_t clnt_addr_size;

    if (argc != 2) {
        printf("Usage: %s <port>\n", argv[0]);
        exit(1);
    }

    serv_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (serv_sock < 0) {
        printf("sock() error\n");
        exit(1);
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    int optval = 1;
    // setting address and port reusable
    setsockopt(serv_sock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

    if (bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("bind() error\n");
        exit(1);
    }

    if (listen(serv_sock, 5) < 0) {
        printf("listen() error\n");
        exit(1);
    }

    clnt_addr_size = sizeof(clnt_addr);
    clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr,
               &clnt_addr_size);

    if (clnt_sock < 0) {
        printf("accept() error\n");
        exit(1);
    }

    char buf[1024];
    printf("connected with ip: %s and port: %d\n",
           inet_ntop(AF_INET, &clnt_addr.sin_addr, buf, 1024),
           ntohs(clnt_addr.sin_port));

    while (1) {
        memset(buf, 0, sizeof(buf));
        ssize_t size = read(clnt_sock, buf, sizeof(buf) - 1);
        if (size < 0) {
            printf("read() error\n");
            break;
        } else {
            printf("client#: %s\n", buf);
        }
    }
    close(clnt_sock);
    close(serv_sock);
    return 0;
}