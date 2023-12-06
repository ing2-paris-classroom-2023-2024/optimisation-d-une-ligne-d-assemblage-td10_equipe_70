#include "base_graphe.h"
#include "exclusion.h"
#include "temps + precedence.h"
#include "exclusion + temps.h"
#include "precedence + exclusion .h"
#include "toute_contrainte.h"

int main() {
    system("cls");

    int choix = 10 ;

    printf (" ____________Bien venue dans le menu de notre projet____________ \n\n");
    printf("il existe plusieurs facon d'optimiser la ligne d'assemblage : \n");
    printf(" Voulez vous utiliser une contraint d'exclusion ? (1)\n");
    printf(" Voulez vous utiliser une contraint de precedence ainsi qu'un temp de cycle ? (2)\n");
    printf(" Voulez vous utiliser une contraint d'exclusion et de precedence ? (3)\n");
    printf(" Voulez vous utiliser une contraint d'exclusion et de temps de cycle ? (4)\n");
    printf(" voulez vous quitter ? (0)\n");

    while(choix<0 || choix>5) {
        scanf("%d", &choix);
        fflush(stdin);
    }

    switch(choix){
        case 1 :
            printf("vous avez choisi l'exclusion \n");

            exclusion_main();
            break;

        case 2 :
            printf("vous avez choisi precedence + temps de cycle \n");

            temps_precedence_main();
            break;

        case 3 :
            printf("vous avez choisi exclusion + precedence \n");

            exclusion_precedence_main();
            break;

        case 4 :
            printf("vous avez choisi exclusion + temps de cycle\n");

            exclusion_temps_main();
            break;

        case 5 :
            printf("vous avez choisi toutes les contraites \n");

            toute_contraint();
            break;


        case 0 :
            printf("vous avez choisi de nous quitter \n");
            printf(" passez une bonne journee \n ");
            return 0;

    }




    return 0;
}
