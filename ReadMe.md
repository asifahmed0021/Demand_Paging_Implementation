# Demand Paging using LRU paging algorithm
### Command to execute the program:
```
g++ main.cpp 
```
``` 
./a.out  
```
### Four inputs after these 2 commands will be following:
1) First line contains total number of processes (k)
2) Second line contains Virtual address space – maximum number of pages required per process (m)
3) Third line contains Physical address space – total number of frames in maim memory (f) [m > f]
4) Fourth line contains Size of the TLB (s) [s < f]

### How the implementation works:

A random number in the range (1,k) is generated to simulate the process dispatch during CPU
scheduling. The generated number is the process ID for the current process to be executed in
the CPU. The first process is dispatched without any condition. For all subsequent processes,
‘the end of execution’ triggers the dispatching of the next process.

For every process, the program selects a random number between (1, m) and assigns it as
the required number of pages of that process and allocates frames proportionately.
Execution of a process means generation of page numbers from reference string. For each
scheduled process Pi, a reference string of length lying in the range (2*mi,10*mi) is generated.
As the reference string for a specific process is parsed, for each logical page reference, first
the TLB is consulted and then RAM.

#### About program execution:
k programs will execute one by one and after a program execution is complete another program execution will start after printing appropriate message.
When a process can find local page replacement it will replace from it (using LRU) otherwise it will use a free frame.

#### Assumption:
When a process can not find free frames in main memory and local frame then it will remove frame of the program which has highest number of frames there.
