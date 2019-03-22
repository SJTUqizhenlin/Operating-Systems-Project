# Project 4: Scheduling Algorithms

Name: 韩冰

Number: 516030910523

***

## 1.Program

In this project, I will implement a program to simulate the scheduling algorithms such as FCFS, SJF, Priority, RR and RR priority.

The program can be divided into three parts: 

1. Read the tasks from the file, create the task link list.
2. Design the task data structure and schedule algorithm.
3. Compute the turnaround time, waiting time, and response time.

### Program file tree structure.

```python
.
├── cpu.h     simulate the cpu to run the task, actually print running state of task
├── driver_FCFS.c  The main function to run the FCFS algorithm
├── driver_Priority.c  The main function to run the Priority algorithm
├── driver_RR.c        The main function to run the RR algorithm
├── driver_RR_Priority.c   to run the RR priority algorithm
├── driver_SJF.c        to run the SJF algorithm
├── fcfs    
├── list.h	        Design the task link list, and operation to insert and delete
├── priority
├── rr
├── rr_p
├── schedulers.h    algorithm implementation
├── schedule.txt    test data
├── sjf
└── task.h          Design the task data structure
```



## 2. FCFS

After create the task list, everytime return the first task to run in the cpu.

### Result:

![1](D:\课程\大三上\计算机系统工程\project\4\screenshot\1.png)

We can see that the **tid** is increasing. It's said that the task first come first served.

## 3. SJF

After create the task list, everytime return the shortest task to run in the cpu.

### Result:

![2](D:\课程\大三上\计算机系统工程\project\4\screenshot\2.png)

We can see that the **burst time** is increasing. It's said that the task shortest first served.

## 3. Priority

After create the task list, everytime return the highest priority task to run in the cpu.

### Result:

![3](D:\课程\大三上\计算机系统工程\project\4\screenshot\3.png)

We can see that the **priority** is increasing. It's said that the task highest priority first served.

## 4. RR

After create the task list, everytime return the task in order.

### Result:

![4](D:\课程\大三上\计算机系统工程\project\4\screenshot\4.png)

We can see that the **runing time** is under **QUANTUM**. It's said that the task run in round robin.

## 5.RR_Priority

After create the task list, everytime return the tasks which are the highest priority in order.

### Result:

![5](D:\课程\大三上\计算机系统工程\project\4\screenshot\5.png)

We can see that the **runing time** is under **QUANTUM** and the **priority** is increasing. It's said that the task run in round robin of priority.