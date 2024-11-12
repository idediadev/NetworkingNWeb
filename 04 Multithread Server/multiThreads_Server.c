#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <dirent.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <sys/select.h>

#define BUF_SIZE 1024

int conta_file(char *name) {
    DIR *dir = opendir(name);
    int count = 0;
    if (dir) {
        while (readdir(dir)) count++;
        closedir(dir);
    }
    return count;
}

int main(int argc, char *argv[]) {
    int udpfd, listenfd, connfd, maxfd, nready;
    struct sockaddr_in servaddr, cliaddr;
    fd_set rset;

    if (argc != 2) {
        fprintf(stderr, "Uso: %s <porta>\n", argv[0]);
        exit(1);
    }

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    udpfd = socket(AF_INET, SOCK_DGRAM, 0);

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(atoi(argv[1]));

    bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
    bind(udpfd, (struct sockaddr*)&servaddr, sizeof(servaddr));

    listen(listenfd, 5);

    FD_ZERO(&rset);
    maxfd = (listenfd > udpfd) ? listenfd : udpfd;

    for (;;) {
        FD_SET(listenfd, &rset);
        FD_SET(udpfd, &rset);

        nready = select(maxfd + 1, &rset, NULL, NULL, NULL);
        
        if (FD_ISSET(udpfd, &rset)) {
            socklen_t len = sizeof(cliaddr);
            char nome_dir[BUF_SIZE];
            recvfrom(udpfd, nome_dir, BUF_SIZE, 0, (struct sockaddr*)&cliaddr, &len);
            int num_file = htonl(conta_file(nome_dir));
            sendto(udpfd, &num_file, sizeof(num_file), 0, (struct sockaddr*)&cliaddr, len);
        }

        if (FD_ISSET(listenfd, &rset)) {
            socklen_t len = sizeof(cliaddr);
            connfd = accept(listenfd, (struct sockaddr*)&cliaddr, &len);

            if (fork() == 0) {
                close(listenfd);
                char nome_file[BUF_SIZE];
                read(connfd, nome_file, BUF_SIZE);
                
                int fd_file = open(nome_file, O_RDONLY);
                if (fd_file < 0) {
                    write(connfd, "N", 1);
                } else {
                    write(connfd, "S", 1);
                    int n;
                    char buffer[BUF_SIZE];
                    while ((n = read(fd_file, buffer, BUF_SIZE)) > 0) {
                        write(connfd, buffer, n);
                    }
                    close(fd_file);
                }
                close(connfd);
                exit(0);
            }
            close(connfd);
        }
    }
    return 0;
}
