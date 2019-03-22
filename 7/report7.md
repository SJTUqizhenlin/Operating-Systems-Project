# Project 7: Contiguous Memory Allocation

Name: 韩冰

Number: 516030910523

***

This project will involve managing a contiguous region of memory of size MAX where addresses may range from 0 ... MAX - 1. Your program must respond to four different requests: 

1. Request for a contiguous block of memory 
2. Release of a contiguous block of memory 
3. Compact unused holes of memory into one single block 
4. Report the regions of free and allocated memory 

Your program will be passed the initial amount of memory at startup. For example, the following initializes the program with 1 MB (1,048,576 bytes) of memory:  ./allocator 1048576

## The Memory Allocation

### Program file tree structure.

```python
.
├── allocate.c   main function
└── report.pdf  readme file
```

### Solution:

1. Initial the array of the all jobs, and initial a job struct which includes the job's start and ends position.
2. Design the main function to interact with user. Read the instructions and print the result.
3. After reading the instructions, if request. then iterate the array to find the interval and put the job into array.
4. The detail can see the codes.

### Screenshot

#### RQ for some memory, then show the state

![1](D:\课程\大三上\计算机系统工程\project\7\1.png)

#### RL for some jobs, then show the state

![1](D:\课程\大三上\计算机系统工程\project\7\2.png)

#### RQ for other allocation

![1](D:\课程\大三上\计算机系统工程\project\7\3.png)

####Compact the allocation

![4](D:\课程\大三上\计算机系统工程\project\7\4.png)