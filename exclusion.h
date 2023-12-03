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
//lecture du fichier et récuperation des differentes valeur
int lecture_fichier_exclusion( char *nomfichier,int ***valeur){
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

   /* // Afficher les valeurs stockées dans le tableau
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
//lecture dan les valeur récupere pour avoir le nombre de differents sommet
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
//lecture dans les valeur pour avoie le plus grand sommet
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
//algorithme de welsh powell pour colorer le graphh d'exclusion
int welsh_powell(Graphe * graphe) {

//pour chaque sommet regarder combien d'arc il a adjacents
    int count;
    int temp;
    int degre[graphe->ordre+1][2];

    //on compte le degres des sommet qu'on stocke dans un tableau
    for (int i = 0; i < graphe->ordre+1; i++) {
        count = 0;
        pArc arc = graphe->pSommet[i]->arc_sortant;
        while (arc != NULL) {
            count++;
            arc = arc->arc_suivant;
        }
        //degrer 0 = le numero du sommet
        degre[i][0] = i;
        //degrer 1 = le degrer des sommet
        degre[i][1] = count;
    }
    //printf("comptage des sommet\n");
    //trier les sommet par ordre decroissant en fonction de leur degrer
    for (int i = 0; i < graphe->ordre ; i++) {
        for (int j = 0; j < graphe->ordre - i ; j++) {
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
    /*printf("triage des sommet dans le tableau \n");
    //affichage test du tableau contenant les degrer avec leur sommet
    printf(" ______Affichage test du tableau_____\n");
    for (int i = 0; i < graphe->ordre+1; i++) {
        printf("(%d, %d)\n ", degre[i][0], degre[i][1]);
    }*/

    //attribuer les couleur en partant du debut du tableau et on donne dabord une couleur puis autre
    //on regarde la couleur du sommet si il est deja colorer ou pas
    int colorer[graphe->ordre+1];
    int adj_colorer[graphe->ordre+1];
    char couleur = 'c';
    //nb_couleur a 1 car on colore les sommet isoler en dehors de la boucle
    int nb_couleur = 1;
    int restant = 1;

    //on commence par colorer les sommet qui n'ont pas d'adjacent d'une couleur particuliere
    //la couleur des sommet qui sont isoler est le noir dans tout le graphe
    for (int j = 0; j < graphe->ordre+1; j++){
        if (degre[j][1] == 0) {
            graphe->pSommet[degre[j][0]]->couleur = 'n';
        }}
    //printf(" on colore tout les sommet qui sont isoler\n");



//tant que tout les sommet ne sont pas colorer alors on refait la boucle avec un autre couleur
    while (restant != 0) {
        //initialisation des tableau a 0 pour pas avoir de valeur etrange
        //on considere que chaque sommet est colorable avant de le tester,
        //on reffait le test a chaque boucle
        for (int  i = 0; i < graphe->ordre+1; i++) {
            colorer[i] = 0;
            adj_colorer[i] = 0;
        }
        //printf(" initialisation des tableau colorer et adj_colorer\n");

        //on augmente de un le nombre  couleur a chaque itération de la boucle
        nb_couleur++;
        //printf("la couleur %c est la %d couleur \n",couleur, nb_couleur);

//pour chaque couleur on regarde dans tout le tableau et on remplir les couleurs
//dans les sommet qui remplissent les conditions
        for(int i = 0; i< graphe->ordre+1; i++) {

            //on check si le sommet est deja colorer
            if (graphe->pSommet[degre[i][0]]->couleur != 'b') {
                colorer[i] = 1;
            }

            //on chek les adjacents du sommet
            pArc arc = graphe->pSommet[degre[i][0]]->arc_sortant;
            while (arc != NULL) {
                //si le sommmet a un adjacent deja colorer de la couleur actuel
                if (graphe->pSommet[arc->sommet]->couleur == couleur) {
                    adj_colorer[i] = 1;
                }
                arc = arc->arc_suivant;
            }


            //si le sommet n'est pas colorer et qu'il n'a pas d'adjacents colorer
            //alors on le colore
            if (colorer[i] == 0 & adj_colorer[i] == 0) {
                //printf("le sommet %d est maintenant colorer en %c\n", degre[i][0], couleur);
                graphe->pSommet[degre[i][0]]->couleur = couleur;
            }
        }
            //////fin de la boucle on change les couleur /////
            //on change de couleur
            //printf(" couleur %c devient ",couleur);
            couleur++;
            //printf(" %c \n",couleur);
            //on dit qu'il ne reste plus de sommet a colorer( en sachant que cette condition est verifier apres
            //comme ca tant qu'il reste des sommet non colorer cette valeur ne va pas s'apliquer
            restant = 0;
            //on regarde si il reste des sommet pas colorer
            for ( int h  = 0; h < graphe->ordre+1; h++) {
                if (graphe->pSommet[degre[h][0]]->couleur == 'b') {
                    restant = 1;
                }
            }


    }
    return nb_couleur;
    //a la fin on retourn le nombre de couleur differents, mais les couleur elle sont stocker dans les structures
    //du graphe donc on les recupere apres
}
//algorithme d'affichage des uniter d'apres les couleures des sommets
int affichage_station_exclusion(Graphe * graphe, int nb_couleur){
    //tableau ou on stske les differentes couleur
    char couleur[nb_couleur];
    int rempli = 0;

    //on commence par recupere les differentes couleur dans le graphe
    for (int i = 0; i < graphe->ordre+1; i++) {
        char valeur_actu = graphe->pSommet[i]->couleur;
        int different = 1  ;

        //on verifie que l'on a pas deja enregistrer la couleur
        for (int j = 0; j < nb_couleur; j++) {
            if (couleur[j] == valeur_actu) {
                different = 0;
                break;
            }
        }

        if(different){
            couleur[rempli] = valeur_actu;
            rempli++;
        }
    }

    printf("pour la contrainte d'exclusion : \n");
    printf(" nous avons trouver %d station differents \n\n", nb_couleur-1);

    //on regarde les sommet 1 a 1, tout ceux qui ont la meme couleur seront afficher
    //dans la meme uniter
    printf(" les sommet suivant n'ont pas de contraint d'exclusion :\n ");
    for ( int i = 0; i < graphe->ordre; i++) {
        //on note les sommet sans contrainte
        if(graphe->pSommet[i]->couleur == 'n'){
            printf(" sommet %d\n",i);
    }}

    printf(" les station sont les suivantes :\n");
    //on note les sommet en fonction de leur differentes couleurs
    for (int j = 0; j < nb_couleur; j++) {
        if( couleur[j]!= 'n') {
            printf(" voici l'uniter %c, elle est consituer des sommet :\n", couleur[j]);
            //on percour le tableau et print les sommet ayant la meme couleur
            for (int k = 0; k < graphe->ordre; k++) {
                if (graphe->pSommet[k]->couleur == couleur[j]) {
                    printf(" sommet %d \n", k);
                }
            }
        }
    }
}
//fonction rassemblant tout pour simplifier sont utilisation dans le menu
//on peut conciderer ca somme le main exclusion
int exclusion_main(){
    char *nomfichier = NULL;
    int ** valeur = NULL ;
    int nb_lignes;
    int gr_sommet;
    int nb_couleur;
    int ordre;
    Graphe * graphe_exclu;
    size_t tailleMax = 100;  // Taille maximale du nom de fichier

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

//on recupere le nombre de lignes et les valeur du fichier
//cad on recupere le nombre de case du tableau valeur
    nb_lignes = lecture_fichier_exclusion(nomfichier,&valeur);

// on compte le nombre de valeur differentes dans le tableau valeur
    ordre = different_sommet(valeur,nb_lignes);
//on recupere également la valeur du plus grand sommet
//ca nous donne le nombre de sommet differents pour le graphe
    gr_sommet = grand_sommet(valeur,nb_lignes);


    //le tableau alouer dynamiquement valeur est donc maintenant remplie des valeur des fichier texte.
    //il faut maintenant modeliser le graph d'exclusion a sovoir chaque couple représente une arrete non orienter

    printf("____________________nous allons maintenant afficher le graphe d'exclusion____________________ \n");
    graphe_exclu = nouv_graphe_pas_oriente(gr_sommet,nb_lignes,valeur);
    afficher_graph(graphe_exclu,gr_sommet);

    //on effectue l'algorithme de welsh powell, on attribut differentes couleur aux differents sommet;
    printf("______________nous allons maintenant effectuer l'algo de welsh powell______________\n");
    nb_couleur = welsh_powell(graphe_exclu);
    printf("_______________l'algorithme de welsh powell est terminer_______________\n");

    //on affiche les station en fonction des couleurs
    affichage_station_exclusion(graphe_exclu,nb_couleur);

    system("pause");
}
#endif //OPTI_LIGNE_ASSEMBLAGE_EXCLUSION_H
