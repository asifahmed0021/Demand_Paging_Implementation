Command to execute the program:
1) g++ main.cpp
2) ./a.out
Four inputs after this 2 commands will be following:
1) First line contains total number of processes (k)
2) Second line contains Virtual address space – maximum number of pages required per process (m)
3) Third line contains Physical address space – total number of frames in maim memory (f) [m > f]
4) Fourth line contains Size of the TLB (s) [s < f]

About program execution:
k programs will execute one by one and after a program execution is complete another program execution will start after printing appropriate message.
When a process can find local page replacement it will replace from it (using LRU) otherwise it will use a free frame.

Assumption:
When a process can not find free frames in main memory and local frame then it will remove frame of the program which has highest number of frames there.
