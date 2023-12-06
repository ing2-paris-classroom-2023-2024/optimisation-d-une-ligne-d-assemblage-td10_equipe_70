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
void triBulles_simple(float **tableau, int taille) {
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
    printf("  \n");


    // Parcours en largeur
    while (file->debut != NULL)
    {
        // Retirer un sommet de la file et l'afficher
        int sommetActuel = defiler(file);
        printf("  %d ",sommetActuel);
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
    printf("  \n");
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
// on separe en station
int station(float ** tableau,int taille){
    //tant qu'il reste des sommet pas afficher
    int station = 2;
    float couleur = (float)'c';
    float noir = 110;
    int restant = 1;

    //comme les couleur sont implementer de 1 en partant de C, on recupere la plus petite couleur et on n'aura
    // qu'a augmenter de 1

    while(restant == 1) {

        //on parcour tout le tableau
        for (int i = 0; i <= taille; i++) {

                //sommet existe et pas encore placer
                if (tableau[i][3] == 1) {
                    //le sommet a la meme couleur ou est noir
                    if (tableau[i][1] == couleur || tableau[i][1] == noir) {

                        //on place le sommet dans la station
                        tableau[i][3] = station;
                        //on implement le temps total de la station

                }
            }
        }
        //on passe a la couleur suivante
        couleur ++;

        //on regarde si il reste des sommet sans station
        restant = 0;
        for (int i = 0; i <= taille; i++) {

            if(tableau[i][3] == 1){
                restant = 1;
            }
        }
        //on cahnge de station
        station++;
    }
    /*taille = taille du tableau

    stationActuelle = 1
    sommeActuelle = 0

    Pour chaque ligne dans le tableau:
    Si la sommeActuelle + tableau[i][2] <= 30:
    // Ajouter à la somme actuelle
    sommeActuelle += tableau[i][2]
    // Associer la station dans la troisième colonne
    tableau[i][3] = stationActuelle
    Sinon:
    // Si la somme dépasse 30, passer à la station suivante
    stationActuelle += 1
    // Réinitialiser la somme actuelle à la valeur de la ligne actuelle
    sommeActuelle = tableau[i][2]
    // Associer la station dans la troisième colonne
    tableau[i][3] = stationActuelle

    // Retourner le tableau modifié
    Retourner tableau*/
    return station - 2 ;
}
//affichage stations
void affichage_stat_simple(float ** tabl,int taille,int station ){


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
    graphe_prede = nouv_graphe_oriente(gr_sommetprede,nb_lignesprede,valeurprede);

    //ensuite on fait la coloration sur le graph d'exclusion pour avoir les couleurs
    graphe_exclu = nouv_graphe_pas_oriente(gr_sommetexclu,nb_lignesexclu,valeurexclu);

     welsh_powell(graphe_exclu);

    // on rajoute les couleur obtenue grace a l'exclusion dans le graphe de precedence
    //on par du principe que les deux graphe on le meme nombre de sommet
    for (int i = 0; i <= gr_sommetprede; i++) {
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

    triBulles_simple(tableau_de_trie,gr_sommetprede);

    //on verifie si les sommet existe bien si non, on les met a 0
    for (int i = 0; i <= gr_sommetprede; i++) {
        int prede = 0;
        int exclu = 0;
        for (int j = 0; j < nb_lignesprede; j++) {
            //si le sommet exite il est a 1
            if (valeurprede[j][0] == tableau_de_trie[i][0] || valeurprede[j][1] == tableau_de_trie[i][0]) {
                prede = 1;
            }
        }
        for (int j = 0; j < nb_lignesexclu; j++) {
            if (valeurexclu[j][0] == tableau_de_trie[i][0] || valeurexclu[j][1] == tableau_de_trie[i][0]) {
                exclu = 1;
            }
        }
            //si le sommet n'a pas ete mis a 1, il n'existe dans aucun des tableau
            if(prede == 0 && exclu == 0){
                tableau_de_trie[i][3] = 0;
                printf(" le sommet %f existe pas \n",tableau_de_trie[i][0]);
            }
        }


   /*printf(" affichage du tableau de trie trier ");
    for (int i = 0; i <= gr_sommetprede; i++) {
        printf("\n");
        for (int j = 0; j < 4; j++) {
            printf(" %f ",tableau_de_trie[i][j] );
        }
    }
    printf("\n");*/


    int stat = station(tableau_de_trie,gr_sommetprede);

    affichage_stat_simple(tableau_de_trie,gr_sommetprede,stat);

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
