/*
 * Sync Lab - installer.c
 * 
 * Ecole polytechnique de Montreal, 2018
 */

#include "libsynclab.h"
#include "installer/libinstaller.h"

// TODO
// Si besoin, ajouter ici les directives d'inclusion
// -------------------------------------------------
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>

// -------------------------------------------------

// TODO
// Si besoin, définissez ici des types de structures et/ou
// des variables
// -------------------------------------------------------

char *tableau[] = {"droohuct-4.4", "sney-1.9", "libroptisp-5.17", "flom-2.4", "wem-5.12", "libblassaw-5.15", "libgrogoof-7.6", "noow-9.9", "muz-2.11", "clog-8.1"};

// -------------------------------------------------------

// TODO
// Si besoin, définissez ici des fonctions supplémentaires
// -------------------------------------------------------

// -------------------------------------------------------

/*
 * Cette fonction alloue et initialise certains champs de la structure md qui
 * sont utilisés dans le reste des fonctions.
 */
void initializeManagementData(struct management_data *md)
{
    // TODO (Q2, Q3)
    md->downloadSemaphore = calloc(1, sizeof(sem_t));
    for (int i = 0; i < 10; ++i)
    {
        md->canInstallPackage[i] = calloc(1, sizeof(sem_t));
        if (i == 9 || i == 6 || i == 8 || i == 7)
        {
            sem_init(md->canInstallPackage[i], 0, 1);
        }
        else
        {
            sem_init(md->canInstallPackage[i], 0, 0);
        }
    }
    sem_init(md->downloadSemaphore, 0, 2);
}

/*
 * Cette fonction nettoie les champs de la structure md qui ont été initialisés
 * par la fonction initializeManagementData.
 */
void cleanupManagementData(struct management_data *md)
{
    // TODO (Q2, Q3)
    sem_destroy(md->downloadSemaphore);
    for (int i = 0; i < 10; ++i)
    {
        free(md->canInstallPackage[i]);
    }
    free(md->downloadSemaphore);
}

/*
 * Cette fonction télécharge et installe le paquet logiciel dont le numéro est
 * passé en argument.
 */
void installPackage(int packageNum, struct management_data *md)
{
    sem_wait(md->downloadSemaphore);

    doPackageDownload(tableau[packageNum], md);

    sem_post(md->downloadSemaphore);

    //utiliser canInstallPackage
    sem_wait(md->canInstallPackage[packageNum]);
    if (md->isPackageInstalled[packageNum] == 0)
    {
        doPackageInstall(tableau[packageNum], md);
        md->isPackageInstalled[packageNum] = 1;
        //  md->canInstallPackage[packageNum] = 0;
    }
    sem_post(md->canInstallPackage[packageNum]);

    //md->isPackageInstalled[packageNum] = 1;
}

/*
 * Cette fonction vérifie si le paquet logiciel dont le numéro est passé en
 * argument est prêt pour l'installation. Si c'est le cas, la fonction débloque
 * le paquet pour que son installation puisse se lancer.
 * 
 * NOTE: Cette fonction vous aidera à clarifier votre code pour la fonction
 * postPackageInstall. Il est fortement recommandée de l'utiliser, mais ce
 * n'est pas obligatoire.
 */
void wakePackage(int wokenPackageNum, struct management_data *md)
{
    // TODO (Q3)
}

/*
 * Cette fonction est exécutée après l'installation du paquet logiciel dont
 * le numéro est passé en argument. Son rôleF est de marquer le paquet comme
 * installé dans la structure md, et également de débloquer l'installation des
 * paquets logiciels qui pourraient désormais être prêts pour installation.
 * 
 * Afin de clarifier votre code, il est conseillé de compléter et d'utiliser la
 * fonction wakePackage définie juste au-dessus.
 * 
 * ATTENTION: Cette fonction est testée de manière unitaire par le script de
 * notation. Vous devez vous assurer qu'elle a bien le comportement décrit plus
 * haut et qu'elle ne modifie pas d'autres variables que celles stockées dans
 * la structure md.
 */
void postPackageInstall(int packageNum, struct management_data *md)
{
    // TODO (Q3)
}

/*
 * Cette fonction crée les dix fils d'exécution qui seront chargés de
 * l'installation des paquets logiciels.
 * Chaque fil d'exécution doit exécuter la fonction threadedPackageInstaller
 * en lui passant comme argument:
 * - le numéro du paquet logiciel à installer (compris entre 0 et 9)
 * - le pointeur md vers la structure passée en argument de la fonction
 *   installer
 */
struct Wrapper
{
    int packageNum;
    struct management_data *data;
};

void toWrapper(struct Wrapper *wrap)
{
    threadedPackageInstaller(wrap->packageNum, wrap->data);
    //printf("%d\n",etoile->packageNum);
    //return NULL;
}

void installer(struct management_data *md)
{
    // NE PAS MODIFIER
    // --------------------------
    initializeManagementData(md);
    installerTestInit(md);
    // --------------------------

    // TODO (Q1)
    // Lancer les fils d'exécution
    // ---------------------------

    for (int i = 0; i < 10; ++i)
    {
        struct Wrapper *wrap = calloc(1, sizeof(struct Wrapper));
        wrap->packageNum = i;
        wrap->data = md;
        //printf("%d\n",i);

        pthread_create(&(md->tids[i]), NULL, (void *)toWrapper, wrap);
    }

    for (int i = 0; i < 10; ++i)
    {
        pthread_join(md->tids[i], NULL);
    }

    // ---------------------------

    // NE PAS MODIFIER
    // -----------------------
    cleanupManagementData(md);
    // -----------------------
}
