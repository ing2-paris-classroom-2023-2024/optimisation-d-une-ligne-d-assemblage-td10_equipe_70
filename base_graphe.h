//
// Created by spinner on 30/11/2023.
//
#include <stdio.h>
#include "string.h"
#include "stdlib.h"
#include "unistd.h"

#ifndef OPTI_LIGNE_ASSEMBLAGE_BASE_GRAPHE_H
#define OPTI_LIGNE_ASSEMBLAGE_BASE_GRAPHE_H
//////////////les structures ( liste chainer ) ///////////////////

/* Alias de pointeur sur un Arc */
typedef struct Arc* pArc;
/* Alias de pointeur sur un Sommet */
typedef struct Sommet* pSommet;


/* Structure d'un arc*/
struct Arc
{
    int sommet; // numéro de sommet d'un arc adjacent au sommet initial
    int poid;
    struct Arc* arc_suivant; //liste chainer d'arc
};
/* Structure d'un sommet*/
struct Sommet
{
    struct Arc* arc; //liste des arcs adjacents
    int valeur;
    char couleur;

};
/* structure d'un Graphe */
typedef struct Graphe
{
    int taille;
    int ordre;
    pSommet* pSommet;//tableau de pointeur de sommet
} Graphe;

///////////////////////////liste des fonctions de bases  ///////////////////////////////
// créer le graphe pouvant contenire ordre sommet
Graphe* CreerGraphe(int ordre);
//creation d'un nouveau graph
Graphe * nouv_graphe_pas_oriente(int ordre,int taille ,int **valeur);
Graphe * nouv_graphe_oriente(int ordre,int taille ,int **valeur);
// Ajouter l'arête entre les sommets s1 et s2 du graphe
pSommet* CreerArete(pSommet* sommet,int s1,int s2);
// affichage des successeurs du sommet num
void afficher_successeurs(pSommet * sommet, int num);
//affichage du graphe avec les successeurs de chaque sommet
void afficher_graph(Graphe* graphe,int grand_sommet);

/////////////////////////Les fonciton de bases ////////////////////////

// affichage des successeurs du sommet num
void afficher_successeurs(pSommet * sommet, int num)
{

    printf(" sommet %d :\n",num);

    pArc arc=sommet[num]->arc;

    while(arc!=NULL)
    {
        printf("%d ",arc->sommet);
        arc=arc->arc_suivant;
    }

}
// Ajouter l'arête entre les sommets s1 et s2 du graphe
pSommet* CreerArete(pSommet* sommet,int s1,int s2)
{
    if(sommet[s1]->arc==NULL)
    {
        pArc Newarc=(pArc)malloc(sizeof(struct Arc));
        Newarc->sommet=s2;
        Newarc->arc_suivant=NULL;
        sommet[s1]->arc=Newarc;
        return sommet;
    }

    else
    {
        pArc temp=sommet[s1]->arc;
        while( !(temp->arc_suivant==NULL))
        {
            temp=temp->arc_suivant;
        }
        pArc Newarc=(pArc)malloc(sizeof(struct Arc));
        Newarc->sommet=s2;
        Newarc->arc_suivant=NULL;

        if(temp->sommet>s2)
        {
            Newarc->arc_suivant=temp->arc_suivant;
            Newarc->sommet=temp->sommet;
            temp->sommet=s2;
            temp->arc_suivant=Newarc;
            return sommet;
        }

        temp->arc_suivant=Newarc;
        return sommet;
    }
}
// créer le graphe
Graphe* CreerGraphe(int ordre)
{
    Graphe * Newgraphe=(Graphe*)malloc(sizeof(Graphe));
    Newgraphe->pSommet = (pSommet*)malloc(ordre*sizeof(pSommet));

    for(int i=0; i<= ordre; i++)
    {
        Newgraphe->pSommet[i]=(pSommet)malloc(sizeof(struct Sommet));
        Newgraphe->pSommet[i]->valeur=i;
        Newgraphe->pSommet[i]->arc=NULL;
        Newgraphe->pSommet[i]->couleur = 'b';
    }
    return Newgraphe;
}
//nouveau graph grace au tableau et au valeur recupere dans les fichier
Graphe * nouv_graphe_pas_oriente(int ordre,int taille ,int **valeur ) {
    Graphe *graphe;
///est ce que l'ordre doit etre le nombre de sommet different ou le sommet le plus elever ?
    graphe = CreerGraphe(ordre); // créer le graphe d'ordre sommets

    //on initialise le graph a partir des donner extraites
    graphe->ordre = ordre;
    graphe->taille = taille;

    // créer les arêtes du graphe depuis les valeurs du tableau
    for (int i = 0; i < taille; i++) {
        graphe->pSommet = CreerArete(graphe->pSommet, valeur[i][0],valeur[i][1]);
        graphe->pSommet = CreerArete(graphe->pSommet, valeur[i][1],valeur[i][0]);
    }
    return graphe;
}
//meme fonction sauf que on cree que 1 arrete au lieu de deux
Graphe * nouv_graphe_oriente(int ordre,int taille ,int **valeur ) {
    Graphe *graphe;
///est ce que l'ordre doit etre le nombre de sommet different ou le sommet le plus elever ?
    graphe = CreerGraphe(ordre); // créer le graphe d'ordre sommets

    //on initialise le graph a partir des donner extraites
    graphe->ordre = ordre;
    graphe->taille = taille;

    // créer les arêtes du graphe depuis les valeurs du tableau
    for (int i = 0; i < taille; i++) {
        graphe->pSommet = CreerArete(graphe->pSommet, valeur[i][0],valeur[i][1]);
    }
    return graphe;
}
//affichage du graphe avec les successeurs de chaque sommet
void afficher_graph(Graphe* graphe,int grand_sommet)
{
    printf("graphe\n");

    printf("non oriente\n");

    printf("ordre = %d\n",graphe->ordre);

    printf("listes d'adjacence :\n");
///ici on met soit le plus grand sommet et donc on affiche 30 sommet ou on met l'ordre et on affiche seulemetn le nombre
///de sommet differents soit un peut moins ou beaucoup moins en fonction des graphs
    for (int i=0; i <= grand_sommet; i++)
    {
        afficher_successeurs(graphe->pSommet, i);
        printf("\n");
    }

}


#endif //OPTI_LIGNE_ASSEMBLAGE_BASE_GRAPHE_H
