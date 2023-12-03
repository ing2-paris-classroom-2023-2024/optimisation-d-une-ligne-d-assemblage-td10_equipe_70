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

    // Afficher les valeurs stockées dans le tableau
    for (int i = 0; i < nombreDeLignes; i++) {
        printf("Ligne %d : %d %d\n", i + 1, (*valeur)[i][0], (*valeur)[i][1]);
    }

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
//fonction principale ajouter dans le main pour simplifier les choses
int exclusion_precedence_main(){
    //tout d'abord, cree le graph des precedence
    //ensuite dans le graphe des precedence qui est un graph orienter
    // on rajoute les couleur obtenue grace a l'exclusion

    //on possede maintenant toutes les information dans un graphe, reste a trouver le moyen de les trier
    //pour les trier,
    //- on pourrait separer le graph en deux, dans la premiere moitier, toutes les station qui sont de la
    //  meme couleur ou qui n'ont pas de contrainte sont mis ensemble, ensuite
    //- trier dans les station deja cree par le fonction d'exclusion en fonction des precedences en
    //  n'oubliant pas qu'on peut utiliser les sommmet qui n'on pas de contrainte d'exclusion


    /* pour le programme ou on prend tout a la fois
     *      pour trier, on doit partir de la contrainte d'exclusion puis faire
     *
     *
     *
     */
}
#endif //OPTI_LIGNE_ASSEMBLAGE_PRECEDENCE_EXCLUSION_H
