#include <netdb.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#define MAX 80 
#define SA struct sockaddr

void help();
int init_connexion(int port, char* pseudo);
void chat(int sockfd, char* pseudo);