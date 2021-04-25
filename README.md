# SOA-Tarea-2
Example of a programs tracking system calls on another program, as default a table with the number of occurencies will be display at the end. The system calls 1 sys_write, 83 sys_mkdir and 316 sys_renameat2 have special messages when using -v or -V (check the examples 4, 5 and 6 to reproduce them).

To compile the program:
1. Open the terminal and head over the directory with the program
2. Run the command "make"
3. A program called "main" will be generated
    
To clean the repository:
1. Run "make clean"

How to run the program:  
    `./main [-v | -V] program [program arguments]*`

The program arguments:  
* [-v | -V]:  
    -v: despliega un mensaje cada vez que detecte un System Call de "program".  
    -V: -V is identical to the -v option, but will pause until the user presses any key to continue execution of "program".  
    The program can only receive -v or -V never both, if both are added the -V will be considered as a command to run and it display an error.  
    They are optional.  

* program:  
    It can be a program or a linux terminal command. Per example:  
        `./main ./programCompiledWithC`  
        `./main ls`
    
* [program arguments]:  
        They are all the argument that will be passed to "program".  
        They can be cero to many and separated by a space, per example.  
            `./main ls /home -a`  
    
Running examples:
1. Track a program that says helloWorld, notice is in the same folder as main.  
    `./main ./helloWorld`

2. Track a program that says helloWorld with -v.  
    `./main -v ./helloWorld`

3. Track a program that says helloWorld with -V.  
    `./main -V ./helloWorld`

4. Track a make folder command.  
    `./main mkdir newFolder`

5. Track a echo command.  
    `./main -v echo "Hello World!"`

6. Track a rename of a file.  
    `./main -v mv original.txt changeName.txt


7. Track a tracker that executes a list command, you will have to copy the main in the same folder.  
    `./main ./mainCopied ls /home -a`
