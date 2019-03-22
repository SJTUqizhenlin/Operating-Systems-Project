/**
 * Implementation of thread pool.
 */

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include "threadpool.h"

#define QUEUE_SIZE 10
#define NUMBER_OF_THREADS 3

#define TRUE 1

// this represents work that has to be 
// completed by a thread in the pool
typedef struct 
{
    void (*function)(void *p);
    void *data;
}
task;

// the work queue
task work_submit, work_todo;
task task_q[QUEUE_SIZE];
int q_n = 0;
sem_t semlock;
int length = 0;
// the worker bee
pthread_t bee[NUMBER_OF_THREADS];
pthread_mutex_t mutex;
// insert a task into the queue
// returns 0 if successful or 1 otherwise, 
int enqueue(task t) 
{
    if(length>=10)
        return 1;
    pthread_mutex_lock(&mutex);
    task_q[length].function = t.function;
    task_q[length].data = t.data;
    length = length + 1;
    pthread_mutex_unlock(&mutex);
    return 0;
}

// remove a task from the queue
task dequeue() 
{
    task work_tmp;
    work_tmp.function = task_q[length-1].function;
    work_tmp.data = task_q[length-1].data;
    length = length -1;
    return work_tmp;
}

// the worker thread in the thread pool
void *worker(void *param)
{
    while(1){
        sem_wait(&semlock);
        pthread_mutex_lock(&mutex);
        // execute the task
        task tmp = dequeue();
        execute(tmp.function, tmp.data);
        pthread_mutex_unlock(&mutex);
    }
    pthread_exit(0);
}

/**
 * Executes the task provided to the thread pool
 */
void execute(void (*somefunction)(void *p), void *p)
{
    (*somefunction)(p);
}

/**
 * Submits work to the pool.
 */
int pool_submit(void (*somefunction)(void *p), void *p)
{
    work_submit.function = somefunction;
    work_submit.data = p;
    while(1){

        if(enqueue(work_submit)==0)
            break;

    }
    sem_post(&semlock);
    return 0;
}

// initialize the thread pool
int pool_init(void)
{
    int i;
    for(i=0; i< NUMBER_OF_THREADS;++i){
        pthread_create(&bee[i],NULL,worker,NULL);
    }
    if(sem_init(&semlock, 0, 0)!=0){
        printf("sem_init is wrong\n");
        return 1;
    }
    if(pthread_mutex_init(&mutex, NULL)!=0){
        printf("pthread_mutex_init is wrong\n");
        return 1;
    }
    printf("pool init over!\n");
    return 0;
}

// shutdown the thread pool
void pool_shutdown(void)
{
    int i;
    for(i=0; i< NUMBER_OF_THREADS;++i){
        pthread_cancel(bee[i]);
    }
    for(i=0; i< NUMBER_OF_THREADS;++i){
        pthread_join(bee[i],NULL);
    }
    printf("pool shutdown over!\n");
}

