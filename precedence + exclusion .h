//
// Created by spinner on 02/12/2023.
//
#include <stdio.h>
#include "string.h"
#include "stdlib.h"
#include "unistd.h"

#ifndef OPTI_LIGNE_ASSEMBLAGE_PRECEDENCE_EXCLUSION_H
#define OPTI_LIGNE_ASSEMBLAGE_PRECEDENCE_EXCLUSION_H
//lecture du fichier de precedence on srocke les valeur dans un tableau
int lecture_fichier_precedence( char *nomfichier,int ***valeur){
    char ligne[100];  // Tableau pour stocker une ligne du fichier
    int nombreDeLignes = 0;  // Variable pour stocker le nombre de lignes

//on initialise le fichier
    FILE *fichier;
    //ouverture en lecture seul
    fichier = fopen(nomfichier, "r");
    if (fichier == NULL) {
        printf("Impossible d'ouvrir le fichier.\n");
        return 1;
    }

// Compte le nombre de lignes dans le fichier
    while (fgets(ligne, sizeof(ligne), fichier) != NULL) {
        nombreDeLignes++;
    }

    // Allouer un tableau dynamique pour stocker les valeurs
    //dabord on alou un tableau de tableau de la taille des lignes
    *valeur = malloc(nombreDeLignes *sizeof(int *));
    if (*valeur == NULL) {
        fprintf(stderr, "Erreur d'allocation mémoire pour le tableau de pointeurs.\n");
        return 1;
    }

    // Allouer les tableaux d'entiers pour chaque ligne
    for (int i = 0; i < nombreDeLignes; i++) {
        (*valeur)[i] = malloc(2 * sizeof(int));
        if ((*valeur)[i] == NULL) {
            fprintf(stderr, "Erreur d'allocation mémoire pour la ligne %d.\n", i);
            return 1;
        }
        //on obtient don un tableau de n ligne, a chaque ligne, 2 colones
    }
    // Retourner au début du fichier
    rewind(fichier);

    // Lire et stocker les valeurs dans le tableau
    for (int i = 0; i < nombreDeLignes; i++) {
        //on lit chaque ligne et on l'enregistre dans le tableau
        fscanf(fichier, "%d %d", &(*valeur)[i][0], &(*valeur)[i][1]);

    }

    /*// Afficher les valeurs stockées dans le tableau
    for (int i = 0; i < nombreDeLignes; i++) {
        printf("Ligne %d : %d %d\n", i + 1, (*valeur)[i][0], (*valeur)[i][1]);
    }
*/
    fclose(fichier);
    return nombreDeLignes;
    //dans cette fonction on récupere les données sous la forme d'un tableau ainsi que
    //le nombre de ligne!! pas egale au nombre d'operation pour l'exclusion mais egale au
    //nombre d'arrete, une arrete egale uen exculsion
    //le programme compte toute les lignes meme les lignes vide !!!!!!!!!

}
//lecture dans les valeur pour avoie le plus grand sommet
int grand_sommet_precedence(int ** valeur, int lignes ){

    int grand_sommet = 0;

    for (int i = 0; i < lignes; i++) {
        for (int j = 0; j < 2; j++) {
            if( valeur[i][j] > grand_sommet){
                grand_sommet = valeur[i][j];
            }
        }
    }
    return grand_sommet;

}
//trouve la valeur max parmis les predecesseru
float trouverValeurMaxPredecesseurs_prede(Graphe* graphe, int i) {
    // Vérifier si le sommet a des prédécesseurs
    if (graphe->pSommet[i]->arc_entrant == NULL) {
        //printf("Le sommet %d n'a pas de prédécesseurs.\n", i);
        return -1; // Une valeur spéciale pour indiquer une absence de prédécesseurs
    }
    //printf("on cherche les sommet de %d \n",i);
    // Initialiser la valeur maximale avec la première valeur de prédécesseur
    float valeurMax = 0;
    //printf("valeur initialiser \n");
    // Parcourir la liste des prédécesseurs et mise à jour la valeur maximale si nécessaire
    pArc predecesseur = graphe->pSommet[i]->arc_entrant;
    while (predecesseur != NULL) {
        if (graphe->pSommet[predecesseur->sommet]->valeur > valeurMax) {
            valeurMax = graphe->pSommet[predecesseur->sommet]->valeur;
            //printf(" valeur max = %f du sommet %d \n",valeurMax,predecesseur->sommet);
        }
        if(predecesseur->arc_suivant == NULL){
            // printf("tout prede explorer \n");
            predecesseur = NULL;
        }
        else{
            //printf(" on passa au prede suivant \n");
            predecesseur = predecesseur->arc_suivant;
        }

    }
    //printf(" on retourn %f\n",valeurMax);
    return valeurMax;
}
//bsf plus simple
float** bfs_simple(Graphe* graphe, int sommetDepart,float **tableau )
{
    // Initialiser la file et marquer tous les sommets comme non visités
    Queue* file = initQueue();
    int* visite = (int*)malloc((graphe->ordre + 1) * sizeof(int));

    for (int i = 0; i <= graphe->ordre; i++)
        visite[i] = 0;

    // Ajouter le sommet de départ à la file et le marquer comme visité
    enfiler(file, sommetDepart);
    visite[sommetDepart] = 1;

    // Parcours en largeur
    while (file->debut != NULL)
    {
        // Retirer un sommet de la file et l'afficher
        int sommetActuel = defiler(file);
        printf(" %d \n",sommetActuel);
        //printf(" on passe au sommet %d\n\n",sommetActuel);
        //trouver la valeur max parmis les predecesseur et l'ajouter au sommet
        if(sommetActuel != sommetDepart) {
            float val = trouverValeurMaxPredecesseurs_prede(graphe, sommetActuel);
            graphe->pSommet[sommetActuel]->valeur += val;
            //printf(" la valeur du sommet %d est mises a jouor avec %f\n",sommetActuel,val);
            tableau[sommetActuel][2] = graphe->pSommet[sommetActuel]->valeur;
            //printf(" valeur sommet %d  = %f\n\n", sommetActuel, graphe->pSommet[sommetActuel]->valeur);
        }
        // Explorer les sommets adjacents non visités
        pArc arc_temp = graphe->pSommet[sommetActuel]->arc_sortant;
        //printf(" on selectionne les adjacents du sommet %d\n",sommetActuel);
        while (arc_temp != NULL)
        {
            if (!visite[arc_temp->sommet])
            {
                //printf("le sommet %d est visiter\n ",arc_temp->sommet);
                enfiler(file, arc_temp->sommet);
                visite[arc_temp->sommet] = 1;
            }
            if(arc_temp->arc_suivant == NULL){
                arc_temp = NULL;
            }
            else {
                arc_temp = arc_temp->arc_suivant;
            }
        }
    }
    // Libérer la mémoire
    free(visite);
    free(file);
    return tableau;
}
// changer pour trouver les sources
int trouverSource_simple(Graphe* graphe,int ** valeur,int taille) {
    // Tableau pour stocker le degré entrant de chaque sommet
    int degreEntrant[graphe->ordre+1];
    int existe[graphe->ordre+1] ;

    // Initialiser tous les degrés entrants à zéro et dire que tout les sommet existe
    for (int i = 0; i <= graphe->ordre; i++) {
        degreEntrant[i] = 0;
        existe[i] = 1;
        printf(" %d %d ",degreEntrant[i],existe[i] );
    }
    //printf(" \nles deux tableau sont initialiser \n");

    // Calculer les degrés entrants pour chaque sommet
    for (int i = 0; i <= graphe->ordre; i++) {
        int degrer = 0;
        //printf(" sommet %d on selectionne les predecesseur %d\n",i,degrer);

        pArc arc_temp = graphe->pSommet[i]->arc_entrant;

        //printf("justea avant la boucle \n");
        while (arc_temp != NULL) {
            degrer++;
            arc_temp = arc_temp->arc_suivant;
        }
        degreEntrant[i] = degrer;
        printf(" sommet %d degre = %d \n",i,degrer);
        //on verifie que les sommet existe pour chaque sommet on parcour tout le tableau
        for (int k = 0; k < taille; k++) {
            for (int j = 0; j < 2; ++j) {
                if (valeur[k][j] == i) {
                    existe[i] = 0;
                    printf(" sommet %d existe bien \n", k);
                }
            }
        }
    }


    // Trouver le sommet sans prédécesseur (degré entrant égal à zéro) qui existe
    for (int i = 0; i <= graphe->ordre; i++) {
        if (degreEntrant[i] == 0 && existe[i] == 0) {
            printf(" \n source = %d\n", i);
            return i;
        }
    }

    // Si aucun sommet sans prédécesseur n'est trouvé, retourner -1
    return -1;
}
// on separe en station
int station(int ** tableau,int taille){
    //on parcour tout le tableau
    for (int i = 0; i <= taille; ++i) {
        for (int j = 0; j < 4; ++j) {
            //si les sommet sont noir ou sont de la bonne couleur
            //les ajouter a la station

        }

    }
}
//fonction principale ajouter dans le main pour simplifier les choses
int exclusion_precedence_main(){
    char *nomfichierprede = NULL;
    char *nomfichierexclu = NULL;
    int ** valeurprede = NULL ;
    int ** valeurexclu = NULL ;
    int nb_lignesprede;
    int nb_lignesexclu;
    int gr_sommetprede;
    int gr_sommetexclu;
    Graphe * graphe_prede;
    Graphe * graphe_exclu;
    size_t tailleMax = 100;  // Taille maximale du nom de fichier
////la precedence //////////
            // Allocation dynamique pour le nom de fichier
            nomfichierprede = (char *)malloc(tailleMax * sizeof(char));
            if (nomfichierprede == NULL) {
                fprintf(stderr, "Erreur d'allocation mémoire.\n");
                return 1;
            }
        //on recupere le nom du fichier
            printf("Entrez le nom du fichier de precedence : \n");
            scanf("%99s", nomfichierprede);
            fflush(stdin);

            nb_lignesprede = lecture_fichier_precedence(nomfichierprede,&valeurprede);
            gr_sommetprede = grand_sommet_precedence(valeurprede,nb_lignesprede);
//////////l'exclusion/////////////
            // Allocation dynamique pour le nom de fichier
    nomfichierexclu = (char *)malloc(tailleMax * sizeof(char));
            if (nomfichierexclu == NULL) {
                fprintf(stderr, "Erreur d'allocation mémoire.\n");
                return 1;
            }
        //on recupere le nom du fichier
            printf("Entrez le nom du fichier d'exclusion : \n");
            scanf("%99s", nomfichierexclu);
            fflush(stdin);

            nb_lignesexclu = lecture_fichier_exclusion(nomfichierexclu,&valeurexclu);
            gr_sommetexclu = grand_sommet(valeurexclu,nb_lignesexclu);


    //tout d'abord, cree le graph des precedence

    printf("____________________nous allons maintenant afficher le graphe de precedence____________________ \n");
    graphe_prede = nouv_graphe_oriente(gr_sommetprede,nb_lignesprede,valeurprede);
    afficher_graph(graphe_prede,gr_sommetprede);


    //ensuite on fait la coloration sur le graph d'exclusion pour avoir les couleurs

    printf("____________________nous allons maintenant afficher le graphe d'exclusion____________________ \n");
    graphe_exclu = nouv_graphe_pas_oriente(gr_sommetexclu,nb_lignesexclu,valeurexclu);
    afficher_graph(graphe_exclu,gr_sommetexclu);

    printf("______________nous allons maintenant effectuer l'algo de welsh powell______________\n");
     welsh_powell(graphe_exclu);
    printf("_______________l'algorithme de welsh powell est terminer_______________\n");

    // on rajoute les couleur obtenue grace a l'exclusion dans le graphe de precedence
    //on par du principe que les deux graphe on le meme nombre de sommet
    for (int i = 0; i < gr_sommetprede; i++) {
        graphe_prede->pSommet[i]->couleur = graphe_exclu->pSommet[i]->couleur;
    }
    ///////////////////////on initialise le tableau de trie////////////////////////////////
    float **tableau_de_trie = (float**)malloc((gr_sommetprede+1) * sizeof(float*));
    for (int i = 0; i < gr_sommetprede+1; i++) {
        tableau_de_trie[i] = (float*)malloc(4 *sizeof(float));
    }
//initialisation du tableau de trie
    for (int i = 0; i <= gr_sommetprede; i++) {
        for (int j = 0; j < 4; j++) {
            tableau_de_trie[i][j] = 0;
        }
    }
//on allou maintenant toutes les valeurs qu'on a dans le tableau pour trier
    for (int i = 0; i <= gr_sommetprede; i++) {
        //ligne sommet
        tableau_de_trie[i][0] = i;
        //ligne couleur
        tableau_de_trie[i][1] = (float)graphe_prede->pSommet[i]->couleur;
        //ligne temps de base
        tableau_de_trie[i][2] = 1;
        //ligne existe ?
        tableau_de_trie[i][3] = 1;

    }
    //on regarde qu'elle est la source du graphe de precedence.
    int source = trouverSource_simple(graphe_prede,valeurprede,nb_lignesprede);


    bfs_simple(graphe_prede,source,tableau_de_trie);

    printf(" affichage du tableau de trie trier ");
    for (int i = 0; i <= gr_sommetprede; i++) {
        printf("\n");
        for (int j = 0; j < 4; j++) {
            printf(" %f ",tableau_de_trie[i][j] );
        }
    }
    printf("\n");




// Libération de la mémoire pour le tableau valeurprede
    for (int i = 0; i < nb_lignesprede; i++) {
        free(valeurprede[i]);
    }
    free(valeurprede);

// Libération de la mémoire pour le tableau valeurexclu
    for (int i = 0; i < nb_lignesexclu; i++) {
        free(valeurexclu[i]);
    }
    free(valeurexclu);

// Libération de la mémoire pour le nom de fichier de precedence
    free(nomfichierprede);

// Libération de la mémoire pour le nom de fichier d'exclusion
    free(nomfichierexclu);

}

#endif //OPTI_LIGNE_ASSEMBLAGE_PRECEDENCE_EXCLUSION_H
