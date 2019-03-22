#define MIN_PRIORITY 1
#define MAX_PRIORITY 10
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include "cpu.h"

struct node *list_head[1000];


// add a task to the list 
void add(char *name, int priority, int burst, int value){
	struct task *Task_tmp = malloc(sizeof(struct task));
	Task_tmp->name = name;
	Task_tmp->priority = priority;
	Task_tmp->burst = burst;
	Task_tmp->left_burst = burst;
	Task_tmp->tid = value;
	insert(list_head, Task_tmp);
}

void compute_time(int count, int *turnaround_time, int* waiting_time, int* response_time){
	int t_t = 0;
	int w_t = 0;
	int r_t = 0;
	int i=0;
	for(i = 0;i < count;++i){
		t_t += turnaround_time[i];
		w_t += waiting_time[i];
		r_t += response_time[i];
	}
	printf("Average turnaround time: %f \n", t_t*1.0/count);
	printf("Average waiting time: %f \n", w_t*1.0/count);
	printf("Average response time: %f \n", r_t*1.0/count);
}


// invoke the scheduler
void schedule_FCFS(int count){
	struct task * next_task;

	int current_time = 0;
	int *turnaround_time;
	turnaround_time = (int*)malloc(sizeof(int)*count);
	int *waiting_time;
	waiting_time = (int*)malloc(sizeof(int)*count);
	int *response_time;
	response_time = (int*)malloc(sizeof(int)*count);
	int i=0;
	for(i=0;i<count;++i){
		turnaround_time[i] = 0;
		waiting_time[i] = 0;
		response_time[i] = 0;
	}

	while(next_task = traverse(*list_head)){
		waiting_time[next_task->tid] = current_time;
		response_time[next_task->tid] = current_time;
		run(next_task,next_task->burst);
		current_time += next_task->burst;
		Delete(list_head, next_task);
		turnaround_time[next_task->tid] = current_time;
	}
	compute_time(count, turnaround_time, waiting_time, response_time);
}




void scheduler_SJF(int count){
	struct task * next_task;

	int current_time = 0;
	int *turnaround_time;
	turnaround_time = (int*)malloc(sizeof(int)*count);
	int *waiting_time;
	waiting_time = (int*)malloc(sizeof(int)*count);
	int *response_time;
	response_time = (int*)malloc(sizeof(int)*count);
	int i=0;
	for(i=0;i<count;++i){
		turnaround_time[i] = 0;
		waiting_time[i] = 0;
		response_time[i] = 0;
	}

	while(1){
		struct node *tmp = *list_head;
		struct task * next_task = tmp->task;
		while(tmp !=NULL){
			if(tmp->task->burst < next_task->burst){
				next_task = tmp->task;
			}
			tmp = tmp->next;
		}
		waiting_time[next_task->tid] = current_time;
		response_time[next_task->tid] = current_time;
		run(next_task, next_task->burst);
		current_time += next_task->burst;
		Delete(list_head,next_task);
		turnaround_time[next_task->tid] = current_time;
		if(list_head[0]==NULL)
			break;
	}
	compute_time(count, turnaround_time, waiting_time, response_time);
}


void scheduler_Priority(int count){
	struct task * next_task;

	int current_time = 0;
	int *turnaround_time;
	turnaround_time = (int*)malloc(sizeof(int)*count);
	int *waiting_time;
	waiting_time = (int*)malloc(sizeof(int)*count);
	int *response_time;
	response_time = (int*)malloc(sizeof(int)*count);
	int i=0;
	for(i=0;i<count;++i){
		turnaround_time[i] = 0;
		waiting_time[i] = 0;
		response_time[i] = 0;
	}

	while(1){
		struct node *tmp = *list_head;
		struct task * next_task = tmp->task;
		while(tmp !=NULL){
			if(tmp->task->priority < next_task->priority){
				next_task = tmp->task;
			}
			tmp = tmp->next;
		}
		waiting_time[next_task->tid] = current_time;
		response_time[next_task->tid] = current_time;
		run(next_task, next_task->burst);
		current_time += next_task->burst;
		Delete(list_head,next_task);
		turnaround_time[next_task->tid] = current_time;
		if(list_head[0]==NULL)
			break;
	}
	compute_time(count, turnaround_time, waiting_time, response_time);
}

void scheduler_RR(int count){

	int current_time = 0;
	int *turnaround_time;
	turnaround_time = (int*)malloc(sizeof(int)*count);
	int *waiting_time;
	waiting_time = (int*)malloc(sizeof(int)*count);
	int *response_time;
	response_time = (int*)malloc(sizeof(int)*count);
	int *response_time_flag;
	response_time_flag = (int*)malloc(sizeof(int)*count);
	int i=0;
	for(i=0;i<count;++i){
		turnaround_time[i] = 0;
		waiting_time[i] = 0;
		response_time[i] = 0;
		response_time_flag[i] = 0;
	}

	while(1){
		struct node *tmp = *list_head;
		struct node *next_node;
		if(tmp==NULL){
			break;
		}
		while(tmp!=NULL){
			next_node = tmp->next;
			if(response_time_flag[tmp->task->tid]==0){
				response_time_flag[tmp->task->tid] = 1;
				response_time[tmp->task->tid] = current_time;
			}
			if(tmp->task->left_burst <= QUANTUM){
				run(tmp->task, tmp->task->left_burst);
				current_time += tmp->task->left_burst;
				Delete(list_head, tmp->task);
				turnaround_time[tmp->task->tid] = current_time;
				waiting_time[tmp->task->tid] = current_time-(tmp->task->burst);
			}
			else{
				run(tmp->task, QUANTUM);
				current_time += QUANTUM;
				tmp->task->left_burst -= QUANTUM;
			}
			tmp = next_node;
		}
	}
	compute_time(count, turnaround_time, waiting_time, response_time);
}

void scheduler_RR_Priority(int count){
	int current_time = 0;
	int *turnaround_time;
	turnaround_time = (int*)malloc(sizeof(int)*count);
	int *waiting_time;
	waiting_time = (int*)malloc(sizeof(int)*count);
	int *response_time;
	response_time = (int*)malloc(sizeof(int)*count);
	int *response_time_flag;
	response_time_flag = (int*)malloc(sizeof(int)*count);
	int i=0;
	for(i=0;i<count;++i){
		turnaround_time[i] = 0;
		waiting_time[i] = 0;
		response_time[i] = 0;
		response_time_flag[i] = 0;
	}

	while(1){
		struct node *tmp = *list_head;
		struct node *next_node;
		if(tmp==NULL){
			break;
		}
		int least_Priority = 10000;
		while(tmp!=NULL){
			if(tmp->task->priority< least_Priority)
				least_Priority = tmp->task->priority;
			tmp = tmp->next;
		}
		tmp = *list_head;
		while(tmp!=NULL){
			next_node = tmp->next;
			if(tmp->task->priority == least_Priority){
				if(response_time_flag[tmp->task->tid]==0){
					response_time_flag[tmp->task->tid] = 1;
					response_time[tmp->task->tid] = current_time;
				}
				if(tmp->task->left_burst <= QUANTUM){
					run(tmp->task, tmp->task->left_burst);
					current_time += tmp->task->left_burst;
					Delete(list_head, tmp->task);
					turnaround_time[tmp->task->tid] = current_time;
					waiting_time[tmp->task->tid] = current_time-(tmp->task->burst);
				}
				else{
					run(tmp->task, QUANTUM);
					current_time += QUANTUM;
					tmp->task->left_burst -= QUANTUM;
				}
			}
			tmp = next_node;
		}
	}
	compute_time(count, turnaround_time, waiting_time, response_time);
}