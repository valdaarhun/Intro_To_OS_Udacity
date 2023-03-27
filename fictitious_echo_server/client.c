#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/ip.h>

int main(){
    struct sockaddr_in addr = {
        .sin_family = AF_INET,
        .sin_port = htons(31337),
        .sin_addr = {
            .s_addr = htonl(INADDR_LOOPBACK)
        }
    };

    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    connect(sock_fd, (struct sockaddr *)&addr, sizeof(struct sockaddr_in));

    char buf[256];
    fgets(buf, 256, stdin);

    send(sock_fd, buf, strlen(buf), 0);
    memset(buf, 0, 256);
    recv(sock_fd, buf, 256, 0);
    printf("%s", buf);

    shutdown(sock_fd, SHUT_RDWR);
    close(sock_fd);
}