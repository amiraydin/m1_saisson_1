#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

void *fsomme(void *arg);
void *fproduit(void *arg);
int main(int argc, char *argv[])
{
	int i;
	pthread_t fils1, fils2;
	char *temp;
	temp = (char *)malloc(sizeof(int) + sizeof(char));
	sprintf(temp, "%d ", 10);
	if (pthread_create(&fils1, NULL, fsomme, (void *)temp))
	{
		perror("pthread_create somme");
	}
	if (pthread_create(&fils2, NULL, fproduit, (void *)temp))
	{
		perror("pthread_create produit ");
	}
	printf("Sortie du main \n");
	pthread_exit(0);
}
void *fsomme(void *arg)
{
	int i, somme = 0;
	int n = atoi((char *)arg);
	for (i = 0; i <= n; i++)
		somme = somme + i;
	printf("Somme = % d\n", somme);
	pthread_exit(0);
}
void *fproduit(void *arg)
{
	int i, produit = 1;
	int n = atoi((har *)arg);
	for (i = 1; i <= n; i++)
		produit = produit * i;
	printf("Produit = %d\n", produit);
	pthread_exit(0);
}
