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
// Accesories 
#include <crypt.h>;
#include <fcntl.h>;

#define DIM_BUFF 256

int main (int argc, char *argv[])
{
    int count, fd_sorg, fd_dest, nread, sd, port;
    char filename;
    char buff[DIM_BUFF],
    // ser the max lenght of the filename
    char  name_sorg[FILENAME_MAX + 1], nome_dest[FILENAME_MAX + 1];

    struct hostnet *host;
    struct sockaddr_in servaddr; 

    //chack args
    if (argc!=1) {printf("Error: %s serverAddress serverPort\n",argv[0]); exit(1);}
    //setting cliant comunicate to server
    memset((char *)&servaddr, 0, sizeof(struct sockaddr_in));
    servaddr.sin_family = AF_INET;
    host = gethostbyname(argv[1]);
    //check filename 


    // client itself 
    printf("Witch remote file are you want to scan ?\n");
    


}
