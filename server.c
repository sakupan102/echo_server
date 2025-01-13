#include <sys/types.h>
#include <sys/socket.h>
#include <sys/event.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <errno.h>

#define MAX_CLIENT 10000
#define BUF_SIZE 32

int main(int argc, char *argv[])
{
    // ToDo ctrl + cで割り込みが生じたときの処理

    int sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == -1)
    {
        perror("failed to create socket");
        exit(1);
    }
    struct sockaddr_in sock_addr = {0};
    sock_addr.sin_family = AF_INET;
    sock_addr.sin_port = htons(8080);
    sock_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    if (bind(sock, &sock_addr, sizeof(struct sockaddr_in)) == -1)
    {
        perror("failed to bind socket");
        exit(1);
    }
    if (listen(sock, MAX_CLIENT) == -1)
    {
        perror("failed to listen socket");
        exit(1);
    }
    int on = 1;
    ioctl(sock, FIONBIO, &on);
    printf("echo server listen on port: 8080\n");
    int kq = kqueue();
    if (kq == -1)
    {
        perror("failed to create kqueue");
        exit(1);
    }
    struct kevent event = {0};
    EV_SET(&event, sock, EVFILT_READ, EV_ADD, 0, 0, NULL);
    int ret = kevent(kq, &event, 1, NULL, 0, NULL);
    if (ret == -1)
    {
        perror("failed to register kevent");
        exit(1);
    }
    int num_clients = 0;

    while (1)
    {
        /* 読み取り可能なソケットがあるか調べる*/
        struct kevent tevent = {0};
        int num_events = kevent(kq, NULL, 0, &tevent, 1, NULL);
        if (num_events == -1)
        {
            perror("failed to read buffer");
            exit(1);
        }
        if (num_events == 0)
        {
        }
        else if ((tevent.flags & 0xf000) == EV_EOF) // ソケットが切断された場合はclose
        {
            int fd = tevent.ident;
            EV_SET(&event, fd, EVFILT_READ, EV_DELETE, 0, 0, NULL);
            int ret = kevent(kq, &event, 1, NULL, 0, NULL);
            if (ret == -1)
            {
                perror("failed to register kevent");
                exit(1);
            }
            close(fd);

            printf("connection closed\n");
            --num_clients;
        }
        else
        {
            if (tevent.ident == sock)
            {
                // 受け入れ可能であれば新しい接続の受け入れを行う
                if (num_clients == MAX_CLIENT)
                {
                    continue;
                }
                struct kevent event = {0};
                struct sockaddr_in client_addr = {0};
                socklen_t addrlen = sizeof(struct sockaddr_in);
                int fd = accept(sock, &client_addr, &addrlen);
                ++num_clients;
                if (fd == -1)
                {
                    if (errno == EAGAIN | errno == EWOULDBLOCK)
                    {
                        continue;
                    }
                    else
                    {

                        perror("failed to accept request");
                        exit(1);
                    }
                }

                printf("client address: %s\n", inet_ntoa(client_addr.sin_addr));
                EV_SET(&event, fd, EVFILT_READ, EV_ADD, 0, 0, NULL);
                int ret = kevent(kq, &event, 1, NULL, 0, NULL);
                if (ret == -1)
                {
                    perror("failed to register kevent");
                    exit(1);
                }
            }
            else
            {
                // メッセージ読み取りの開始

                int fd = tevent.ident;
                char echo_buffer[BUF_SIZE];
                ssize_t message_size = read(fd, echo_buffer, BUF_SIZE);
                if (message_size == -1)
                {
                    perror("failed to read buffer");
                    exit(1);
                }
                echo_buffer[message_size] = '\0';
                printf("message size: %d\n\n", message_size);
                ssize_t write_buf = write(fd, echo_buffer, message_size);
                if (write_buf == -1)
                {
                    perror("failed to read buffer");
                    exit(1);
                }
            }
        }
    }
}