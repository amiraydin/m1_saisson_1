#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

// int pthread_creat(
// 	pthread_t	*thread,
// 	pthread_attr_t	*attr,
// 	void	*(*start_routine) (void *)
// 	void	*arg
// 	);


void *thread_1(void *arg){
	printf("Nous sommes dans le thread.\n");
	pthread_exit(NULL);
}
int main(void){
	pthread_t thread1;
	printf("Avant la création du thread.\n");
	if(pthread_create(&thread1, NULL, thread_1, NULL) == - 1) {
	perror("pthread_create");
	return EXIT_FAILURE;
	}
	if (pthread_join(thread1, NULL)) {
	perror("pthread_join");
	return EXIT_FAILURE;
	}
	printf("Après la création du thread.\n");
	return EXIT_SUCCESS;
}
