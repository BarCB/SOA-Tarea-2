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
#include "include/hello.h"

#define MinArgsRequired 2

void PrintHelp()
{
    printf("Wrong number of parameters\n");
    printf("Usage: tracker [-v | -V ] program [program arguments]*\n");
}

int main(int argc, char **argv)
{
    if (argc < MinArgsRequired)
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

    char *prograName = argv[programIndex];
    printf("Tracker%s\n", argv[0]);
    printf("Program%s\n", prograName);
    printf("Program arguments\n");

    int numberParameters = argc - programIndex - 1;
    char **paramList = (char **)malloc(numberParameters * sizeof(char *));
    for (int i = 0; i < numberParameters; i++)
    {
        paramList[i] = (char *)malloc((1 + strlen(argv[programIndex + i])) * sizeof(char));
        strcat(paramList[i], argv[programIndex + i + 1]);
        printf("Arg %d: %s\n", i, paramList[i]);
    }

    exit(0);
}