#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include "buffer.h"

#define TRUE 1
int sleep_time;
void *producer(void *param){
	buffer_item item;
	while(1){
		sem_wait(&empty);
		item = rand();
		insert_item(item);
		printf("producer produced %d\n", item);
		sem_post(&full);
	}
}

void *consumer(void *param){
	buffer_item item;
	while(1){
		sem_wait(&full);
		remove_item(&item);
		printf("consumer consumed %d\n", item);
		sem_post(&empty);
	}
}

int main(int argc,  char *argv[]){
	// get command line arguments argv[1], argv[2], argv[3]
	sleep_time = atoi(argv[1]);
	int producer_thread_num = atoi(argv[2]);
	int consumer_thread_num = atoi(argv[3]);
	// Initialize buffer
	sem_init(&empty, 0, BUFFER_SIZE);
	sem_init(&full, 0, 0);
	pthread_mutex_init(&mutex, NULL);
	// Create producer and consumer threads
	pthread_t* producer_pthread;
	pthread_t* consumer_pthread;
	producer_pthread = (pthread_t *)malloc(sizeof(pthread_t)*producer_thread_num);
	consumer_pthread = (pthread_t *)malloc(sizeof(pthread_t)*consumer_thread_num);
	int i=0;
	for(i = 0; i< producer_thread_num; ++i){
		pthread_create(&producer_pthread[i], NULL, producer, NULL);
	}
	for(i = 0; i< consumer_thread_num; ++i){
		pthread_create(&consumer_pthread[i], NULL, consumer, NULL);
	}
	// sleep
	sleep(sleep_time);
	//exit
	return 0;
}
