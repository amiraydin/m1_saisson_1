#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <ctype.h>
#include <unistd.h>

int portno = 8080; // Faut definir le port sinon la connection est refusé coté client
int sockfd, newsockfd;
int clilen;
int n;
char buffer[256];

struct sockaddr_in serv_addr, cli_addr;

void bzero(void *s, size_t n);

int main(void){

	// Creation socket
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("Erreur lors de la création du socket");
        exit(EXIT_FAILURE);
    }

	bzero((char *)&serv_addr, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(portno);

    // Attachement du socket au port 8080
    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1) {
        perror("Erreur lors de l'attachement du socket au port");
        exit(EXIT_FAILURE);
    }

    // Écoute du serveur sur 2 connexions
    printf("Adresse : %d, Port : %d\n", inet_ntoa(serv_addr.sin_addr), ntohs(serv_addr.sin_port));
    listen(sockfd, 2);

    // Socket de connexion au Client-Server
    clilen = sizeof(cli_addr);
    newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
    if (newsockfd == -1) {
        perror("Erreur lors de l'acceptation de la connexion");
        exit(EXIT_FAILURE);
    }

    // Communication Client-Serveur
    bzero(buffer, 256);
    n = read(newsockfd, buffer, 255);
    printf("Message du client : %s\n", buffer);
    n = write(newsockfd, "J'ai bien reçu votre message", 28);
    if (n == -1) {
        perror("Erreur lors de l'écriture vers le socket");
        exit(EXIT_FAILURE);
    }

    close(sockfd);
    close(newsockfd);

    return 0;
}