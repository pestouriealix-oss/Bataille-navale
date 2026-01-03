#include "librairies.h"
#include "grille.h"
#include "bateau.h"
#include "affichage.h"

/* ============================================================================
 * 1. INITIALISATION ET MÉMOIRE
 * ============================================================================ */

/* Crée une grille vide remplie d'eau (0) */
Grille* initialiser(void)
{
    // 1. Allocation dynamique
    Grille* g = malloc(sizeof(Grille));

    // Sécurité : toujours vérifier si l'allocation a réussi
    if (g == NULL) {
        fprintf(stderr, "Erreur fatale : Allocation mémoire impossible pour la grille.\n");
        return NULL;
    }

    // 2. Initialisation à 0
    for(int i = 0; i < 10; i++){
        for(int j = 0; j < 10; j++){
            g->tab[i][j] = 0;
        }
    }

    return g;
}

/* Libère la mémoire */
void detruireGrille(Grille* g)
{
    if (g != NULL) {
        free(g);
    }
}


/* ============================================================================
 * 2. AFFICHAGE
 * ============================================================================ */

/* Affichage brut (pour débugger uniquement) */
void afficher(Grille* g)
{
    for(int i = 0; i < 10; i++){
        for(int j = 0; j < 10; j++){
            printf("%d ", g->tab[i][j]);
        }
        printf("\n");
    }
}

/* Affichage COMPLET (Ma propre grille) : On voit ses bateaux */
void afficherCoordonnees(Grille* g)
{
    // En-tête des colonnes (A-J)
    printf("\n    A  B  C  D  E  F  G  H  I  J\n");
    
    // Bordure supérieure
    printf("   ┌──────────────────────────────┐\n");

    for(int i = 0; i < 10; i++){
        // Numéro de ligne (1-10) aligné
        printf("%2d │", i + 1);

        for(int j = 0; j < 10; j++){
            int val = g->tab[i][j];

            // --- RENDU GRAPHIQUE ---
            if (val == 0) { 
                // EAU : Fond Bleu + Vague cyan
                printf(BG_BLEU CYAN " ~ " RESET); 
            } 
            else if (val == 1) { 
                // BATEAU : Fond Vert + Carré
                printf(BG_VERT BLANC " ■ " RESET); 
            } 
            else if (val == 2) { 
                // RATÉ : Fond Noir + Rond gris
                printf(BG_NOIR BLANC " o " RESET); 
            } 
            else if (val == 3) { 
                // TOUCHÉ : Fond Rouge + Croix
                printf(BG_ROUGE JAUNE " X " RESET); 
            }
        }
        printf("│\n"); // Fin de ligne avec bordure droite
    }
    
    // Bordure inférieure
    printf("   └──────────────────────────────┘\n");
}

/* Affichage MASQUÉ (Grille adverse) : On cache les bateaux intacts */
void afficherMasque(Grille* g)
{
    // En-tête des colonnes
    printf("\n    A  B  C  D  E  F  G  H  I  J\n");
    
    // Bordure supérieure
    printf("   ┌──────────────────────────────┐\n");

    for(int i = 0; i < 10; i++){
        // Numéro de ligne
        printf("%2d │", i + 1);

        for(int j = 0; j < 10; j++){
            int val = g->tab[i][j];

            // --- RENDU GRAPHIQUE MASQUÉ ---
            
            // Si c'est un BATEAU (1), on ment et on affiche de l'EAU (0)
            if (val == 1) {
                // On affiche comme de l'eau pour cacher le bateau
                printf(BG_BLEU CYAN " ~ " RESET); 
            }
            // Sinon (Eau, Raté, Touché), on affiche normalement
            else if (val == 0) { 
                printf(BG_BLEU CYAN " ~ " RESET); 
            } 
            else if (val == 2) { 
                printf(BG_NOIR BLANC " o " RESET); 
            } 
            else if (val == 3) { 
                printf(BG_ROUGE JAUNE " X " RESET); 
            }
        }
        printf("│\n");
    }
    
    // Bordure inférieure
    printf("   └──────────────────────────────┘\n");
}


/* ============================================================================
 * 3. ACTIONS DE JEU
 * ============================================================================ */

int tirer(Grille* g, int ligne, int colonne)
{
    // 1. Vérification des bornes (Sécurité anti-crash)
    if (ligne < 0 || ligne > 9 || colonne < 0 || colonne > 9){
        return -2; // Erreur : Hors de la grille
    }
    
    // 2. Vérification si la case a déjà été jouée
    // (2 = Raté, 3 = Touché)
    else if (g->tab[ligne][colonne] == 2 || g->tab[ligne][colonne] == 3){
        return -1; // Erreur : Déjà tiré ici
    }
    
    // 3. Cas : Bateau touché !
    else if (g->tab[ligne][colonne] == 1){
        g->tab[ligne][colonne] = 3; // On marque la case comme "Touchée"
        return 1; // Succès
    }
    
    // 4. Cas : Tir dans l'eau
    else {
        g->tab[ligne][colonne] = 2; // On marque la case comme "Ratée"
        return 0; // Échec
    }
}
