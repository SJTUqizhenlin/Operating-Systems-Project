#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include "buffer.h"
int length = 0;
int insert_item(buffer_item item){
	pthread_mutex_lock(&mutex);
	//int index;
	//sem_getvalue(&full,&index);
/*	int i=0;
	for(i=1;i<BUFFER_SIZE;++i){
		buffer[i] = buffer[i-1];
	}*/
	buffer[length] = item;
	length = length + 1;
	pthread_mutex_unlock(&mutex);
	return 0;
}
int remove_item(buffer_item *item){
	pthread_mutex_lock(&mutex);
	*item = buffer[length-1];
	//int index;
	//sem_getvalue(&full,&index);
/*	int i=0;
	for(i=0;i<BUFFER_SIZE-1;++i){
		buffer[i] = buffer[i+1];
	}*/
	length = length -1;
	pthread_mutex_unlock(&mutex);
	return 0;
}
