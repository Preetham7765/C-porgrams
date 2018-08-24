#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#define SIZE 5
#define NUMTHREADS 2

typedef struct buffer {
    int buff[SIZE];
    int index;
} Buffer; 
pthread_cond_t cond_full, cond_empty;
pthread_mutex_t lock;
Buffer buffer;

int isfull() {
    if(buffer.index == SIZE)
        return 1;
    return 0;
}


int isempty() {
    if(buffer.index == 0)
        return 1;
    return 0;
}

void insertdata(int data){

    buffer.buff[buffer.index] = data;
    buffer.index++;
}

int readdata(){

   buffer.index --;
   int temp = buffer.buff[buffer.index];
   return temp;

}

void *producer( void* args) {

    int threadid = *(int *)args;
    printf("creating producer thread %d\n", threadid);    
    for(int i =0; i< SIZE; i++){
        pthread_mutex_lock(&lock);
        while(isfull()){
            pthread_cond_wait(&cond_full, &lock);
        }
        printf("Thread %d is writing data %d\n ", threadid, i);
        insertdata(i);
        pthread_cond_signal(&cond_empty);        
        pthread_mutex_unlock(&lock);

    }

    pthread_exit(NULL);
}

void *consumer( void* args) {


    int threadid = *(int *)args;
    int data;
    printf("creating consumer thread %d\n", threadid);
    
    for(int i =0; i< SIZE; i++){
        pthread_mutex_lock(&lock);
        while(isempty()){
            pthread_cond_wait(&cond_empty, &lock);
        }
        data = readdata();
        printf("Thread %d read data %d\n ", threadid, data);        
        pthread_cond_signal(&cond_full);
        pthread_mutex_unlock(&lock);

    }

    pthread_exit(NULL);
}



int main() {
    
    printf("inside\n");
    pthread_t threads[NUMTHREADS];
    int i ;
    int threadids[NUMTHREADS];
    pthread_cond_init(&cond_full, NULL);
    pthread_cond_init(&cond_empty,NULL);

    pthread_mutex_init(&lock,NULL);
    buffer.index = 0;
    for(i= 0; i<NUMTHREADS;){
        threadids[i] = i;
        pthread_create(&threads[i], NULL, &producer, &threadids[i]);
        i++;
        threadids[i] = i;
        pthread_create(&threads[i], NULL, &consumer, &threadids[i]);
        i++;

    }

    //sleep(5);
    for(i= 0; i<NUMTHREADS ;i++){
        pthread_join(threads[i],NULL);
    }

    pthread_mutex_destroy(&lock);
    pthread_cond_destroy(&cond_full);
    pthread_cond_destroy(&cond_empty);    

    
}
