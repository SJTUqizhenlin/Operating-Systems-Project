#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "task.h"


struct node {
    Task *task;
    struct node *next;
};

// insert and delete operations.
// add a new task to the list of tasks
void insert(struct node **head, struct task *newTask) {
    // add the new task to the list 
    struct node *newNode = malloc(sizeof(struct node));
    newNode->task = newTask;
    newNode->next = *head;
    *head = newNode;
}

// delete the selected task from the list
void Delete(struct node **head, struct task *task) {
    struct node *temp;
    struct node *prev;

    temp = *head;
    // special case - beginning of list
    if (strcmp(task->name,temp->task->name) == 0) {
        *head = (*head)->next;
    }
    else {
        // interior or last element in the list
        prev = *head;
        temp = temp->next;
        while (strcmp(task->name,temp->task->name) != 0) {
            prev = temp;
            temp = temp->next;
        }

        prev->next = temp->next;
    }
}

// traverse the list
Task* traverse(struct node *head) {
    struct node *temp;
    temp = head;
    while (temp != NULL) {
        if(temp->next==NULL)
            return temp->task;
        temp = temp->next;
    }
    return NULL;
}
