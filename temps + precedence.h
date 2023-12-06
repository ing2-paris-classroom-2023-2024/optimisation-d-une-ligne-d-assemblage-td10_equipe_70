//
// Created by spinner on 06/12/2023.
//
#include <stdio.h>
#include "string.h"
#include "stdlib.h"
#include "unistd.h"

#ifndef OPTI_LIGNE_ASSEMBLAGE_TEMPS_PRECEDENCE_H
#define OPTI_LIGNE_ASSEMBLAGE_TEMPS_PRECEDENCE_H
//lecture du fichier de precedence on srocke les valeur dans un tableau
int lecture_fichier_precedence_( char *nomfichier,int ***valeur){
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

    fclose(fichier);
    return nombreDeLignes;
    //dans cette fonction on récupere les données sous la forme d'un tableau ainsi que
    //le nombre de ligne!! pas egale au nombre d'operation pour l'exclusion mais egale au
    //nombre d'arrete, une arrete egale uen exculsion
    //le programme compte toute les lignes meme les lignes vide !!!!!!!!!

}
//lecture fichier temps de cycle
float lecture_fichier_cycle_( char *nomfichier){
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
// fonction pour lire le fichier de temps de cycle
int lecture_fichier_temps_( char *nomfichier,float ***valeur){
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
//lecture dans les valeur pour avoie le plus grand sommet
int grand_sommet_precedence_(int ** valeur, int lignes ){

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
//lecture dans les valeur pour avoie le plus grand sommet
int grand_sommet_temps_(float ** valeur, int lignes ){

    int grand_sommet = 0;

    for (int i = 0; i < lignes; i++) {
        if( valeur[i][0] > grand_sommet){
            grand_sommet = valeur[i][0];

        }
    }
    return grand_sommet;

}
//trouve la valeur max parmis les predecesseru
float ValeurMaxPredecesseurs(Graphe* graphe, int i) {
    // Vérifier si le sommet a des prédécesseurs
    if (graphe->pSommet[i]->arc_entrant == NULL) {
        return -1; // Une valeur spéciale pour indiquer une absence de prédécesseurs
    }
    // Initialiser la valeur maximale avec la première valeur de prédécesseur
    float valeurMax = 0;
    // Parcourir la liste des prédécesseurs et mise à jour la valeur maximale si nécessaire
    pArc predecesseur = graphe->pSommet[i]->arc_entrant;
    while (predecesseur != NULL) {
        if (graphe->pSommet[predecesseur->sommet]->valeur > valeurMax) {
            valeurMax = graphe->pSommet[predecesseur->sommet]->valeur;
        }
        if(predecesseur->arc_suivant == NULL){
            predecesseur = NULL;
        }
        else{
            predecesseur = predecesseur->arc_suivant;
        }

    }
    return valeurMax;
}
//trier les tableau
void triBulles_(float **tableau, int taille) {
    for (int i = 0; i < taille - 1; i++) {
        for (int j = 0; j < taille - i - 1; j++) {
            // Comparaison et échange si l'élément est plus grand que le suivant en fonction du temps on echange tout
            if (tableau[j][2] > tableau[j + 1][2]) {
                float temp = tableau[j][2];
                float temp0 = tableau[j][0];
                float temp1 = tableau[j][1];
                float temp3 = tableau[j][3];
                tableau[j][2] = tableau[j+1][2];
                tableau[j][0] = tableau[j+1][0];
                tableau[j][1] = tableau[j+1][1];
                tableau[j][3] = tableau[j+1][3];

                tableau[j+1][2] = temp;
                tableau[j+1][0] = temp0;
                tableau[j+1][1] = temp1;
                tableau[j+1][3] = temp3;
            }
        }
    }
}
// Fonction pour effectuer un parcours en largeur (BFS) sur le graphe
float** bfs_(Graphe* graphe, int sommetDepart,float **tableau )
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
        printf(" %d",sommetActuel);
        //trouver la valeur max parmis les predecesseur et l'ajouter au sommet
        if(sommetActuel != sommetDepart) {
            float val = ValeurMaxPredecesseurs(graphe, sommetActuel);
            graphe->pSommet[sommetActuel]->valeur += val;
            tableau[sommetActuel][2] = graphe->pSommet[sommetActuel]->valeur+ val;
        }
        // Explorer les sommets adjacents non visités
        pArc arc_temp = graphe->pSommet[sommetActuel]->arc_sortant;
        while (arc_temp != NULL)
        {
            if (!visite[arc_temp->sommet])
            {
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
int trouverSource_(Graphe* graphe,int ** valeur,int taille) {
    // Tableau pour stocker le degré entrant de chaque sommet
    int degreEntrant[graphe->ordre+1];
    int existe[graphe->ordre+1] ;

    // Initialiser tous les degrés entrants à zéro et dire que tout les sommet existe
    for (int i = 0; i <= graphe->ordre; i++) {
        degreEntrant[i] = 0;
        existe[i] = 1;
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
        //on verifie que les sommet existe pour chaque sommet on parcour tout le tableau
        for (int k = 0; k < taille; k++) {
            for (int j = 0; j < 2; ++j) {
                if (valeur[k][j] == i) {
                    existe[i] = 0;
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
//creation des stations
int station_(float ** tableau,int taille,float T ){

    //tant qu'il reste des sommet pas afficher
    int station = 2;
    float tpstotal = 0;
    int restant = 1;

    //comme les couleur sont implementer de 1 en partant de C, on recupere la plus petite couleur et on n'aura
    // qu'a augmenter de 1

    while(restant == 1) {

        //on parcour tout le tableau
        for (int i = 0; i <= taille; i++) {
            //l'ajout d'un sommet a la station respecte le temps de cycle
            if (tableau[i][2] + tpstotal <= T) {
                //sommet existe et pas encore placer
                if (tableau[i][3] == 0) {
                    //le sommet a la meme couleur ou est noir
                        //on place le sommet dans la station
                        tableau[i][3] = station;
                        //on implement le temps total de la station
                        tpstotal += tableau[i][2];

                }
            }
        }
        //on remet le temps total a 0
        tpstotal = 0;

        //on regarde si il reste des sommet sans station
        restant = 0;
        for (int i = 0; i <= taille; i++) {
            if(tableau[i][3] == 0){
                restant = 1;
            }
        }
        //on cahnge de station
        station++;
    }
    return station - 2 ;
}
//affichage stations
void affichage_stat_(float ** tabl,int taille,int station ){


    printf(" \nnous avons trouver %d station :\n\n ",station);

    for (int i = 0; i < station; i++) {
        printf(" STATION %d :\n",i+1);

        for (int k = 0; k <= taille ; k++) {
            if (tabl[k][3] == i + 2) {
                printf("sommet %d \n", (int)tabl[k][0]);
            }

        }
    }

}


int temps_precedence_main(){

    char *nomfichierprede = NULL;
    char *nomfichiercycle = NULL;
    char *nomfichiertemp = NULL;


    int ** valeurprede = NULL ;
    float ** valeurtemps = NULL;

    int nb_lignesprede;
    int nb_lignetemps;

    int gr_sommetprede;
    int gr_sommettemps;

    Graphe * graphe_prede;
    size_t tailleMax = 100;  // Taille maximale du nom de fichier
    float ** temps;

/////////////la precedence //////////////////////
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

    nb_lignesprede = lecture_fichier_precedence_(nomfichierprede,&valeurprede);
    gr_sommetprede = grand_sommet_precedence_(valeurprede,nb_lignesprede);


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

    nb_lignetemps = lecture_fichier_temps_(nomfichiertemp,&valeurtemps);
    gr_sommettemps = grand_sommet_temps_(valeurtemps,nb_lignetemps);


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

    float T = lecture_fichier_cycle_(nomfichiercycle);
    printf("\ntemps de cycle = %f \n ",T);

//////////////////////////////////////////on organise les valeur recupere pour pouvoir trier en station //////////

    //tout d'abord, cree le graph des precedence
    graphe_prede = nouv_graphe_oriente(gr_sommetprede,nb_lignesprede,valeurprede);

    //on assigne les valeur de temps aux sommet
    for (int i = 0; i <= gr_sommettemps; i++) {
        graphe_prede->pSommet[i]->valeur = temps[i][1] ;
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
        tableau_de_trie[i][1] = 0;
        //ligne temps de base
        tableau_de_trie[i][2] = temps[i][1];
        //ligne existe ?
        if(tableau_de_trie[i][2]==0){
            tableau_de_trie[i][3] = 1;
        }
    }



    //on regarde qu'elle est la source du graphe de precedence.
    int source = trouverSource_(graphe_prede,valeurprede,nb_lignesprede);

    bfs_(graphe_prede,source,tableau_de_trie);

    triBulles_(tableau_de_trie,gr_sommetprede);

    //on assigne les valeur de temps aux sommet
    for (int i = 0; i <= gr_sommettemps; i++) {
        graphe_prede->pSommet[i]->valeur = temps[i][1] ;
    }
    //puis on remet les bonnes valeur dans le tableau mais sans changer l'ordre
    for (int i = 0; i <= gr_sommetprede; i++) {
        tableau_de_trie[i][2] = graphe_prede->pSommet[(int)tableau_de_trie[i][0]]->valeur;
    }




    int stat = station_(tableau_de_trie,gr_sommetprede,T);
    affichage_stat_(tableau_de_trie,gr_sommetprede,stat);

}
#endif //OPTI_LIGNE_ASSEMBLAGE_TEMPS_PRECEDENCE_H
