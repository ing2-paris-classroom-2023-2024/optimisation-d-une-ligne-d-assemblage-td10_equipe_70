//
// Created by spinner on 30/11/2023.
//
//fichier contenant les fonction pour l'exclusion
#include <stdio.h>
#include "string.h"
#include "stdlib.h"
#include "unistd.h"

#ifndef OPTI_LIGNE_ASSEMBLAGE_EXCLUSION_H
#define OPTI_LIGNE_ASSEMBLAGE_EXCLUSION_H
///faire une fonction qui pourrait faire la lecture en fonction de ce qu'on demande///
int lecture_fichier( char *nomfichier,int ***valeur){
    char ligne[100];  // Tableau pour stocker une ligne du fichier
    int tableau[2];   // Tableau pour stocker les deux nombres de chaque ligne
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

int different_sommet(int **valeur, int lignes) {
    int compte = 0;
//on compte ne nombre de valeur differentes sur la premiere ligne
    for (int i = 0; i < lignes; i++) {
        int estDifferent = 1;  // Suppose que la paire de valeurs est différente par défaut

        for (int j = 0; j < i; j++) {
            if (valeur[i][0] == valeur[j][0] ) {
                estDifferent = 0;  // La paire de valeurs n'est pas différente
                break;
            }
        }
        compte += estDifferent;
    }
    //on compte le nombre de valeur sur la deuxieme ligne
    for (int i = 0; i < lignes; i++) {
        int estDifferent = 1;  // Suppose que la paire de valeurs est différente par défaut

        for (int j = 0; j < i; j++) {
            if (valeur[i][1] == valeur[j][1] ) {
                estDifferent = 0;  // La paire de valeurs n'est pas différente
                break;
            }
        }
        compte += estDifferent;
    }
    printf("il y a %d sommet differents\n\n",compte);

    return compte;
    //possiblement une amelioration, avec une grand boucle for en fonction du nombre de lignes, ici on le fait 2
    // fois mais on pourrati le faire N fois
}

int grand_sommet(int ** valeur, int lignes ){

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

int welsh_powell(Graphe * graphe){
//pour chaque sommet regarder combien d'arc il a adjacents
    int count;
    int temp;
    int degre[graphe->ordre][2];
    //on compte le degres des sommet qu'on stocke dans un tableau
    for (int i = 0; i < graphe->ordre; i++) {
        count = 0;
        pArc arc = graphe->pSommet[i]->arc;
        while (arc != NULL)
        {
            count++;
            arc = arc->arc_suivant;
        }
        degre[i][0] = i;
        degre[i][1] = count;
    }
    //trier les sommet par ordre decroissant en fonction de leur degrer
    for (int i = 0; i < graphe->ordre - 1; i++) {
        for (int j = 0; j < graphe->ordre - i - 1; j++) {
            // Compare les éléments de la première colonne
            if (degre[j][1] < degre[j + 1][1]) {
                // Échange les lignes complètes
                for (int k = 0; k < 2; k++) {
                    temp = degre[j][k];
                    degre[j][k] = degre[j + 1][k];
                    degre[j + 1][k] = temp;
                }
            }
        }
    }
    //affichage test du tableau
    for (int i = 0; i < graphe->ordre; i++) {
        printf("(%d, %d)\n\n ", degre[i][0], degre[i][1]);
    }
    //faire un tableau avec les sommet !!si deg(s) = 0 alors on ne le coloriera pas cor le sommet sera isoler il pourr
    //avoir soit la premmiere couleur soit n'importe quel couleur

    //attribuer les couleur en partant du debut du tableau et on donne dabord une couleur puis autre
    //on regarde la couleur du sommet si il est deja colorer ou pas
    int colorer[graphe->ordre];
    int adj_colorer[graphe->ordre];
    char couleur = 'b';
    ///while il reste des sommet non colorer on fait couleur plus 1
    //pour tout le tableau on chek la couleur
    for (int i = 0; i < graphe->ordre; i++) {
        //on commence par colorer les sommet qui n'ont pas d'adjacent d'une couleur particuliere
        //la couleur des sommet qui sont isoler est le noir
        if(degre[i][1] == 0){
            graphe->pSommet[degre[i][1]]->couleur = 'n';
        }

        //on check le sommet
        if(graphe->pSommet[degre[i][0]]->couleur == 'b'){
            colorer[i] = 0;
        }else{
            colorer[i] =1;
        }
        //on chek les adjacents
        pArc arc = graphe->pSommet[i]->arc;
        while (arc != NULL)
        {
            //si le sommmet a un adjacent deja colorer
            if(graphe->pSommet[arc->sommet]->couleur != 'b'){
                adj_colorer[i] = 1;
            }
            arc = arc->arc_suivant;
        }
        //si le sommet n'est pas colorer et qu'il n'a pas d'adjacents colorer
        //alors on le colore
        if(colorer[i] == 0 & adj_colorer[i] == 0) {
            graphe->pSommet[degre[i][0]]->couleur = couleur;
        }
        //faut il initialiser les valeurs des tableau a 0 ??
        //retourner le nombre de couleurs differentes
    }


    //on regarde les sommet 1 a 1, tout ceux qui ont la meme couleur seront afficher
    //dans la meme uniter

    //faire un affichage en fonction des couleur, chaque couleur = une station


}
#endif //OPTI_LIGNE_ASSEMBLAGE_EXCLUSION_H
