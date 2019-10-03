/*
 * Clone Lab - part1.c
 * 
 * Ecole polytechnique de Montreal, 2018
 */

// TODO
// Si besoin, ajouter ici les directives d'inclusion
// -------------------------------------------------
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
// -------------------------------------------------

void part1() {
    // TODO
    //virgina --p PID
    
    pid_t parentPID = getpid();
    pid_t childPID = fork();
    char getpidChild[100];
    char getpidParent[100];
    sprintf(getpidChild,"%d",childPID);
    sprintf(getpidParent,"%d",parentPID);
    if(childPID == 0){    //le fils entre ici
     printf("fils\n");
        execl("part1/wade","wade","--p",getpidParent, NULL);
       
    }
    else{
        printf("pere\n");
        execl("part1/virgina","virgina","--p",getpidChild, NULL);
        wait(NULL);
    }
    
}