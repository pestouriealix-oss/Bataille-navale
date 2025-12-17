#include "librairies.h"
#include "grille.h"
#include "bateau.h"
#include"affichage.h"



Grille* initialiser(void)
{
    // 1. Allocation dynamique de la structure
    Grille* g = malloc(sizeof(Grille));

    // Sécurité : toujours vérifier si l'allocation a réussi
    if (g == NULL) {
        return NULL;
    }

    // 2. Initialisation des cases à 0 (Eau)
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
        printf("\n");// Retour à la ligne après chaque rangée
    }
}


void detruireGrille(Grille* g)
{
    free(g);
}


void afficherCoordonnees(Grille* g)
{
    // En-tête avec les colonnes
    printf("\n     A   B   C   D   E   F   G   H   I   J\n");
    printf("   +---+---+---+---+---+---+---+---+---+---+\n");
    
    for(int i = 0; i < 10; i++){
        // Numéro de ligne (aligné à droite pour 10)
        printf("%2d |", i + 1);
        
        for(int j = 0; j < 10; j++){
            int valeur = g->tab[i][j];
            
            // Affichage selon la valeur
            if (valeur == 0) {
                printf(" ~ |");  // Eau
            } else if (valeur == 1) {
                printf(VERT " ■ " RESET "|");  // Bateau
            } else if (valeur == 2) {
                printf(CIEL " ~ " RESET "|");  // Raté
            } else if (valeur == 3) {
                printf(ROUGE " X " RESET "|");   // Touché
            } else {
                printf(" %d |", valeur);  // Autre valeur
            }
        }
        
        printf("\n   +---+---+---+---+---+---+---+---+---+---+\n");
    }
}

void afficherMasque(Grille* g)
{
    // 1. En-tête (A-J)
    printf("  ");
    for(int j = 0; j < 10; j++){
        printf("%c ", 'A' + j);
    }
    printf("\n");
    
    // 2. Contenu masqué
    for(int i = 0; i < 10; i++){
        printf("%2d ", i + 1); // Numéro de ligne
        
        for(int j = 0; j < 10; j++){
            // LOGIQUE DE MASQUAGE :
            // Si la case contient un bateau intact (1), on affiche de l'eau (0)
            // pour ne pas tricher. Sinon, on affiche la vraie valeur (2 ou 3).
            if(g->tab[i][j] == 1){
                printf("0 "); 
            } else {
                printf("%d ", g->tab[i][j]);
            }
        }
        printf("\n");
    }
}


int tirer(Grille* g, int ligne, int colonne)
{
    
    // 1. Vérification des bornes (Sécurité)
    if (ligne < 0 || ligne > 9 || colonne < 0 || colonne > 9){
        return -2; // Erreur : Hors de la grille
    }
    
    // 2. Vérification si la case a déjà été jouée
    // 2 = Tir manqué précédent, 3 = Bateau touché précédent
    else if (g->tab[ligne][colonne] == 2 || g->tab[ligne][colonne] == 3){
        return -1; // Erreur : Déjà tiré ici
    }
    
    // 3. Cas : Bateau touché !
    else if (g->tab[ligne][colonne] == 1){
        g->tab[ligne][colonne] = 3; // Mise à jour de l'état vers "Touché"
        return 1; // Succès : Touché
    }
    
    // 4. Cas : Tir dans l'eau
    else {
        g->tab[ligne][colonne] = 2; // Mise à jour de l'état vers "Raté"
        return 0; // Succès : Raté
    }
}
