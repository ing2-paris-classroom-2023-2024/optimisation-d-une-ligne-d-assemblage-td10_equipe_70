# optimisation-d-une-ligne-d-assemblage-td10_equipe_70
optimisation-d-une-ligne-d-assemblage-td10_equipe_70 created by GitHub Classroom
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Operation {
    char nom[50];

} Operation;


typedef struct Graphe {
    int taille;
    Operation* operations;
    int** matriceAdjacence;
} Graphe;

Graphe* initialiserGraphe(int taille) {
    Graphe* graphe = (Graphe*)malloc(sizeof(Graphe));
    graphe->taille = taille;

    graphe->operations = (Operation*)malloc(taille * sizeof(Operation));

    graphe->matriceAdjacence = (int**)malloc(taille * sizeof(int*));
    for (int i = 0; i < taille; ++i) {
        graphe->matriceAdjacence[i] = (int*)malloc(taille * sizeof(int));
        for (int j = 0; j < taille; ++j) {
            graphe->matriceAdjacence[i][j] = 0;
        }
    }

    return graphe;
}
