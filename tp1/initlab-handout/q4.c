/*
 * Init Lab - q4.c
 * 
 * Ecole polytechnique de Montreal, 2018
 */

#include <stdint.h>
#include <stdlib.h>
#include "q4/libq4.h"

// TODO
// Si besoin, ajouter ici les directives d'inclusion
// -------------------------------------------------

// -------------------------------------------------

uint64_t question4A(uint64_t x) {
    // TODO
    return 77 *x*x + 85 * x + 5850;
}

void question4B(uint64_t x, uint64_t *result) {
    // TODO
    *result = question4A(x);
}

uint64_t *question4C(uint64_t x) {
    // TODO
    uint64_t * valeurDeRetour = calloc(0, sizeof(uint64_t));
    question4B(x,valeurDeRetour);
    return valeurDeRetour;
}

void question4D(uint64_t x, uint64_t **resultPtr) {
    // TODO
    *resultPtr = question4C(x);
}

/*
 * Attention: Vous devez impérativement obtenir le résultat du calcul de
 * l’expression demandée par un appel à la fonction _question4B (et non
 * question4B!) définie dans le fichier q4/libq4.h, que nous avons programmée
 * et qui a la même signature que votre fonction question4B.
 */
uint64_t question4E(uint64_t x) {
    // TODO
    uint64_t * valeurDeRetour = calloc(0, sizeof(uint64_t));
    _question4B(x,valeurDeRetour);
    return *valeurDeRetour;
}