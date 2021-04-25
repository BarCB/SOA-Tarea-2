/****************************************************
 * Tecnológico de Costa Rica
 * Maestría en Ciencias de la Computación
 * Sistemas Operativos Avanzados
 * Tarea 2
 * 
 * Author:
 *  Barnum Franco Castillo Barquero
 *  
 * **************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ptrace.h>
#include <sys/user.h>
#include "include/SystemCalls.h"

#define MIN_ARGS_REQUIRED 2

void PrintHelp()
{
    printf("Wrong number of parameters\n");
    printf("Usage: tracker [-v | -V ] program [program arguments]*\n");
}

int WaitForSystemCall(pid_t child)
{
    int status;
    int error = 0;
    while (1)
    {
        error = ptrace(PTRACE_SYSCALL, child, 0, 0);
        if (error == -1)
        {
            return error;
        }

        waitpid(child, &status, 0);
        if (WIFSTOPPED(status) && WSTOPSIG(status))
            return 0;
        if (WIFEXITED(status))
            return 1;
    }
}

int main(int argc, char **argv)
{
    if (argc < MIN_ARGS_REQUIRED)
    {
        PrintHelp();
        return 1;
    }

    int printLines = 0;
    int breakLines = 0;
    int programIndex = 1;
    if (strcmp(argv[1], "-v") == 0)
    {
        printLines = 1;
        programIndex = 2;
    }
    else if (strcmp(argv[1], "-V") == 0)
    {
        breakLines = 1;
        programIndex = 2;
    }

    if ((printLines || breakLines) && argc < 3)
    {
        PrintHelp();
        return 1;
    }

    int numberParameters = argc - programIndex - 1;
    char **argumentList = (char **)malloc(numberParameters * sizeof(char *));
    for (int i = 0; i < numberParameters; i++)
    {
        argumentList[i] = (char *)malloc((1 + strlen(argv[programIndex + i])) * sizeof(char));
        strcat(argumentList[i], argv[programIndex + i + 1]);
    }

    int childPID = fork();
    // Is the parent tracker
    if (childPID > 0)
    {
        int status;
        waitpid(childPID, &status, 0);
        ptrace(PTRACE_SETOPTIONS, childPID, 0, PTRACE_O_TRACESYSGOOD);
        struct user_regs_struct regs;
        int sysCallsOccurrences[SYSTEM_CALLS_COUNT] = {0};
        while (1)
        {
            if (WaitForSystemCall(childPID))
                break;

            ptrace(PTRACE_GETREGS, childPID, NULL, &regs);

            unsigned long long int systemCallNumber = regs.orig_rax;
            unsigned long long int returnValue = regs.rax;

            if (systemCallNumber <= SYSTEM_CALLS_COUNT)
            {
                sysCallsOccurrences[systemCallNumber] += 1;
                printf("System call %lld ", systemCallNumber);
                printf("%s\n", SYSTEM_CALLS[systemCallNumber]);
            }
        }
    }
    else
    {
        // PTRACE_TRACEME indicates to its parent to trace this process
        ptrace(PTRACE_TRACEME, 0, NULL, NULL);
        kill(getpid(), SIGSTOP);
        execv(argv[programIndex], argumentList);
    }

    exit(0);
}