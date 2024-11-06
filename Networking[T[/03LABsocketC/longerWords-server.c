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

// f: longestWod 
int longestWord(const char *filename)
{
    FILE *file = fopen(filename, "r");                                              // foper file in reading mode
    if (!file) return -1;                                                           // file !E so return error -1

    int max_lenght = 0, current_length = 0;
    char ch;
    while ((ch = fgetc(file)) != EOF)                                               // reading of the chars in the file till the EOF
    {
        if (ch == '' || ch == '\n')                                                 // with a Space char or a next line char do 
        {
            if (current_length > max_lenght) max_lenght = current_length;           // update the MAx lenght of the Longest Word
            current_length = 0; // reset
        }else{
            current_length++; 
        }
    }
    fclose(file);
    return max_lenght > current_length ? max_lenght : current_length;               // return Longest Word
}
// Server ITself
int main (int argc, char *argv[])
{
    int server_fd, new_socket;                                                      // file descriptors for the Server and the new socket
    struct sockaddr_inaddress;                                                      // structure for the Server address 
    int addrlen = sizeof(address);                                                  // Address's structure dimension 
    char buffer[BUFFER_SIZE] = {0};                                                 // buffer send-recive stream 

    // Socket 
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)== 0))
    {
        perror("Error: Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // address assigment to socket's port 
    address.sin_family = AF_INET; 
    address.sin_addr.s_addr = INADDR_ANY; 
    address.sin_port = htcons(PORT);
    
    // bind.socket 
    if(bind(server_fd, (struct sockaddr *)&address, sizeof(address)<0))             // socket's binding to the port address 
    {
        perror("Error: Bind Failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

// Client comunication 

    // waiting Client 
    if(listen(server_fd, 3)<0)
    {
        perror("Error: Listen Failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    
    // Terminal Interface 
    printf("The Server is listening on port %d\n", PORT);

    while (1)
    {
        // allow new connectuion
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)addrlen))<0)
        {
            perror("Error: Accept failed");
            continue;
        }
        // recive nameFile from the Client
        int valread = read(new_socket, buffer, BUFFER_SIZE);
        if (valread > 0 )
        {
            buffer[valread]= '\0'; // aggiunge il terminatore di stringa
            int LongestW = longestWord(buffer);
            snprintf(buffer, BUFFER_SIZE, "%d", LongestW); // convert lenght in string
            sen(new_socket, buffer, strlen(buffer), 0); // send lenght to client
            printf("Sent the longest word lenght %s \n", buffer);  // log message
        }
        close(new_socket);
    }
    close(server_fd);
    return 0;


}
