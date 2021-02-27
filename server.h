#include <stdio.h> 
#include <netdb.h> 
#include <netinet/in.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#define MAX 1024
#define MAX_CLIENT 10
#define PORT 8081
#define SA struct sockaddr

typedef struct {
    int socket;
    char pseudo[50];
}Client;

int send_message(Client tab[MAX_CLIENT], char buff[MAX], int socket, int id);
int send_online(Client tab[MAX_CLIENT], int socket);