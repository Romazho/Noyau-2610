/*
 * Init Lab - q2.c
 * 
 * Ecole polytechnique de Montreal, 2018
 */

// TODO
// Si besoin, ajouter ici les directives d'inclusion
// -------------------------------------------------
#include <stdio.h>
#include <unistd.h>

// -------------------------------------------------

/*
 * Vous devez imprimer le message indiqué dans l'énoncé:
 * - En exécutant un premier appel à printf AVANT l'appel à write
 * - Sans utiliser la fonction fflush
 * - En terminant chaque ligne par le caractère '\n' de fin de ligne
 */
void question2() {
    // TODO
    
     printf("1d597d465229 (printed using printf)");
     write(STDOUT_FILENO,"1d597d465229 (printed using write)\n",36);
     printf("\n");

    //write(1,"1d597d465229 (printed using write)\n 1d597d465229 (printed using printf)\n",500);
    //printf("1d597d465229 (printed using write)\n1d597d465229 (printed using printf)\n");
   //// write(1,"\n",2);
}


