#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <errno.h>

#define MAX_CLIENT 10
#define BUF_SIZE 32

int main(int argc, char *argv[])
{
    int socket_num = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (socket_num == -1)
    {
        perror("failed to create socket");
        exit(1);
    }

    struct sockaddr_in sock;
    memset(&sock, 0, sizeof(struct sockaddr_in));
    sock.sin_family = AF_INET;
    sock.sin_port = htons(8080);
    sock.sin_addr.s_addr = inet_addr("127.0.0.1");
    int connected = connect(socket_num, &sock, sizeof(struct sockaddr_in));
    if (connected == -1)
    {
        perror("failed to connect server");
        exit(1);
    }
    char *message = argv[1];
    int wrote = write(socket_num, message, strlen(message));
    if (wrote == -1)
    {
        perror("failed send message");
        exit(1);
    }
    char response_buf[strlen(message)];
    int success = read(socket_num, response_buf, strlen(message));
    if (success == -1)
    {
        perror("failed send message");
        exit(1);
    }
    printf("response from server: %s", response_buf);
    success = close(socket_num);
    if (success == -1)
    {
        perror("failed to close socket");
        exit(1);
    }
    return 0;
}