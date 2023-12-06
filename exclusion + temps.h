//
// Created by spinner on 06/12/2023.
//
#include <stdio.h>
#include "string.h"
#include "stdlib.h"
#include "unistd.h"
#ifndef OPTI_LIGNE_ASSEMBLAGE_EXCLUSION_TEMPS_H
#define OPTI_LIGNE_ASSEMBLAGE_EXCLUSION_TEMPS_H
// fonction pour lire le fichier de temps de cycle
int lecture_temps( char *nomfichier,float ***valeur){
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

    fclose(fichier);
    return nombreDeLignes;
    //dans cette fonction on récupere les données sous la forme d'un tableau ainsi que
    //le nombre de ligne!! pas egale au nombre d'operation pour l'exclusion mais egale au
    //nombre d'arrete, une arrete egale uen exculsion
    //le programme compte toute les lignes meme les lignes vide !!!!!!!!!

}
//lecture fichier temps de cycle
float lecture_cycle( char *nomfichier){
    float cycle;

//on initialise le fichier
    FILE *fichier;
    //ouverture en lecture seul
    fichier = fopen(nomfichier, "r");
    if (fichier == NULL) {
        printf("Impossible d'ouvrir le fichier.\n");
        return 1;
    }
    fscanf(fichier,"%f",&cycle);
    return cycle ;
}
//lecture dans les valeur pour avoie le plus grand sommet
int gr_sommet_tps(float ** valeur, int lignes ){

    int grand_sommet = 0;

    for (int i = 0; i < lignes; i++) {
        if( valeur[i][0] > grand_sommet){
            grand_sommet = valeur[i][0];

        }
    }
    return grand_sommet;

}
//algorithme d'affichage des uniter d'apres les couleures des sommets
int _triage_station(float ** tableau,int taille,float T){

    float coul = (float)'c';
    int restant = 1;
    float noir = 110;
    float tpstotal = 0;
    int station = 2;


    while(restant == 1) {
        //on parcour tout le tableau
        for (int i = 0; i <= taille; i++) {

            //l'ajout d'un sommet a la station respecte le temps de cycle
            if (tableau[i][2] + tpstotal <= T) {
                //sommet existe et pas encore placer
                if (tableau[i][3] == 0) {
                    //le sommet a la meme couleur ou est noir
                    if (tableau[i][1] == coul || tableau[i][1] == noir) {

                        //on place le sommet dans la station
                        tableau[i][3] = station;
                        //on implement le temps total de la station
                        tpstotal += tableau[i][2];
                    }
                }

            }
        }
        //on passe a la couleur suivante
        coul++;
        //on remet le temps total a 0
        tpstotal = 0;
        //on regarde si il reste des sommet sans station
        restant = 0;
        for (int i = 0; i <= taille; i++) {
            if (tableau[i][3] == 0) {
                restant = 1;
            }
        }
        //on cahnge de station
        station++;
    }
    return station - 2 ;
}
//affichage stations
void affich_stat(float ** tabl,int taille,int station ){

    printf(" nous avons trouver %d station :\n\n ",station);

    for (int i = 0; i < station; i++) {
        printf(" STATION %d :\n",i+1);

        for (int k = 0; k <= taille ; k++) {
            if (tabl[k][3] == i + 2) {
                printf("sommet %d \n", (int)tabl[k][0]);
            }

        }
    }

}

int exclusion_temps_main(){
    char *nomfichiertemp = NULL;
    char *nomfichiercycle = NULL;
    char *nomfichierexclu = NULL;

    int ** valeurexclu = NULL ;
    float ** valeurtemps = NULL;

    int nb_lignesexclu;
    int nb_lignetemps;

    int gr_sommetexclu;
    int gr_sommettemps;

    Graphe * graphe_exclu;
    size_t tailleMax = 100;  // Taille maximale du nom de fichier
    int source;
    float ** temps;
    float** tableau_de_trie ;

//////////////on cherche tout dabord a recuperer toutes les valeurs ///////////////////////////////////////


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


////////////le temps d'operation ///////////////
    nomfichiertemp = (char *)malloc(tailleMax * sizeof(char));
    if (nomfichiertemp == NULL) {
        fprintf(stderr, "Erreur d'allocation mémoire.\n");
        return 1;
    }
    //on recupere le nom du fichier
    printf("Entrez le nom du fichier de temps operation : \n");
    scanf("%99s", nomfichiertemp);
    fflush(stdin);

    nb_lignetemps = lecture_temps(nomfichiertemp,&valeurtemps);
    gr_sommettemps = gr_sommet_tps(valeurtemps,nb_lignetemps);


// Allouer un tableau dynamique pour stocker les valeurs du temps
    temps = malloc((gr_sommettemps+1) *sizeof(float *));
    if (temps == NULL) {
        fprintf(stderr, "Erreur d'allocation mémoire pour le tableau de pointeurs.\n");
        return 1;
    }
    // Allouer les tableaux d'entiers pour chaque ligne
    for (int i = 0; i <= gr_sommettemps; i++) {
        temps[i] = malloc(2 * sizeof(float));
        if (temps[i] == NULL) {
            fprintf(stderr, "Erreur d'allocation mémoire pour la ligne %d.\n", i);
            return 1;
        }
    }
    //on initialise a 0 toutes les valeurs
    for (int i = 0; i <= gr_sommettemps; i++) {
        for (int j = 0; j < 2; j++) {
            temps[i][j] = 0;
        }
    }
    //on transvase les valeur dans un tableau ou chaque case represente un sommet, et on les a tous meme ceux qui
    //n'existe pas
    for (int i = 0; i <= gr_sommettemps; i++) {
        temps[i][0] = i;
        for (int j = 0; j < nb_lignetemps; j++) {
            if(valeurtemps[j][0] == i){

                temps[i][1] = valeurtemps[j][1];

            }
        }
    }

/////////////temps de cycle /////////////////
//on recupere le temps de cycle
    nomfichiercycle = (char *)malloc(tailleMax * sizeof(char));
    if (nomfichiercycle == NULL) {
        fprintf(stderr, "Erreur d'allocation mémoire.\n");
        return 1;
    }
    //on recupere le nom du fichier
    printf("Entrez le nom du fichier de temps de cycle  : \n");
    scanf("%99s", nomfichiercycle);
    fflush(stdin);

    float T = lecture_cycle(nomfichiercycle);
    printf("\ntemps de cycle = %f \n ",T);

///////////////////////////////////////on organise les valeur recupere pour pouvoir trier en station //////////
    graphe_exclu = nouv_graphe_pas_oriente(gr_sommetexclu,nb_lignesexclu,valeurexclu);
    welsh_powell(graphe_exclu);
    ///////////////on initialise le tableau de trie pour obtenir un table de ordre colones et 4 lignes/////////////////
    //un ligne sommet
    //une ligne temps
    //une ligne couleur
//alocation dynamique
    tableau_de_trie = (float**)malloc((gr_sommettemps+1) * sizeof(float*));
    for (int i = 0; i < gr_sommettemps+1; i++) {
        tableau_de_trie[i] = (float*)malloc(4 *sizeof(float));
    }
//initialisation du tableau de trie
    for (int i = 0; i <= gr_sommettemps; i++) {
        for (int j = 0; j < 4; j++) {
            tableau_de_trie[i][j] = 0;
        }
    }
//on allou maintenant toutes les valeurs qu'on a dans le tableau pour trier
    for (int i = 0; i <= gr_sommettemps; i++) {
        //ligne sommet
        tableau_de_trie[i][0] = i;
        //ligne couleur
        tableau_de_trie[i][1] = (float)graphe_exclu->pSommet[i]->couleur;
        //ligne temps de base
        tableau_de_trie[i][2] = temps[i][1];
        //ligne existe ?
        if(tableau_de_trie[i][2]==0){
            tableau_de_trie[i][3] = 1;
            printf(" sommet %d existe pas\n ",i);
        }
    }

    //pas besoin de trier le tableau, on regrade juste les couleurs plus le temps
    int stat = _triage_station(tableau_de_trie,gr_sommettemps,T);
    affich_stat(tableau_de_trie,gr_sommettemps,stat);


}

#endif //OPTI_LIGNE_ASSEMBLAGE_EXCLUSION_TEMPS_H
