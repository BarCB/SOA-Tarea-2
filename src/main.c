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

void PrintTable(int sysCallsOcurrences[])
{
    int sysCallLength = 50, countLenght = 10, index = 0, total = 0;
    int lineLength = sysCallLength + countLenght;

    char lineBuffer[60];
    char spaceBuffer[60];
    char space[50] = "                                                 ";

    printf("\n\n");
    printf("+--------------------SYSTEM CALLS TABLE---------------------+\n");
    printf("|  #     Name                                      Calls    |\n");
    for (int i = 0; i < SYSTEM_CALLS_COUNT; i++)
    {
        if (sysCallsOcurrences[i] > 0)
        {
            strcpy(spaceBuffer, space);
            sprintf(lineBuffer, "|  %s", SYSTEM_CALLS[i]);
            index = sysCallLength - strlen(lineBuffer);
            spaceBuffer[index] = '\0';
            strcat(lineBuffer, spaceBuffer);
            printf("%s", lineBuffer);

            sprintf(lineBuffer, "  %d", sysCallsOcurrences[i]);
            strcpy(spaceBuffer, space);
            index = countLenght - strlen(lineBuffer);
            spaceBuffer[index] = '\0';
            strcat(lineBuffer, spaceBuffer);
            strcat(lineBuffer, "|\n");
            printf("%s", lineBuffer);

            total += sysCallsOcurrences[i];
        }
    }

    strcpy(spaceBuffer, space);
    sprintf(lineBuffer, "|                                            Total: %d", total);
    index = lineLength - strlen(lineBuffer);
    spaceBuffer[index] = '\0';
    strcat(lineBuffer, spaceBuffer);
    strcat(lineBuffer, "|\n");
    printf("%s", lineBuffer);
    printf("+-----------------------------------------------------------+\n\n");
}

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
            printf("The child process could not run the program\n");
            return error;
        }

        waitpid(child, &status, 0);
        if (WIFSTOPPED(status) && WSTOPSIG(status))
            return 0;
        if (WIFEXITED(status))
            return 1;
    }
}

void RunTrackerProcess(int childPID, int printLines, int breakLines)
{
    int status;
    waitpid(childPID, &status, 0);
    ptrace(PTRACE_SETOPTIONS, childPID, 0, PTRACE_O_TRACESYSGOOD);
    struct user_regs_struct regs;
    int sysCallsOccurrences[SYSTEM_CALLS_COUNT] = {0};
    while (1)
    {
        if (WaitForSystemCall(childPID))
        {
            break;
        }

        ptrace(PTRACE_GETREGS, childPID, NULL, &regs);
        unsigned long long int systemCallNumber = regs.orig_rax;

        if (systemCallNumber < SYSTEM_CALLS_COUNT)
        {
            sysCallsOccurrences[systemCallNumber] += 1;
            if (printLines || breakLines)
            {
                printf("System call %s\n", SYSTEM_CALLS[systemCallNumber]);

                if (breakLines)
                {
                    getchar();
                }
            }
        }
    }

    PrintTable(sysCallsOccurrences);
}

void RunChildProcess(char **argumentList)
{
    // PTRACE_TRACEME indicates to its parent to trace this process
    ptrace(PTRACE_TRACEME, 0, NULL, NULL);
    kill(getpid(), SIGSTOP);
    int error = execvp(argumentList[0], argumentList);
    if (error < 0)
    {
        printf("The child process could not run the program\n");
        PrintHelp();
    }
}

int main(int argc, char **argv)
{
    if (argc < MIN_ARGS_REQUIRED)
    {
        PrintHelp();
        exit(EXIT_FAILURE);
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
        exit(EXIT_FAILURE);
    }

    int numberParameters = argc - programIndex;
    char **argumentList = (char **)malloc((numberParameters + 1) * sizeof(char *));
    for (int i = 0; i < numberParameters; i++)
    {
        argumentList[i] = (char *)malloc((1 + strlen(argv[programIndex + i])) * sizeof(char));
        strcat(argumentList[i], argv[programIndex + i]);
    }
    argumentList[numberParameters] = NULL;

    int childPID = fork();
    if (childPID > 0)
    {
        RunTrackerProcess(childPID, printLines, breakLines);
    }
    else
    {
        RunChildProcess(argumentList);
    }

    exit(EXIT_SUCCESS);
}