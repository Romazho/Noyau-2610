/*
 * Comm Lab - puzzle1.c
 * 
 * Ecole polytechnique de Montreal, 2018
 */

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

#include "libcommlab.h"

void puzzle1() {
    // TODO
    char token[13] = "7a554e2a4d6f";
    
    mkfifo("/tmp/pipe_ca72e8b4", 0660);
    mkfifo("/tmp/pipe_e30af157", 0660);
    
    pid_t childPID = fork();

    if(childPID == 0){   
        execl("./puzzle1/exchanger", "exchanger", NULL);
    }
   

    int output1 = open("/tmp/pipe_ca72e8b4", O_WRONLY);
     int input1 = open("/tmp/pipe_e30af157", O_RDONLY);
    
    write(output1, token, strlen(token));
    close(output1);

    char buf[100];
    kill(childPID, SIGUSR1);
    read(input1, buf ,100);

    close(input1);
    checkExchangerMessage(buf);

    wait(NULL);
}