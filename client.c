#include "client.h"


void help() {
	printf("\033[0;36m!help Command\n");
	printf("!exit : ferme le chat\n");
	printf("!online : affiche les clients en ligne\n");
	printf("!ping : affiche le ping entre le serveur et vous\n\033[0m");
}

int init_connexion(int port, char* pseudo) {
	int socket_client, connfd; 
	struct sockaddr_in servaddr, cli;
	
	// creation socket
	socket_client = socket(AF_INET, SOCK_STREAM, 0); 
	if (socket_client == -1) { 
		printf("socket creation failed...\n"); 
		exit(0); 
	} 
	else
		printf("Socket successfully created..\n"); 
	bzero(&servaddr, sizeof(servaddr)); 

	// ip/port serveur
	servaddr.sin_family = AF_INET; 
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); 
	servaddr.sin_port = htons(port); 

	// connexion au serveur
	if (connect(socket_client, (SA*)&servaddr, sizeof(servaddr)) != 0) { 
		printf("connection with the server failed...\n"); 
		exit(0); 
	} else {
		write(socket_client, pseudo, sizeof(pseudo));
		printf("connected to the server..\n"); 
	}

	return socket_client;
}

void chat(int sockfd, char* pseudo) 
{ 
	char buff[MAX]; 
	int n, act;
	fd_set readfds;

	for (;;) { 
		FD_ZERO(&readfds);
		FD_SET(sockfd, &readfds);
		FD_SET(STDIN_FILENO, &readfds);

		act = select(sockfd+1, &readfds, NULL, NULL, NULL);
		if ((act < 0) && (errno!=EINTR)) {   
            printf("select error");   
        }

		if (FD_ISSET(STDIN_FILENO, &readfds)) {
			bzero(buff, sizeof(buff)); 
			n = 0; 
			while ((buff[n++] = getchar()) != '\n');
			if(strncmp(buff,"!exit", strlen(buff)-1) == 0) {
				return;
			} else if (strncmp(buff,"!help", strlen(buff)-1) == 0) {
				help();
			} else if (strncmp(buff,"!online", strlen(buff)-1) == 0) {
				write(sockfd, buff, sizeof(buff)); 
				read(sockfd, buff, sizeof(buff));
				printf("%s\n", buff);
			} else {
				write(sockfd, buff, sizeof(buff)); 
				bzero(buff, sizeof(buff)); 
			}
		} else if (FD_ISSET(sockfd, &readfds)) {
			if(read(sockfd, buff, sizeof(buff)) == 0) {
				printf("\033[1;33mServer disconnected ! (connexion lost)\033[0m\n");
				exit(1);
			} else {
				printf("%s", buff);
			}
		}
	} 
} 

int main(int argc, char *argv[]) { 
	int socket_client;

	//verification du nombre d'arguments
	if(argc != 3) {
		printf("Usage : ./client [pseudo] [port]\n");
		return -1;
	}

	char* pseudo = argv[1];
	int port = atoi(argv[2]);

	//verification du port
	if(port < 1024 || port > 49151) {
		printf("Erreur : le port doit être entre 1024 et 49151\n");
		return -1;
	}
	
	//creation du socket + connexion au serveur
	socket_client = init_connexion(port, pseudo);

	//function for chat 
	chat(socket_client, pseudo); 

	//fermeture du socket
	close(socket_client); 
}