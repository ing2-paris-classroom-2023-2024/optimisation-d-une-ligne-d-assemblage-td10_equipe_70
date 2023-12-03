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
    //on possede maintenant toutes les information dans un graphe, reste a trouver le moyen de les trier

    //nous cherchons maintenant a faire PERT pour avoir un graphe de pert et savoir quel tache utiliser
    //avant une autre en fonction de -son temps le plus tot

    //pour cela nous devons trouver la source
    //puis parcourir le graphe de precedence a partir de la source en faisant un bfs

    //a chaque sommet faire temps precedent + temps du sommet
    //on a donc l'odre pour le temps le plus tot
    //a chaque sommet, le mettre soit dans un tableau soit dans une file

    //faire un tableau avec la liste des sommet par precedence croissante (bfs)



}

#endif //OPTI_LIGNE_ASSEMBLAGE_PRECEDENCE_EXCLUSION_H
