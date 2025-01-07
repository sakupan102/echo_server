#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
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
    struct sockaddr_in sock_addr;
    sock_addr.sin_family = AF_INET;
    sock_addr.sin_port = htons(8080);
    sock_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    int bind_num = bind(sock, &sock, sizeof(struct sockaddr_in));
    if (bind_num == -1)
    {
        perror("failed to bind socket");
        exit(1);
    }
    int listen_num = listen(sock, MAX_CLIENT);
    if (listen_num == -1)
    {
        perror("failed to listen socket");
        exit(1);
    }
    while (1)
    {
        printf("hello!\n");
        struct sockaddr_in client_addr;
        socklen_t addrlen = sizeof(struct sockaddr_in);
        int queued_socket = accept(sock, &client_addr, &addrlen);

        if (queued_socket == -1)
        {
            perror("failed to accept request");
            exit(1);
        }
        char echo_buffer[BUF_SIZE];
        ssize_t message_size = read(queued_socket, echo_buffer, BUF_SIZE);
        printf("client address: %s\n", inet_ntoa(client_addr.sin_addr));
        printf("reqest message from client: %s\n", echo_buffer);
        printf("message size: %d\n", message_size);
        if (message_size == -1)
        {
            perror("failed to read buffer");
            exit(1);
        }
        ssize_t write_buf = write(queued_socket, echo_buffer, message_size);
        if (write_buf == -1)
        {
            perror("failed to read buffer");
            exit(1);
        }
        close(queued_socket);
    }
}