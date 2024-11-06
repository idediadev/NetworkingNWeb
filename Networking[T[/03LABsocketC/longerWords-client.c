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

#define PORT 8080;
#define BUFFER_SIZE 1024

int main ()
{
    int sock = 0;                                                           // file descriptor for the client's socket
    struct sockaddr_in serv_addr;                                           // struct for the Server's address
    char buffer[BUFFER_SIZE];                                               // buffer setting Stream
    char filename[BUFFER_SIZE];                                             // buffer stream for fileName  

    // Socket client side
    if ((sock = socket(AF_INET, SOCK_STREAM, 0))<0)
    {
        printf("\nError : Socket creation");
        return -1;
    }
    
    serv_addr.sin_family= AF_INET;
    serv_addr.sin_addr= htons(PORT);

    // IP to bin
    if (inet_pton(AF_INET,"198.154.32.32", &serv_addr.sin_addr)<=0)     //FAKE IP
    {
        printf("\nError : Invalid address IP");
        return -1;
    }
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr))<0)
    {
        printf("\nError: Connection failed");
        return -1;
    }

    while(1)
    {
        
        printf("Which file are you want to scan ? (type 'exit' to quit): "); // ask the file name to scan; Terminal interface 
        scanf("%s", filename);

        if (strcmp(filename, "exit") == 0 ) break;                           // user write exit, so end it 

        send(sock, filename, strlen(filename), 0);

        
        int valread = read(sock, buffer, BUFFER_SIZE);                      // recive the Longest Word 
        if (valread > 0)
        {
            buffer[valread] = '\0';
            printf("The Longest word lenght : %s \n", buffer);
        }
    }
    close(sock);
    return 0;
}

   
    



