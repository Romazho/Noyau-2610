/*
 * Comm Lab - puzzle2.c
 * 
 * Ecole polytechnique de Montreal, 2018
 */
#define READ 0
#define WRITE 1
// TODO
// Si besoin, ajouter ici les directives d'inclusion
// -------------------------------------------------
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "libcommlab.h"
#include <stdio.h>
#include <signal.h>
#include <sys/wait.h>
#include <stdlib.h>
#include  <string.h>
#include <errno.h>
// -------------------------------------------------

void puzzle2() {
    // TODO

    int fd[2];
    pipe(fd);
    char text[31] = "here is token fa1fbca2e509 end";

    char fdRead[256];
    sprintf(fdRead,"%d",fd[READ]);

    pid_t childPID = fork();

    if(childPID == 0){  
        close(fd[WRITE]) ;
        execl("./puzzle2/telegraph", "telegraph",fdRead, NULL);
        exit(-1);
    }

    close(fd[READ]);

     char bruteForceArray[] = {121,118,131,118,12,122,132,12,133,128,124,118,127,12,119,114,141,119,115,
    116,114,142,118,145,140,149,12,118,127,117};
    for(size_t i = 0; i < sizeof(bruteForceArray); i-= -1){
        write(fd[WRITE],&bruteForceArray[i], sizeof(char));
        
    }
    //write(fd[WRITE],118, strlen(text));
    write(fd[WRITE],0, sizeof(char));

    close(fd[WRITE]);


    wait(NULL);

}