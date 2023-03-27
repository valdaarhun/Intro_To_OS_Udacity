#include <ctype.h>
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
    const int reuse_addr = 1;
    setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, (void *)&reuse_addr, sizeof(reuse_addr));
    bind(sock_fd, (struct sockaddr *)&addr, sizeof(struct sockaddr_in));

    listen(sock_fd, 5);
    int peer_fd = accept(sock_fd, NULL, NULL);

    char buf[256];
    recv(peer_fd, buf, 256, 0);
    for(int i = 0 ; buf[i] != '\0' && buf[i] != '\n' ; i++){
        buf[i] = toupper(buf[i]);
    }
    send(peer_fd, buf, strlen(buf), 0);

    shutdown(peer_fd, SHUT_RDWR);
    close(peer_fd);
    close(sock_fd);
}