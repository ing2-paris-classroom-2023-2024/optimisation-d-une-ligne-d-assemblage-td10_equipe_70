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
int trouverSource(Graphe* graphe,float ** valeur) {
    // Tableau pour stocker le degré entrant de chaque sommet
    int degreEntrant[graphe->ordre+1];
    int existe[graphe->ordre+1] ;

    // Initialiser tous les degrés entrants à zéro et dire que tout les sommet existe
    for (int i = 0; i <= graphe->ordre; i++) {
        degreEntrant[i] = 0;
        existe[i] = 0;
        //printf(" %d %d ",degreEntrant[i],existe[i] );
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
        //printf(" sommet %d degre = %d \n",i,degrer);
    }
    for (int i = 0; i <= graphe->ordre; i++) {
        if(valeur[i][1] == 0.000000){
            existe[i] = 1;
            printf(" sommet %d existe pas \n",i);
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

    /*// Afficher les valeurs stockées dans le tableau
    for (int i = 0; i < nombreDeLignes; i++) {
        printf("Ligne %d : %f %f\n", i + 1, (*valeur)[i][0], (*valeur)[i][1]);
    }
*/
    fclose(fichier);
    return nombreDeLignes;
    //dans cette fonction on récupere les données sous la forme d'un tableau ainsi que
    //le nombre de ligne!! pas egale au nombre d'operation pour l'exclusion mais egale au
    //nombre d'arrete, une arrete egale uen exculsion
    //le programme compte toute les lignes meme les lignes vide !!!!!!!!!

}
//lecture fichier temps de cycle
float lecture_fichier_cycle( char *nomfichier){
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
int grand_sommet_temps(float ** valeur, int lignes ){

    int grand_sommet = 0;

    for (int i = 0; i < lignes; i++) {
            if( valeur[i][0] > grand_sommet){
                grand_sommet = valeur[i][0];

        }
    }
    return grand_sommet;

}
//trouve la valeur max parmis les predecesseru
float trouverValeurMaxPredecesseurs(Graphe* graphe, int i) {
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
// Fonction pour effectuer un parcours en largeur (BFS) sur le graphe
float** bfs(Graphe* graphe, int sommetDepart,float **tableau )
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
        //printf(" on passe au sommet %d\n\n",sommetActuel);
        //trouver la valeur max parmis les predecesseur et l'ajouter au sommet
        if(sommetActuel != sommetDepart) {
            float val = trouverValeurMaxPredecesseurs(graphe, sommetActuel);
            //graphe->pSommet[sommetActuel]->valeur += val;
            //printf(" la valeur du sommet %d est mises a jouor avec %f\n",sommetActuel,val);
            tableau[sommetActuel][2] = graphe->pSommet[sommetActuel]->valeur+ val;
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
//trier les tableau
void triBulles(float **tableau, int taille) {
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
//creation des stations
int stations(float ** tableau,int taille,float T ){

    //tant qu'il reste des sommet pas afficher
    int station = 2;
    float couleur = (float)'c';
    float noir = 110;
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
                    if (tableau[i][1] == couleur || tableau[i][1] == noir) {
                            //on place le sommet dans la station
                            tableau[i][3] = station;
                            printf(" sommet %d = station %f\n ", i,tableau[i][3]);
                            //on implement le temps total de la station
                            tpstotal += tableau[i][2];
                        }
                    }
                }

        }
        //on passe a la couleur suivante
        char temp = (char)couleur ;
        couleur = (float )(temp++) ;
        //on remet le temps total a 0
        printf(" temps totatl station %d = %f ",station,tpstotal);
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
    return station - 1 ;
}
//affichage stations
void affichage_station(float ** tabl,int taille ){


    printf(" nous avons trouver %d station :\n ");
    printf(" station %d \n");
    for (int i = 0; i <= taille; i++) {
        printf(" %f\n ");
    }
}

int toute_contraint(){
    char *nomfichierexclu = NULL;
    char *nomfichierprede = NULL;
    char *nomfichiertemp = NULL;
    char *nomfichiercycle = NULL;

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
    int source;
    float ** temps;

    float** tableau_de_trie ;
        ///////////////////on cherche tout dabord a recuperer toutes les valeurs ///////////////////////////////////////


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

    nb_lignetemps = lecture_fichier_temps(nomfichiertemp,&valeurtemps);
    gr_sommettemps = grand_sommet_temps(valeurtemps,nb_lignetemps);


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
        //printf(" %f %f \n",temps[i][0],temps[i][1]);
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

    float T = lecture_fichier_cycle(nomfichiercycle);
    printf("\n %f \n ",T);

    ///////////////////////////////////////////on organise les valeur recupere pour pouvoir trier en station //////////
    //on transvase les valeur dans un tableau ou chaque case represente un sommet, et on les a tous meme ceux qui
    //n'existe pas

    //tout d'abord, cree le graph des precedence
    graphe_prede = nouv_graphe_oriente(gr_sommetprede,nb_lignesprede,valeurprede);

    //ensuite on fait la coloration sur le graph d'exclusion pour avoir les couleurs
    graphe_exclu = nouv_graphe_pas_oriente(gr_sommetexclu,nb_lignesexclu,valeurexclu);
    welsh_powell(graphe_exclu);
    // on rajoute les couleur obtenue grace a l'exclusion dans le graphe de precedence
    //on par du principe que les deux graphe on le meme nombre de sommet
   // printf(" voici les couleur des sommet \n");
    for (int i = 0; i <= gr_sommetprede; i++) {
        graphe_prede->pSommet[i]->couleur = graphe_exclu->pSommet[i]->couleur;
        //printf(" sommet %d = %c \n ",i,graphe_exclu->pSommet[i]->couleur);
    }
    //on assigne les valeur de temps aux sommet
    //printf(" voici les valeur temp des sommet \n");
    for (int i = 0; i <= gr_sommettemps; i++) {
            graphe_prede->pSommet[i]->valeur = temps[i][1] ;
        //printf(" sommet %d = %f \n ",i,graphe_prede->pSommet[i]->valeur);
    }

    //regarder dans le tableau de operation, les valeur qui sont a O si 0 alors ca n'existe pas donc par consequent on
    // transmet ca dans trouver sources

    //maintenant, nous allons parcourir le graph de precedence pour assigner les valeur de prioriter des operation
    //cad on fait un pert modifier avec les temps les plus tot
    //pour choisir le sommet de départ du BFS, on trouve la source du graph

    ///////////////on initialise le tableau de trie pour obtenir un table de ordre colones et 4 lignes/////////////////
    //un ligne sommet
    //une ligne temps
    //une ligne couleur
//alocation dynamique
    tableau_de_trie = (float**)malloc((gr_sommetprede+1) * sizeof(float*));
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
        tableau_de_trie[i][2] = temps[i][1];
        //ligne existe ?
        if(tableau_de_trie[i][2]==0){
            tableau_de_trie[i][3] = 1;
        }    }
/*/affichage du tableau
    printf(" affichage du tableau de trie ");
    for (int i = 0; i <= gr_sommetprede; i++) {
        printf("\n");
        for (int j = 0; j < 4; j++) {
            printf(" %f ",tableau_de_trie[i][j] );
        }
        printf("\n");
    }
*/
    ////////////////////////////on commence a trier les valeur /////////////////////////////////////////////////////////
    source = trouverSource(graphe_prede,temps);

    bfs(graphe_prede,source,tableau_de_trie);
    //!!!!!!!ne pas oublier de modifier pour que les val soit uniquement dans le tableau et que les sommet
    ///conserve leur valeur  de base
    triBulles(tableau_de_trie,gr_sommetprede);

//affichage du tableau rempllie avec tout ce qu'il faut et trier en fonction du temps de cycle
    printf(" affichage du tableau de trie trier ");
    for (int i = 0; i <= gr_sommetprede; i++) {
        printf("\n");
        for (int j = 0; j < 4; j++) {
            printf(" %f ",tableau_de_trie[i][j] );
        }
    }
    printf("\n");

    //on separe en station et recupere le nombre de station
    int stat = stations(tableau_de_trie,gr_sommetprede,T);
    printf(" il y a %d stations \n",stat);
    //on affiche les stations
    affichage_station(tableau_de_trie,stat);

///refaire le programme de trie des station

    for (int i = 0; i <= gr_sommettemps; i++) {
        free(temps[i]);
    }
    free(temps);
}
#endif //OPTI_LIGNE_ASSEMBLAGE_TOUTE_CONTRAINTE_H
