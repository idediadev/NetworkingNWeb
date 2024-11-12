#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define BUF_SIZE 1024

int main(int argc, char *argv[]) {
    int sd, n;
    struct sockaddr_in servaddr;
    char nome_file[BUF_SIZE];
    char buffer[BUF_SIZE];
    char esito;

    if (argc != 3) {
        fprintf(stderr, "Uso: %s <indirizzo_server> <porta>\n", argv[0]);
        exit(1);
    }

    while (1) {
        printf("Nome del file da ricevere: ");
        fgets(nome_file, BUF_SIZE, stdin);
        nome_file[strcspn(nome_file, "\n")] = 0;

        sd = socket(AF_INET, SOCK_STREAM, 0);
        if (sd < 0) {
            perror("Errore apertura socket");
            exit(1);
        }

        memset(&servaddr, 0, sizeof(servaddr));
        servaddr.sin_family = AF_INET;
        servaddr.sin_port = htons(atoi(argv[2]));
        inet_pton(AF_INET, argv[1], &servaddr.sin_addr);

        connect(sd, (struct sockaddr*)&servaddr, sizeof(servaddr));
        write(sd, nome_file, strlen(nome_file) + 1);

        read(sd, &esito, 1);
        if (esito == 'S') {
            while ((n = read(sd, buffer, BUF_SIZE)) > 0) {
                write(1, buffer, n); // Output su console
            }
        } else {
            printf("File inesistente\n");
        }
        close(sd);
    }
    return 0;
}
