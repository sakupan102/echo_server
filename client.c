#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <errno.h>

#define MAX_CLIENT 10
#define BUF_SIZE 32

int main(int argc, char *argv[])
{
    int sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == -1)
    {
        perror("failed to create socket");
        exit(1);
    }
    char *hostname = argv[2];
    struct addrinfo hints, *info;
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    getaddrinfo(hostname, NULL, &hints, &info);
    struct sockaddr_in sock_addr;
    sock_addr.sin_family = AF_INET;
    sock_addr.sin_port = htons(8080);
    sock_addr.sin_addr.s_addr = ((struct sockaddr_in *)info->ai_addr)->sin_addr.s_addr;
    int connected = connect(sock, &sock_addr, sizeof(struct sockaddr_in));
    if (connected == -1)
    {
        perror("failed to connect server");
        exit(1);
    }
    char *message = argv[1];
    int wrote = write(sock, message, strlen(message));
    if (wrote == -1)
    {
        perror("failed send message");
        exit(1);
    }
    char response_buf[strlen(message)];
    int success = read(sock, response_buf, strlen(message));
    if (success == -1)
    {
        perror("failed send message");
        exit(1);
    }
    printf("response from server: %s", response_buf);
    success = close(sock);
    if (success == -1)
    {
        perror("failed to close socket");
        exit(1);
    }
    return 0;
}