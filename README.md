# Concurrent Sorting using Semaphores & Shared Memory



## DESCRIPTION
This concurrent sorting program implemented uses Linux shared memory and semaphores to lexicographically sort a list of unsorted distinct letters and converts all letters into lowercase. The program will fork three process from from a parent process to swap the letters based on their lexicographic order as each of the three processes will have assigned index values to handle and swap. Each of the three processes will continue to sort and swap the array of letters until sorting is complete.  The program will ask the user to input 7 letters and will store the values into shared memory which can be accessed by each process. Debug Mode was implemented to textually display to the user what each process is swapping.


The application consists of one source file for execution that includes 2 test cases which will test the concurrent program with different intialized sets of data values to ensure functionality of the program:

```CSORT.c		(main C source file)```

The application is supported by a single header file that contains variable delcarations:

```CSORT.h		(header file for CSORT.c)```



## INSTALLATION
To successfully run the program, the user must have access to Linux OS and Fedora 30 Workstation.

For Carleton University Staff Members & Students, please access the program through SYSC Lab Machines through Remote Access:
1. Connect to Carleton University VPN using Cisco AnyConnect Secure Mobility Client.
2. Log in to your MC1 Carleton University account for remote access.
3. Launch the MacOS or Windows Terminal/Command Line.
4. Type 'ssh -p 30003 carleton_username@me3475-##.labs.sce.carleton.ca' onto the command line, where '##' is the computer number.
5. Provide your Carleton University password.
6. Open the TAR file using FileZilla to run the program on the terminal.



## USAGE

#### Program Compilation
The program can be executed after opening the tar file with the required C files onto the Linux terminal. 
First, please change the current directory to the directory containing these files by using the command cd.
To compile this program type the following commands onto the terminal when the current directory is the required directory...

##### Running CSORT.c file using the command:
```
$ gcc CSORT.c -o CSORT
$ ./CSORT
```

##### User Input Prompt
The program will first ask if Debug Mode will be used. To use Debug Mode, simply input 1 and hit the Enter key and 0 if not used.
The program will then ask to input a 7 distinct letters into the terminal. After each letter is typed, please hit the Enter key.

Example - if Debug Mode is used:

```
$ Would you like to use Debug Mode? (Yes (Input 1) or No (Input 0): 1

$ Please input 7 distinct letters by hitting enter each time a letter is typed: 
X
A
z
M
W
y
D
```
The program should run and display the expected result of the lexicographic sorted array. If Debug Mode was used, the program should textually display which child processes performed swapping and which child processes did not perform swapping.



## RESULTS
After compilation, the program should display user input prompts. After inputting a set of 7 character letter values to sort, the program should output a lexicographic result of the input array.

#### Test Case 1
The input data for Test Case 1 was provided: X, A, z, M, W, y, D


##### Test Case 1: Output on the Terminal (if Debug mode is not used)
```
Memory attached at BB938000
To use Debug Mode, input 1 or 0 otherwise: 0

Please input 7 distinct letters by pressing the Enter key each time a letter is typed: 
X
A
z
M
W
y
D

Lexicographic Sorted Array: 
[ a  d  m  w  x  y  z ]
```


After compiling the CSORT.c program, the expected results of a the sorted array of letters are displayed.

#### Test Case 2
The input data for Test Case 2 was provided: Q, H, t, b, U, S, a 

##### Test Case 2: Output on the Terminal
```
Memory attached at CFCE4000
To use Debug Mode, input 1 or 0 otherwise: 0

Please input 7 distinct letters by pressing the Enter key each time a letter is typed: 
Q
H
t
b
U
S
a

Lexicographic Sorted Array: 
[ a  b  h  q  s  t  u ]
```
----------------------------------
After compiling the CSORT.c program, the expected results of a lexicographic array of '[ X, A, z, M, W, y, D ]' was obtained with the input data '[ a  d  m  w  x  y  z ]'.

The provided test cases show that the program works with various combinations of different letters as the expects results were achieved based on the output on the terminal. If Debug Mode is used, the program will textually display all the processes that have performed swapping and those that have not.



## PSEUDOCODE 
This algorithm works because it cycles through the array until each swap is done for each child process. Each child process has its own set of array index elements to swap and will continue to swap the values until the final array has achieved lexicographic order. Since semaphores are being used, it controls which process is using which resource to swap the values and ensures that there is mutual exclusion and that no two process can simultaneously access the same array element. 

```
while(sorting of array indexes is not finished) {
    // 3 separate iterations for each process
    if(child process 1 or 2 or 3 is running) {
        if(handling first index and second index of the input array in shared memroy) 
        {
            swap the first index and second index based on lexicographic order
        }
        get the semaphore
        if(handling second index and third index of the input array in shared memroy) 
        {
            swap the second index and third index if it is not in lexicographic order
        }
        if(no swapping was needed)
        {
            in debug mode:
                Process i did no swaps
        }
        release the semaphore
        checks if the set of indexes of the array are done swapping
    }
}
```



## LICENSE
Copyright 2021 Dorothy Tran. All rights reserved.
