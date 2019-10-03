/*
 * Clone Lab - part2.c
 * 
 * Ecole polytechnique de Montreal, 2018
 */

#include "libclonelab.h"

#include <stdio.h>

// TODO
// Si besoin, ajouter ici les directives d'inclusion
// -------------------------------------------------
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
// -------------------------------------------------

#define PART2_OUTPUT_FILE_PATH "part2Output.txt"

void part2() {
    // Ouverture du fichier de sortie pour la question 2.3
    FILE* part2OutputFile = fopen(PART2_OUTPUT_FILE_PATH, "a");

    // TODO
    //void registerProc(int procLevel, int procNum, pid_t pid, pid_t ppid)
    //action 0
    //1: terminal
    //2: appel fonction
    //3: librairies

    pid_t pid_level_0 = getpid();
    char pid[20];
    sprintf(pid,"%d",pid_level_0);
    if(fork()== 0){
        registerProc(1, 1, getpid(), getppid());        //action 1.1
        execl("part2/level1.1","level1.1",pid, NULL);
       
        if(fork() == 0){
            registerProc(2, 1, getpid(), getppid());      //action 2.1
            
            exit(0);
        }
       
        wait(NULL);
        exit(0);
    }
    
    if(fork() == 0){
         registerProc(1, 2, getpid(), getppid());       //action 1.2
         execl("part2/level1.2","level1.2",pid, NULL);
        if(fork() == 0){
            registerProc(2, 2, getpid(), getppid());    //action 2.2
             exit(0);
        }
        wait(NULL);
        exit(0);
    }

    if(fork() == 0){
         registerProc(1, 3, getpid(), getppid());       //action 1.3
        if(fork() == 0){
            registerProc(2, 3, getpid(), getppid());    //action 2.3
             exit(0);
        }
          if(fork() == 0){
            registerProc(2, 4, getpid(), getppid());    //action 2.3
             exit(0);
        }
        wait(NULL);
        exit(0);
    }

    if(fork() == 0){
        registerProc(1, 4, getpid(), getppid());       //action 1.4
        if(fork() == 0){
            registerProc(2, 5, getpid(), getppid());    //action 2.4
            exit(0);
        }
        wait(NULL);
        exit(0);
    }

    while (wait(NULL) > 0) // Pour attendre que les fils soient TERMINÉ
    {
        
    }
     
    
}