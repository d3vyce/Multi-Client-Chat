#ifdef WIN32

#include <winsock2.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#elif defined (linux)

#include <netdb.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#endif

#define MAX 1024 //1kbit buffer 
#define SA struct sockaddr

void help();
int init_connexion(char* ip, int port, char* pseudo);
void chat(int sockfd, char* pseudo);