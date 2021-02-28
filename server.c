#include "server.h"

int send_message(Client tab[MAX_CLIENT], char buff[MAX], int socket, int id) {
	char message[MAX];
	int i;

	bzero(message, sizeof(message)); 

	if (id != -1) {
		strncpy(message, tab[id].pseudo, MAX - 1);
		strncat(message, " : ", sizeof message - strlen(message) - 1);
	}

	strncat(message, buff, sizeof message - strlen(message) - 1);
	
	for(i=0; i < MAX_CLIENT; i++) {
		if(tab[i].socket != 0 && tab[i].socket != socket) write(tab[i].socket, message, sizeof(message));
	}
}

int send_online(Client tab[MAX_CLIENT], int socket) {
	char message[MAX];
	int i;

	bzero(message, sizeof(message)); 

	strncpy(message, "Online client(s) : ", MAX - 1);

	for(i=0; i < MAX_CLIENT; i++) {
		if(tab[i].socket != 0) { 
			strncat(message, tab[i].pseudo, sizeof tab[i].pseudo - strlen(message) - 1);
			strncat(message, " ", sizeof message - strlen(message) - 1);
		}
	}

	write(socket, message, sizeof(message));
}

int main() 
{ 
	int sockfd, connfd, len; 
	struct sockaddr_in servaddr, cli;
	Client tab[MAX_CLIENT]; 

	char buff[MAX];
	
	int n = 0, i, j, test, val, act, max_sock, temp_socket;
	fd_set readfds;

	for(i=0; i < MAX_CLIENT; i++) {
		tab[i].socket = 0;
	}

	// socket create and verification 
	sockfd = socket(AF_INET, SOCK_STREAM, 0); 
	if (sockfd == -1) { 
		printf("socket creation failed...\n"); 
		exit(0); 
	} 
	else
		printf("Socket successfully created..\n"); 
	bzero(&servaddr, sizeof(servaddr)); 

	// assign IP, PORT 
	servaddr.sin_family = AF_INET; 
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
	servaddr.sin_port = htons(PORT); 

	// Binding newly created socket to given IP and verification 
	if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) { 
		printf("socket bind failed...\n"); 
		exit(0); 
	} 
	else
		printf("Socket successfully binded..\n"); 

	// Now server is ready to listen and verification 
	if ((listen(sockfd, 5)) != 0) { 
		printf("Listen failed...\n"); 
		exit(0); 
	} 
	else
		printf("Server listening..\n"); 
	len = sizeof(cli); 

	max_sock = sockfd;

	// Accept the data packet from client and verification 
	for (;;) { 	 
		FD_ZERO(&readfds);
		FD_SET(sockfd, &readfds);
		bzero(buff, sizeof(buff));
		
		for(i=0; i < MAX_CLIENT; i++) {
			test = tab[i].socket;
			if(test > 0) {
				FD_SET(test, &readfds);
			}
			if(test > sockfd) {
				max_sock = test;
			}
		}

		act = select(max_sock+1, &readfds, NULL, NULL, NULL);
		if ((act < 0) && (errno!=EINTR)) {   
            printf("select error");   
        }
		
		if (FD_ISSET(sockfd, &readfds) != 0) {
			if ((temp_socket = accept(sockfd, (SA*)&cli, &len)) < 0) { 
				printf("server acccept failed...\n"); 
				exit(0); 
			} else {
				for(i=0; i < MAX_CLIENT; i++) {
					if(tab[i].socket == 0) {
						tab[n].socket = temp_socket;
						read(tab[n].socket, tab[n].pseudo, sizeof(tab[n].pseudo)); 
						printf("\033[1;33m%s is connected !\033[0m\n", tab[n].pseudo);

						strncpy(buff, "\033[1;33m", MAX - 1);
						strncat(buff, tab[i].pseudo, sizeof tab[i].pseudo - strlen(buff) - 1);
						strncat(buff, " is connected !\033[0m\n", sizeof tab[i].pseudo - strlen(buff) - 1);
						send_message(tab, buff, tab[i].socket, -1);

						n++;
						break;
					}
				}
			}
		}
		
		for(i=0; i < MAX_CLIENT; i++) {
			test = tab[i].socket;
			if (FD_ISSET(test, &readfds) != 0) {
				if((val = read(test, buff, sizeof(buff))) == 0) { //Client déco
					
					printf("\033[1;33m%s is disconnected !\033[0m\n", tab[i].pseudo);

					strncpy(buff, "\033[1;33m", MAX - 1);
					strncat(buff, tab[i].pseudo, sizeof tab[i].pseudo - strlen(buff) - 1);
					strncat(buff, " is disconnected !\033[0m\n", sizeof tab[i].pseudo - strlen(buff) - 1);
					send_message(tab, buff, tab[i].socket, -1);
					
					close(test);
					tab[i].socket = 0;
					n--;
					if(n == 0) {
						return 0;
					}
				} else {
					if(strncmp(buff,"!online", strlen(buff)-1) == 0) {
						printf("%s: %s", tab[i].pseudo, buff);
						send_online(tab, test);
					} else {
						printf("%s: %s", tab[i].pseudo, buff);
						send_message(tab, buff, tab[i].socket, i);
					}
				}
			}
		}
	}

	//After chatting close the socket 
	close(sockfd); 
}