// length of a time quantum
#define QUANTUM 10
#include <stdio.h>
#include "task.h"
// run the specified task for the following time slice
void run(struct task *task, int slice) {
    printf("Running task = [%s] [%d] [%d] [%d] for %d units.\n",task->name, task->tid, task->priority, task->burst, slice);
}


// run this task for the specified time slice

