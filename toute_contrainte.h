//
// Created by spinner on 03/12/2023.
//
#include <stdio.h>
#include "string.h"
#include "stdlib.h"
#include "unistd.h"

#ifndef OPTI_LIGNE_ASSEMBLAGE_TOUTE_CONTRAINTE_H
#define OPTI_LIGNE_ASSEMBLAGE_TOUTE_CONTRAINTE_H
//fonction qui permet de trouver lA source dans un graphe
// changer pour trouver les sources
int trouverSource(Graphe* graphe) {
    // Tableau pour stocker le degré entrant de chaque sommet
    int degreEntrant[graphe->ordre];

    // Initialiser tous les degrés entrants à zéro
    for (int i = 0; i < graphe->ordre; i++) {
        degreEntrant[i] = 0;
    }

    // Calculer les degrés entrants pour chaque sommet
    for (int i = 0; i < graphe->ordre; i++) {
        pArc arc = graphe->pSommet[i]->arc_entrant;
        while (arc != NULL) {
            degreEntrant[arc->sommet]++;
            arc = arc->arc_suivant;
        }
    }

    // Trouver le sommet sans prédécesseur (degré entrant égal à zéro)
    for (int i = 0; i < graphe->ordre; i++) {
        if (degreEntrant[i] == 0) {
            return i;
        }
    }

    // Si aucun sommet sans prédécesseur n'est trouvé, retourner -1
    return -1;
}

// fonction pour lire le fichier de temps de cycle
int lecture_fichier_temps( char *nomfichier,float ***valeur){
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
    *valeur = malloc(nombreDeLignes *sizeof(float *));
    if (*valeur == NULL) {
        fprintf(stderr, "Erreur d'allocation mémoire pour le tableau de pointeurs.\n");
        return 1;
    }

    // Allouer les tableaux d'entiers pour chaque ligne
    for (int i = 0; i < nombreDeLignes; i++) {
        (*valeur)[i] = malloc(2 * sizeof(float));
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
        fscanf(fichier, "%f %f", &(*valeur)[i][0], &(*valeur)[i][1]);

    }

    // Afficher les valeurs stockées dans le tableau
    for (int i = 0; i < nombreDeLignes; i++) {
        printf("Ligne %d : %f %f\n", i + 1, (*valeur)[i][0], (*valeur)[i][1]);
    }

    fclose(fichier);
    return nombreDeLignes;
    //dans cette fonction on récupere les données sous la forme d'un tableau ainsi que
    //le nombre de ligne!! pas egale au nombre d'operation pour l'exclusion mais egale au
    //nombre d'arrete, une arrete egale uen exculsion
    //le programme compte toute les lignes meme les lignes vide !!!!!!!!!

}
//lecture dans les valeur pour avoie le plus grand sommet
int grand_sommet_temps(float ** valeur, int lignes ){

    int grand_sommet = 0;

    for (int i = 0; i < lignes; i++) {
            if( valeur[i][0] > grand_sommet){
                grand_sommet = valeur[i][0];

        }
    }
    return grand_sommet;

}

// Algorithme de parcours en largeur (BFS)
void BFS(Graphe* graphe, int depart) {
    // Tableau pour marquer les sommets visités
    int* visite = (int*)malloc(graphe->ordre * sizeof(int));

    // Initialiser le tableau de visite à 0 (non visité)
    for (int i = 0; i < graphe->ordre; i++) {
        visite[i] = 0;
    }

    // Créer une file pour le BFS
    File* file = creerFile(graphe->ordre);

    // Marquer le sommet de départ comme visité et l'enfiler
    visite[depart] = 1;
    enfiler(file, depart);
    ///lui attribuer son temps qui vient du tableau des temps

    // Tant que la file n'est pas vide
    while (!estVide(file)) {
        // Défiler un sommet de la file et l'afficher
        int sommetCourant = defiler(file);
        printf("%d ", sommetCourant);

        // Parcourir tous les successeurs du sommet actuel
        struct Arc* successeur = graphe->pSommet[sommetCourant]->arc_sortant;
        while (successeur != NULL) {
            // Si le successeur n'a pas été visité, le marquer comme visité et l'enfiler
            if (!visite[successeur->sommet]) {
                visite[successeur->sommet] = 1;
                enfiler(file, successeur->sommet);
            }
            successeur = successeur->arc_suivant;
        }
    }

    // Libérer la mémoire allouée
    free(visite);
    libererFile(file);
}

// cree le graphe de precedence en fonction du temps de cycle et des precedences, on veut recupere
//un graphe orienter ponderer

//y ajouter les couleurs obtenue par la coloration

// trier les sommets

int toute_contraint(){
    char *nomfichier = NULL;
    int ** valeurprede = NULL ;
    int ** valeurexclu = NULL ;
    float ** valeurtemps = NULL;
    int nb_lignesprede;
    int nb_lignesexclu;
    int nb_lignetemps;
    int gr_sommetprede;
    int gr_sommetexclu;
    int gr_sommettemps;
    Graphe * graphe_prede;
    Graphe * graphe_exclu;
    size_t tailleMax = 100;  // Taille maximale du nom de fichier
    ///////////////////on cherche tout dabord a recuperer toutes les valeurs ////////////////////////////////////////////


////la precedence //////////
    // Allocation dynamique pour le nom de fichier
    nomfichier = (char *)malloc(tailleMax * sizeof(char));
    if (nomfichier == NULL) {
        fprintf(stderr, "Erreur d'allocation mémoire.\n");
        return 1;
    }
    //on recupere le nom du fichier
    printf("Entrez le nom du fichier de precedence : \n");
    scanf("%99s", nomfichier);
    fflush(stdin);

    nb_lignesprede = lecture_fichier_precedence(nomfichier,&valeurprede);
    gr_sommetprede = grand_sommet_precedence(valeurprede,nb_lignesprede);


//////////l'exclusion/////////////
    // Allocation dynamique pour le nom de fichier
    nomfichier = (char *)malloc(tailleMax * sizeof(char));
    if (nomfichier == NULL) {
        fprintf(stderr, "Erreur d'allocation mémoire.\n");
        return 1;
    }
    //on recupere le nom du fichier
    printf("Entrez le nom du fichier d'exclusion : \n");
    scanf("%99s", nomfichier);
    fflush(stdin);

    nb_lignesexclu = lecture_fichier_exclusion(nomfichier,&valeurexclu);
    gr_sommetexclu = grand_sommet(valeurexclu,nb_lignesexclu);


////////////le temps de cycle ///////////////
    nomfichier = (char *)malloc(tailleMax * sizeof(char));
    if (nomfichier == NULL) {
        fprintf(stderr, "Erreur d'allocation mémoire.\n");
        return 1;
    }
    //on recupere le nom du fichier
    printf("Entrez le nom du fichier de temps operation : \n");
    scanf("%99s", nomfichier);
    fflush(stdin);

    nb_lignetemps = lecture_fichier_temps(nomfichier,&valeurtemps);
    gr_sommettemps = grand_sommet_temps(valeurtemps,nb_lignetemps);



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


}
#endif //OPTI_LIGNE_ASSEMBLAGE_TOUTE_CONTRAINTE_H
