#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>

#define SIZE  10
#define NUM_THREADS 4 
struct buffer {

    int index;
	int array[SIZE];
	sem_t full, empty;	

};

pthread_mutex_t lock;
sem_t full;
sem_t empty;
struct buffer *buff;

int isfull(struct buffer *buff);
int isempty(struct buffer *buff);
void * consumer(void *args);
void * producer(void *args);

int isfull(struct buffer *buff) {

    if(buff->index == SIZE) {
    //printf("returning 1");        
        return 1;
    }
    //printf("returning 0\n");
    return 0;
}


int isempty(struct buffer *buff) {

    if(buff->index < 0) {
        return 1;
    }

    return 0;
}

void writedata(int i) {

    buff->array[buff->index] = i;
    buff->index ++;
    
}


int readdata() {

    int data ;
    buff->index --;
    data = buff->array[buff->index];
    

    return data;
     
}

void * producer(void *args) {


    int  tid = *(int *) args;
    int i;
    //printf("inside producer threadid %d\n", tid);        
    for(i = 0; i<SIZE; i++){
        pthread_mutex_lock(&lock);
        while(isfull(buff)){
            pthread_mutex_unlock(&lock);
            //printf("producer hung here %d\n", tid);                                
            sem_wait(&full);
        pthread_mutex_lock(&lock);
        }
        //pthread_mutex_lock(&lock);
        printf("wrote data %d by thread %d\n", i, tid);        
        writedata(i);
                        
        sem_post(&empty);
        pthread_mutex_unlock(&lock);
        
    }

    pthread_exit(NULL);
}


void * consumer(void *args) {

    int tid = *(int *) args;
    int i;
    int data;
    //printf("inside consumer threadid %d\n", tid);    
    for(i = 0; i<SIZE; i++){

        pthread_mutex_lock(&lock);
        do{        
            pthread_mutex_unlock(&lock);
           // printf("consumer hung here %d \n", tid);
            
            sem_wait(&empty);
            pthread_mutex_lock(&lock);                    
        }while(isempty(buff));
        data = readdata();
        sem_post(&full);
        pthread_mutex_unlock(&lock);        
        

        printf("read data %d, by thread %d\n", data, tid);
    }

    pthread_exit(NULL);
}


int main() {

    
    pthread_t threads[NUM_THREADS];
    pthread_mutex_init(&lock, NULL);
    sem_init(&full, 0, SIZE);
    sem_init(&empty, 0, 0);
    int threadids[NUM_THREADS];
    int i;

    buff = (struct buffer *)malloc(sizeof(struct buffer));
    buff->index = 0;

    for(i=0;i<NUM_THREADS;){
        //printf("%d\n", i);
        threadids[i] = i;
        pthread_create(&threads[i], NULL, &producer, &threadids[i]);
        i++;
        
        threadids[i] = i;
        pthread_create(&threads[i], NULL, &consumer, &threadids[i]);
        i++;
    }

    for(i=0;i<NUM_THREADS;i++){
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&lock);
    sem_destroy(&full);
    sem_destroy(&empty);

    if(buff){
        printf("@@@@FREE@@@");
        free(buff);
    }
}   