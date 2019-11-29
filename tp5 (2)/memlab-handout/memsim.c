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
#define ttlb 16
#define tmem 507
#define tpt 1024
#define firstCadreNumber 396
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
    return addr & 0b11111111111111;
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
    struct paging_system_state *ms = malloc(sizeof(struct paging_system_state));
    ms->tlb = malloc(sizeof(struct tlb));
    ms->tlb->pageNumbers = calloc(ttlb, sizeof(unsigned long));
    ms->tlb->frameNumbers = calloc(ttlb, sizeof(unsigned long));
    ms->tlb->lastAccessTimestamps = calloc(ttlb, sizeof(unsigned int));
    ms->tlb->entryCreationTimestamps = calloc(ttlb, sizeof(unsigned int));
    ms->tlb->isUsed = calloc(ttlb, sizeof(char));
    for (int i = 0; i < ttlb; ++i)
    {
        ms->tlb->isUsed[i] = 0;
    }

    ms->pt = malloc(sizeof(struct pt));
    ms->pt->frameNumbers = calloc(tpt, sizeof(unsigned long));
    ms->pt->isValid = calloc(tpt, sizeof(char));
    for (int i = 0; i < tpt; ++i)
    {
        ms->pt->isValid[i] = 0;
    }

    ms->mem = malloc(sizeof(struct memory));
    ms->mem->pageNumbers = calloc(tmem, sizeof(unsigned long));
    ms->mem->lastAccessTimestamps = calloc(tmem, sizeof(unsigned int));
    ms->mem->entryCreationTimestamps = calloc(tmem, sizeof(unsigned int));
    ms->mem->isUsed = calloc(tmem, sizeof(char));
    for (int i = 0; i < tmem; ++i)
    {
        ms->mem->isUsed[i] = 0;
    }

    return ms;
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

    for (int i = 0; i < ttlb; i++)
    {
        unsigned long pageNumber = getPageNumber(mr->virtualAddr);
        if (ms->tlb->pageNumbers[i] == pageNumber && ms->tlb->isUsed[i] == 1)
        {
            mr->wasFoundInTLB = 1;
            mr->physicalAddr = getStartPageAddress(ms->tlb->frameNumbers[i] + firstCadreNumber) + getPageOffset(mr->virtualAddr);

            ms->tlb->lastAccessTimestamps[i] = mr->timestamp;
            ms->mem->lastAccessTimestamps[ms->tlb->frameNumbers[i]] = mr->timestamp;

            ms->pt->isValid = 1;
            //ms->pt->frameNumbers[i] = mr->physicalAddr;
            return;
        }
    }
    mr->wasFoundInTLB = 0;
}

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
//PT = TABLE DES PAGES
void lookupInPT(struct paging_system_state *ms,
                struct memory_request *mr)
{
    // TODO
    unsigned long pageNumber = getPageNumber(mr->virtualAddr);
    if (ms->pt->isValid[pageNumber] == 1)
    {
        mr->wasPageFault = 0;
        // ms->mem->lastAccessTimestamps[ms->pt->frameNumbers[pageNumber]] = mr->timestamp;

        mr->physicalAddr = getStartPageAddress(ms->mem->pageNumbers[pageNumber] + firstCadreNumber);
    }
    else
    {
        mr->wasPageFault = 1;
    }
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
    //si une page X est retirée de la moméoire
    if (mr->wasEvictionInMemory == 1)
    {
        int pageNo = getPageNumber(mr->virtualAddrEvictedFromMemory);
        for (int i = 0; i < ttlb; ++i)
        {
            if (pageNo == ms->tlb->pageNumbers[i])
            {
                ms->tlb->isUsed[i] = 1;
                ms->tlb->lastAccessTimestamps[i] = mr->timestamp;
                ms->tlb->entryCreationTimestamps[i] = mr->timestamp;
                ms->tlb->pageNumbers[i] = getPageNumber(mr->virtualAddr);
                ms->tlb->frameNumbers[i] = ms->pt->frameNumbers[ms->tlb->pageNumbers[i]];

                mr->physicalAddr = getStartPageAddress(ms->tlb->frameNumbers[i] + firstCadreNumber) + getPageOffset(mr->virtualAddr);
                mr->wasEvictionInTLB = 0;

                return;
            }
        }
    }

    //si TLB N'EST PAS FULL et pas d'éviction dans la mémoire
    for (int i = 0; i < ttlb; i++)
    {
        if (ms->tlb->isUsed[i] == 0) //si la rangé n'est pas utilisé
        {
            ms->tlb->isUsed[i] = 1;
            ms->tlb->lastAccessTimestamps[i] = mr->timestamp;
            ms->tlb->entryCreationTimestamps[i] = mr->timestamp;
            ms->tlb->pageNumbers[i] = getPageNumber(mr->virtualAddr);
            ms->tlb->frameNumbers[i] = ms->pt->frameNumbers[ms->tlb->pageNumbers[i]];

            mr->physicalAddr = getStartPageAddress(ms->tlb->frameNumbers[i] + firstCadreNumber) + getPageOffset(mr->virtualAddr);
            mr->wasEvictionInTLB = 0;

            return;
        }
    }

    //si TLB FULL => wasEvictionInTLB = 1
    int index = -1;
    unsigned int time_ = UINT_MAX;
    for(int i=0; i< ttlb; ++i){
        if(ms->tlb->lastAccessTimestamps[i] < time_){
            time_ = ms->tlb->lastAccessTimestamps[i];
            index = i;
        }
    }
    ms->tlb->isUsed[index] = 1;
    ms->tlb->lastAccessTimestamps = mr->timestamp;
    ms->tlb->entryCreationTimestamps = mr->timestamp;
    ms->tlb->pageNumbers[index] = getPageNumber(mr->virtualAddr);
    ms->tlb->frameNumbers[index] = getStartPageAddress(ms->tlb->frameNumbers[index] + firstCadreNumber) + getPageOffset(mr->virtualAddr);
    // ^^ ici aussi un petit changement
    mr->wasEvictionInTLB =1;
    mr->physicalAddr = getStartPageAddress(ms->tlb->frameNumbers[index]+ firstCadreNumber) + getPageOffset(mr->virtualAddr);
    mr->virtualAddrEvictedFromMemory = getStartPageAddress(ms->tlb->pageNumbers[index]);
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

    //if wasEvictionInMemory = 0
    //and don't forget return;
    for(int i=0; i<tmem; ++i){
        if(ms->mem->isUsed[i] != 1){

            ms->mem->isUsed[i] = 1;
            ms->mem->entryCreationTimestamps[i] = mr->timestamp;
            ms->mem->lastAccessTimestamps[i] = mr->timestamp;
            ms->mem->pageNumbers[i] = getPageNumber(mr->virtualAddr);

            ms->pt->isValid[getPageNumber(mr->virtualAddr)] = 1;
            ms->pt->frameNumbers[getPageNumber(mr->virtualAddr)] = i;

            mr->wasEvictionInMemory = 0;

            return;
        }
    }

    //if wasEvictionInMemory = 1
    int index = -1;
    unsigned long time_ = UINT_MAX;
    for(int i=0; i<tmem ; ++i){
        if(ms->mem->lastAccessTimestamps[i] < time_){   //ici un changement
            time_ = ms->mem->lastAccessTimestamps[i];
            index = i;
        }
    }

    mr->wasEvictionInMemory =1;
    mr->virtualAddrEvictedFromMemory = getStartPageAddress(ms->mem->pageNumbers[index]);

    ms->pt->isValid[ms->mem->pageNumbers[index]] = 0; //he
    ms->pt->frameNumbers[getPageNumber(mr->virtualAddr)] = index;
    ms->pt->isValid[getPageNumber(mr->virtualAddr)] = 1; //hehe

    ms->mem->lastAccessTimestamps[index] = mr->timestamp;
    ms->mem->entryCreationTimestamps[index] = mr->timestamp;
    ms->mem->isUsed[index] = 1;
    ms->mem->pageNumbers[index] = getPageNumber(mr->virtualAddr);

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
    //étape 2, si ce n'est pas dans TLB =­> chercher le cadre dans PT
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
    free(ms->tlb->pageNumbers);
    free(ms->tlb->frameNumbers);
    free(ms->tlb->lastAccessTimestamps);
    free(ms->tlb->entryCreationTimestamps);
    free(ms->tlb->isUsed);
    free(ms->tlb);
    free(ms->pt->frameNumbers);
    free(ms->pt->isValid);
    free(ms->pt);
    free(ms->mem->pageNumbers);
    free(ms->mem->lastAccessTimestamps);
    free(ms->mem->entryCreationTimestamps);
    free(ms->mem->isUsed);
    free(ms->mem);
    free(ms);
}
