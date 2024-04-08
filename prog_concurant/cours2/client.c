#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

int sockfd;
int portno = 8080; // Assure que la valeur du port est la même que celle du serveur
struct sockaddr_in serv_addr;
char buffer[1024];
int n;

int main(void) {
    // Creation d'un socket
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket creation error");
        return -1;
    }

    // Configuration des paramètres du serveur
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(portno);
    // L'adresse IP du serveur (remplace-la par l'adresse réelle du serveur)
    if (inet_pton(AF_INET, "127.0.0.1", &(serv_addr.sin_addr)) <= 0) {
        perror("Invalid address/ Address not supported");
        return -1;
    }

    // Connection au serveur
    if (connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1) {
        perror("Connection failed");
        return -1;
    }
    printf("Adresse : %d, Port : %d\n", inet_ntoa(serv_addr.sin_addr), ntohs(serv_addr.sin_port));
    // Client-Server communication
    printf("Message : ");
    bzero(buffer, 1024);
    fgets(buffer, 1024, stdin);
    n = write(sockfd, buffer, strlen(buffer));

    bzero(buffer, 1024);
    n = read(sockfd, buffer, 1024);
    printf("Réponse du serveur : %s\n", buffer);

    close(sockfd);

    return 0;
}