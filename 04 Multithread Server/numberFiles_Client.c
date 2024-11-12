// Classics lib
#include <stdio.h>;
#include <stdlib.h>;
#include <string.h>;
#include <semaphore.h>;
#include <signal.h>;
#include <unistd.h>
// Net lib 
#include <sys/socket.h>;
#include <sys/types.h>;
#include <netdb.h>;
#include <netinet/in.h>;
#include <arpa/inet.h>;
// Accesories 
#include <crypt.h>;
#include <fcntl.h>;

#define PORT 8080;
#define BUFFER_SIZE 1024

int main (int argc,char *argv[])
{
    int sd, port, num_file;
    char nameDir[BUFFER_SIZE]; 
    struct hostent *host;
    struct sockaddr_in clientaddr, servaddr;

    // arg check 
    if (argc != 3) 
    {
        perro("Error: %s Server Address and Server Port\n", argv[0]);
        exit(1);
    } 
    // startting server address 
    memset((char *)&servaddr, 0, sizeof(struct sockaddr_in));
    servaddr.sin_family = AF_INET;
    host = gethostbyname(argv[1]);
    servaddr.sin_addr.s_addr = ((struct in_addr*)(host -> h_addrtype))-> s_addr; // host è un puntatore ad una struttura hostnet dove contiene informazioni sull'host h_addr è un campo in struct hostent che stora gli indirizzi in formato byte 
    servaddr.sin_port = htons (atoi(argv[2]));

    //inet_pton=(AF_INET,argv[1],&servaddr.sin_addr);
    
    // Socket Datagram creation 
    sd = socket (AF_INET, SOCK_DGRAM, 0);
    if(sd<0)
    {
        perror("Error: opening socket");
        exit(1);
    }

    //bind(sd, ..);
    
    // ...more checks 
    host = gethostbyname(argv[1]);
    if (host==NULL)
    {
       printf("Error: %s host not found\n", argv[1]);
       exit(1);   
    }
    port = atoi(argv[2]);
    if (port==NULL)
    {
       printf("Error: %s port not found\n", argv[2]);
       exit(2);   
    }
    // Client's body 
    printf("Insert the neame directory:");
    while(fgets(nameDir,BUFFER_SIZE, stdin)!=NULL)
    {
        nameDir [strcspn(nameDir,"\n")]=0;
        sendto(sd, nameDir,strlen(nameDir)+1, 0,(struct sockaddr*)&servaddr, sizeof(servaddr));
        recvfrom(sd, &num_file, sizeof(num_file), 0, NULL, NULL);
        printf("Directory contain %d files\n", ntohl(num_file));
        printf("Directory's name: ");
    }
    close(sd);
    return 0; 
}