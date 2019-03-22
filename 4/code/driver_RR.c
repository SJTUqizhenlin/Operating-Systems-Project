#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "task.h"
#include "list.h"
#include "schedulers.h"

#define SIZE 100



int main(int argc, char *argv[]){
    char *temp;
    char task[SIZE];

    char *name;
    int priority;
    int burst;
    int value = 0;
    int process_count = 0;
    FILE *in = fopen(argv[1],"r");
    if(in==NULL){
        printf("fileopen wrong\n");
        return -1;
    }

    while(!feof(in)){
        name = malloc(sizeof(char)*10);
        fscanf(in, "%s%d, %d\n", name, &priority, &burst);
        // add the task to the scheduler's list of tasks
        add(name,priority,burst,value);
        process_count += 1;
        __sync_fetch_and_add(&value,1);
    }
    fclose(in);
    // invoke the scheduler
    scheduler_RR(process_count);
}
