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
    char *hostname = argv[1];
    struct addrinfo hints, *info;
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    getaddrinfo(hostname, NULL, &hints, &info);
    struct sockaddr_in sock_addr;
    sock_addr.sin_family = AF_INET;
    sock_addr.sin_port = htons(8080);
    sock_addr.sin_addr.s_addr = ((struct sockaddr_in *)info->ai_addr)->sin_addr.s_addr;
    if (connect(sock, &sock_addr, sizeof(struct sockaddr_in)))
    {
        perror("failed to connect server");
        exit(1);
    }
    while (1)
    {
        char message_buf[BUF_SIZE];
        printf("input message: ");
        fgets(message_buf, BUF_SIZE, stdin);
        if (write(sock, message_buf, strlen(message_buf)) == -1)
        {
            perror("failed to send message");
            exit(1);
        }
        char response_buf[BUF_SIZE];
        ssize_t message_size = read(sock, response_buf, BUF_SIZE);
        if (message_size == -1)
        {
            perror("failed to send message");
            exit(1);
        }
        response_buf[message_size] = '\0';
        printf("response from server: %s\n", response_buf);
    }
    cose(sock);
    return 0;
}