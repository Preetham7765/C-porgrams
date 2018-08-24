#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
struct buffer {

	int val;
	sem_t full, empty;	

};

	
void * push_data(void *arg) {

	struct buffer *buff = (struct buffer*)arg;
	//printf("buff val in push%d", buff->val);
	int i;
	for( i= 0; i<10;i++){

		sem_wait(&(buff->full));
		//buff->val = i;
		printf("pushing data %d\n", i);
		buff->val = i;
		sem_post(&(buff->empty));	
	}
	
}


void * read_data(void *arg) {

	struct buffer *buff = (struct buffer*)arg;
	int i;
	for( i=0; i<10;i++){

		sem_wait(&(buff->empty));
		printf("reading data %d\n", buff->val);
		sem_post(&(buff->full));	
	}
	
}

int main() {
	
	//buff = (struct buffer *)malloc(sizeof(struct buffer *));
	pthread_t producer, consumer;
	//buff->val = 10;
	struct buffer buff ;
	sem_init(&(buff.full), 1, 1);
	sem_init(&(buff.empty), 1, 0);
	pthread_create(&producer, NULL, push_data, &buff);
	pthread_create(&consumer, NULL, read_data, &buff);
	

	pthread_join(producer, NULL);
	pthread_join(consumer, NULL);

	//free(buff);		


	return 0;
}
