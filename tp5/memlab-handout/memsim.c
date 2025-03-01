/*
 * Mem Lab - memsim.c
 * 
 * Ecole polytechnique de Montreal, 2018
 */

#include "memsim.h"
#include "memsim/libmemsim.h"
#include "libmemlab.h"

#include <stdlib.h>
// TODO
// Si besoin, ajoutez ici les directives d'inclusion
// -------------------------------------------------

// -------------------------------------------------

// TODO
// Si besoin, définissez ici des fonctions supplémentaires
// -------------------------------------------------------

// -------------------------------------------------------

/*
 * Calcule et renvoie le déplacement dans la page correspondant à l'adresse
 * fournie en argument.
 */
unsigned long getPageOffset(unsigned long addr)
{
    // TODO
    return (addr & 0b11111111111111);
}

/*
 * Calcule et renvoie le numéro de page correspondant à l'adresse virtuelle
 * fournie en argument.
 */
unsigned long getPageNumber(unsigned long addr)
{
    // TODO
    return addr >> 14;
}

/*
 * Calcule et renvoie l'adresse de début de page (ou cadre) correspondant au
 * numéro de page (ou de cadre) fourni en argument.
 */
unsigned long getStartPageAddress(unsigned long pageNumber)
{
    // TODO
    return pageNumber << 14;
}

/*
 * Initialise votre structure permettant de stocker l'état de la mémoire
 * 
 * Retourne:
 * - un pointeur vers une structure initialisée stockant l'état de la mémoire
 */
struct paging_system_state *initMemoryState()
{
    // TODO
    struct paging_system_state *initial_memory_state = calloc(1, sizeof(struct paging_system_state));
    initial_memory_state->tlb = calloc(1, sizeof(struct tlb));
    //est-ce qu'il faut allouer aussi les attribut des structes internes???

    initial_memory_state->pt = calloc(1, sizeof(struct pt));
    initial_memory_state->mem = calloc(1, sizeof(struct memory));

    return initial_memory_state;
}

/*
 * Cherche la traduction de l'adresse virtuelle dans le TLB.
 * 
 * Si la traduction est trouvée dans le TLB, modifier les champs:
 * - mr->wasFoundInTLB
 * - mr->physicalAddress
 * 
 * Vous devez également mettre à jour  la table
 * de pages.
 */
void lookupInTLB(struct paging_system_state *ms,
                 struct memory_request *mr)
{
    for (int i = 0; i < *ms->tlb->pageNumbers; ++i)
    {
        if (ms->tlb->pageNumbers[i] == getPageNumber(mr->virtualAddr))
        {
            mr->wasFoundInTLB = 1;
            unsigned long dummy2 = (i + 0xA) << 14;
            mr->physicalAddr = dummy2 + getPageOffset(mr->virtualAddr);
            return;
        }
    }
}

// struct paging_system_state {
//     struct tlb* tlb;
//     struct pt* pt;
//     struct memory* mem;
// };

/*
 * Cherche la traduction de l'adresse virtuelle dans la table de pages.
 * 
 * Si la traduction est trouvée dans la table de pages, modifier le champ:
 * - mr->physicalAddress
 * 
 * Sinon, modifier le champ:
 * - mr->wasPageFault
 * 
 * Vous devez également mettre à jour les timestamps dans la mémoire centrale.
 */
void lookupInPT(struct paging_system_state *ms,
                struct memory_request *mr)
{
    // TODO
    //if(ms->tlb->)
}

/*
 * Ajoute la traduction de l'adresse virtuelle dans le TLB.
 * 
 * Si le TLB est plein, vous devez prendre en compte la politique de
 * remplacement du TLB pour modifier les champs:
 * - mr->wasEvictionInTLB
 * - mr->virtualAddrEvictedFromTLB
 * 
 * N'oubliez pas d'initialiser correctement le timestamp de votre nouvelle
 * entrée dans le TLB.
 * 
 * Attention: Si une page X est retirée de la mémoire où elle est remplacée par
 * une page Y, alors le TLB est mis à jour pour remplacer X par Y.
 */
void addToTLB(struct paging_system_state *ms,
              struct memory_request *mr)
{
    // TODO
}

/*
 * Si cette fonction est appelée en dernier recours, cela signifie que la page
 * demandée n'est même pas présente en mémoire. Il faut donc l'amener en
 * mémoire puis ajouter la traduction dans la table de pages.
 * 
 * Si la mémoire est pleine, vous devez prendre en compte la politique de
 * remplacement de la mémoire pour modifier les champs:
 * - mr->wasEvictionInMemory
 * - mr->virtualAddrEvictedFromMemory
 * 
 * Dans tous les cas, vous devez modifier le champ:
 * - mr->physicalAddress
 */
void getPageIntoMemory(struct paging_system_state *ms,
                       struct memory_request *mr)
{
    // TODO
}

/*
 * Traite une demande d'accès à la mémoire.
 * 
 * Cette fonction mute les structures fournies en arguments pour modifier
 * l'état de la mémoire et donner des informations sur la demande d'accès en
 * argument (traduction en adresse physique, présence ou non de défaut de page,
 * présence ou non de la traduction dans le TLB...)
 * 
 * Arguments:
 * - un pointeur vers votre structure représentant l'état de la mémoire
 * - un pointeur vers une structure représentant la demande d'accès
 */
void processMemoryRequest(struct paging_system_state *ms,
                          struct memory_request *mr)
{
    lookupInTLB(ms, mr);
    if (mr->wasFoundInTLB == 1)
    {
        return;
    }
    /* Lookup in page table */
    lookupInPT(ms, mr);
    if (mr->wasPageFault == 0)
    {
        addToTLB(ms, mr);
        return;
    }
    /* Get frame in memory */
    getPageIntoMemory(ms, mr);
    addToTLB(ms, mr);
}

/*
 * Désalloue votre structure permettant de stocker l'état de la mémoire
 * 
 * Arguments:
 * - un pointeur vers votre structure stockant l'état de la mémoire
 */
void cleanMemoryState(struct paging_system_state *ms)
{
    free(ms->tlb);
    free(ms->pt);
    free(ms->mem);
    free(ms);
}

// struct paging_system_state {
//     struct tlb* tlb;
//     struct pt* pt;
//     struct memory* mem;
// };