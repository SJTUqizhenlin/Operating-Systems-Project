# Project 6: Banker's algorithm

Name: 韩冰

Number: 516030910523

***

For this project, you will write a program that implements the banker’s algorithm discussed in Section 8.6.3. Customers request and release resources from the bank. The banker will grant a request only if it leaves the system in a safe state. A request that leaves the system in an unsafe state will be denied. Although the code examples that describe this project are illustrated in C, you may also develop a solution using Java. 

## The Banker

### Program file tree structure.

```python
.
├── banker.c   main function
└── matrix.txt  matrix file to read
```

### Solution:

1. Initial the matrix of maximum, need, available, and allocation.
2. design the input part to get the input from the user.
3. Design the request function. In this function, to check the whether the input is illegal. And check if we answer this request, whether the state is unsafe. 
4. Design the release function. Check the instruction is illegal.
5. The detail can see the codes.

### Screenshot

#### * instruction to show the state.

![1](D:\课程\大三上\计算机系统工程\project\6\1.png)

#### To test the safe check function

![1](D:\课程\大三上\计算机系统工程\project\6\2.png)

#### To test others(request more than need or more than available)

![1](D:\课程\大三上\计算机系统工程\project\6\3.png)