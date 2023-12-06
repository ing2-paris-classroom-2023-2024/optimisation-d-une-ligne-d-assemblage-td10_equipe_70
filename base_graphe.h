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
    float poid;
    struct Arc* arc_suivant; //liste chainer d'arc
};
/* Structure d'un sommet*/
struct Sommet
{
    struct Arc* arc_sortant; //liste des arcs adjacents
    struct Arc* arc_entrant;
    float valeur;
    char couleur;
    int marque ;

};
/* structure d'un Graphe */
typedef struct Graphe
{
    int taille;
    int ordre;
    pSommet* pSommet;//tableau de pointeur de sommet
} Graphe;
/*
// File utilisée pour le BFS
typedef struct File {
    int* tableau;
    int taille;
    int capacite;
    int debut;
    int fin;
} File;
*/
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

///////////////////////liste des fonction de la file //////////////////////////:::
// Structure pour représenter un élément de la file
typedef struct QueueNode
{
    int sommet;
    struct QueueNode* suivant;
} QueueNode;

// Structure pour représenter une file
typedef struct Queue
{
    QueueNode* debut;
    QueueNode* fin;
} Queue;

// Fonction pour initialiser une file vide
Queue* initQueue()
{
    Queue* file = (Queue*)malloc(sizeof(Queue));
    file->debut = file->fin = NULL;
    return file;
}

// Fonction pour ajouter un sommet à la file
void enfiler(Queue* file, int sommet)
{
    QueueNode* nouveauSommet = (QueueNode*)malloc(sizeof(QueueNode));
    nouveauSommet->sommet = sommet;
    nouveauSommet->suivant = NULL;

    if (file->fin == NULL)
    {
        file->debut = file->fin = nouveauSommet;
        return;
    }

    file->fin->suivant = nouveauSommet;
    file->fin = nouveauSommet;
}

// Fonction pour retirer un sommet de la file
int defiler(Queue* file)
{
    if (file->debut == NULL)
        return -1; // File vide

    int sommet = file->debut->sommet;
    QueueNode* temp = file->debut;

    file->debut = file->debut->suivant;

    if (file->debut == NULL)
        file->fin = NULL;

    free(temp);
    return sommet;
}

/////////////////////////Les fonciton de bases ////////////////////////

// affichage des successeurs du sommet num
void afficher_successeurs(pSommet * sommet, int num)
{

    printf(" sommet %d :\n",num);

    pArc arc=sommet[num]->arc_sortant;

    while(arc!=NULL)
    {
        printf("%d ",arc->sommet);
        arc=arc->arc_suivant;
    }

}
void afficher_predecesseur(pSommet * sommet, int num)
{

    printf(" sommet %d :\n",num);

    pArc arc=sommet[num]->arc_entrant;

    while(arc!=NULL)
    {
        printf("%d ",arc->sommet);
        arc=arc->arc_suivant;
    }

}
// Ajouter arc sortant (succeseur) s1 s2
pSommet* CreerArete_sortant(pSommet* sommet,int s1,int s2)
{
    if(sommet[s1]->arc_sortant==NULL)
    {
        pArc Newarc=(pArc)malloc(sizeof(struct Arc));
        Newarc->sommet=s2;
        Newarc->arc_suivant=NULL;
        sommet[s1]->arc_sortant=Newarc;
        return sommet;
    }

    else
    {
        pArc temp=sommet[s1]->arc_sortant;
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
//ajouter arc entrant ( predecesseur) S2 S1
pSommet* CreerArete_entrant(pSommet* sommet,int s1,int s2)
{
    if(sommet[s1]->arc_entrant==NULL)
    {
        pArc Newarc=(pArc)malloc(sizeof(struct Arc));
        Newarc->sommet=s2;
        Newarc->arc_suivant=NULL;
        sommet[s1]->arc_entrant=Newarc;
        return sommet;
    }

    else
    {
        pArc temp=sommet[s1]->arc_entrant;
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
        Newgraphe->pSommet[i]->arc_entrant=NULL;
        Newgraphe->pSommet[i]->arc_sortant=NULL;
        Newgraphe->pSommet[i]->couleur = 'b';
        Newgraphe->pSommet[i]->marque = 0;
    }
    return Newgraphe;
}
//nouveau graph grace au tableau et au valeur recupere dans les fichier
Graphe * nouv_graphe_pas_oriente(int ordre,int taille ,int **valeur ) {
    Graphe *graphe;
    graphe = CreerGraphe(ordre); // créer le graphe d'ordre sommets

    //on initialise le graph a partir des donner extraites
    graphe->ordre = ordre;
    graphe->taille = taille;

    // créer les arêtes du graphe depuis les valeurs du tableau
    for (int i = 0; i < taille; i++) {
        graphe->pSommet = CreerArete_sortant(graphe->pSommet, valeur[i][0],valeur[i][1]);
        graphe->pSommet = CreerArete_sortant(graphe->pSommet, valeur[i][1],valeur[i][0]);
    }
    return graphe;
}
//meme fonction sauf que on cree que 1 arrete au lieu de deux
Graphe * nouv_graphe_oriente(int ordre,int taille ,int **valeur ) {

    Graphe *graphe;
    graphe = CreerGraphe(ordre); // créer le graphe d'ordre sommets

    //on initialise le graph a partir des donner extraites
    graphe->ordre = ordre;
    graphe->taille = taille;

    // créer les arêtes du graphe depuis les valeurs du tableau
    for (int i = 0; i < taille; i++) {
        graphe->pSommet = CreerArete_sortant(graphe->pSommet, valeur[i][0],valeur[i][1]);
        graphe->pSommet = CreerArete_entrant(graphe->pSommet, valeur[i][1],valeur[i][0]);
    }
    return graphe;
}
//affichage du graphe avec les successeurs de chaque sommet
void afficher_graph(Graphe* graphe,int grand_sommet)
{
    printf("ordre = %d\n",graphe->ordre);

    printf("listes d'adjacence :\n");
    for (int i=0; i <= grand_sommet; i++)
    {
        afficher_successeurs(graphe->pSommet, i);
        printf("\n");
    }
    printf("listes de precedence :\n");
    for (int i=0; i <= grand_sommet; i++)
    {
        afficher_predecesseur(graphe->pSommet, i);
        printf("\n");
    }

}


#endif //OPTI_LIGNE_ASSEMBLAGE_BASE_GRAPHE_H
