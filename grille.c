#include "librairies.h"
#include "grille.h"
#include "bateau.h"

Grille* initialiser(void)
{
    Grille* g = malloc(sizeof(Grille));
    for(int i = 0; i < 10; i++){
        for(int j = 0; j < 10; j++){
            g->tab[i][j] = 0;
        }
    }
    return g;
}

void afficher(Grille* g)
{
    for(int i = 0; i < 10; i++){
        for(int j = 0; j < 10; j++){
            printf("%d ", g->tab[i][j]);
        }
        printf("\n");
    }
}

void detruireGrille(Grille* g){
    free(g);
}

void afficherCoordonnees(Grille* g)
{
    printf("  ");
    for(int j = 0; j < 10; j++){
        printf("%c ", 'A' + j);
    }
    printf("\n");
    
    for(int i = 0; i < 10; i++){
        printf("%d ", i + 1);  
        
        for(int j = 0; j < 10; j++){
            printf("%d ", g->tab[i][j]);
        }
        printf("\n");
    }
}

void afficherMasque(Grille* g)
{
    // En-tête
    printf("  ");
    for(int j = 0; j < 10; j++){
        printf("%c ", 'A' + j);
    }
    printf("\n");
    
    // Afficher ligne par ligne
    for(int i = 0; i < 10; i++){
        printf("%d ", i + 1);
        for(int j = 0; j < 10; j++){
            // MASQUER les bateaux non touchés
            if(g->tab[i][j] == 1){
                printf("0 ");  // Afficher 0 au lieu de 1
            } else {
                printf("%d ", g->tab[i][j]);  // Afficher normalement
            }
        }
        printf("\n");
    }
}

int tirer(Grille* g, int ligne, int colonne){
    if (ligne < 0 || ligne > 10 || colonne < 0 || colonne > 10){
        return -2;
    }
    else if (g->tab[ligne][colonne] == 2 || g->tab[ligne][colonne] == 3){
        return -2;
    }
    else if (g->tab[ligne][colonne] == 1){
        g->tab[ligne][colonne] = 3;
        return 1;
    }
    else {
        g->tab[ligne][colonne] = 2;
        return 0;
    }
}
