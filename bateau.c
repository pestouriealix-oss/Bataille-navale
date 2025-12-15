#include "librairies.h"
#include "grille.h"
#include "bateau.h"

Bateaux* creerFlotte(void)
{
    Bateaux* flotte = malloc(NB_BATEAUX * sizeof(Bateaux));

    //Porte-avion
    strcpy(flotte[0].nom, "Porte-avion");
    flotte[0].taille = 5;
    flotte[0].touche = 0;

    //Croiseur
    strcpy(flotte[1].nom, "Croiseur");
    flotte[1].taille = 4;
    flotte[1].touche = 0;

    //Contre torpilleurs
    strcpy(flotte[2].nom, "Contre-torpilleur");
    flotte[2].taille = 3;
    flotte[2].touche = 0;

    //Contre torpilleurs
    strcpy(flotte[3].nom, "Contre-torpilleur");
    flotte[3].taille = 3;
    flotte[3].touche = 0;

    //Torpilleur
    strcpy(flotte[4].nom, "Torpilleur");
    flotte[4].taille = 2;
    flotte[4].touche = 0;

    return flotte;
 
}

void detruireFlotte (Bateaux* flotte)
{
    free(flotte);
}

Bateaux* creerBateaux(void)
{
    Bateaux* b = malloc(sizeof(Bateaux));
    strcpy(b->nom, "Vide");
    b->taille = 0;
    b->ligne = 0;
    b->colonne = 0;
    b->orientation = '\0';
    b->touche = 0;

    return b;
}

void detruireBateaux(Bateaux* b)
{
    free(b);
}

int positionnerBateau(Bateaux* b, Grille* g)
{
    int ligne, colonne;
    char orientation;

    while (1) 
    {
        printf("\n--- Placement du %s (Taille %d) ---\n", b->nom, b->taille);
        printf("Entrez la ligne (1-10) ou '-1' pour quitter : ");
        
        scanf("%d", &ligne);
        
        // VÉRIFIER -1 IMMÉDIATEMENT
        if (ligne == -1) {
            // Nettoyer le buffer
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
            return 0;
        }
        
        printf("Entrez la colonne (1-10) : ");
        scanf("%d", &colonne);

        printf("Orientation (H/V) : ");
        scanf(" %c", &orientation);
        
        // Nettoyer le buffer d'entrée
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
        
        // Convertir en majuscule
        if (orientation == 'h') orientation = 'H';
        if (orientation == 'v') orientation = 'V';
        
        // Convertir en indices 0-9
        ligne = ligne - 1;
        colonne = colonne - 1;

        // --- VALIDATION ---
        
        // Vérifier orientation valide
        if (orientation != 'H' && orientation != 'V') {
            printf("❌ Erreur : Orientation invalide ! Utilisez H ou V.\n");
            continue;
        }

        // Erreur 1 : Dépassement
        if ((orientation == 'H' && (colonne + b->taille > 10 || colonne < 0 || ligne < 0 || ligne >= 10)) ||
            (orientation == 'V' && (ligne + b->taille > 10 || ligne < 0 || colonne < 0 || colonne >= 10))) {
            
            printf("❌ Erreur : Le bateau sort de la grille ! Réessayez.\n");
            continue;
        }

        // Erreur 2 : Collision
        int collision = 0;
        for (int i = 0; i < b->taille; i++) {
            if (orientation == 'H' && g->tab[ligne][colonne + i] == 1) collision = 1;
            if (orientation == 'V' && g->tab[ligne + i][colonne] == 1) collision = 1;
        }

        if (collision == 1) {
            printf("❌ Erreur : Il y a déjà un bateau ici ! Réessayez.\n");
            continue;
        }

        // --- SUCCÈS ---
        for (int i = 0; i < b->taille; i++) {
            if (orientation == 'H') g->tab[ligne][colonne + i] = 1;
            else g->tab[ligne + i][colonne] = 1;
        }

        b->ligne = ligne;
        b->colonne = colonne;
        b->orientation = orientation;

        printf("✅ %s placé avec succès !\n", b->nom);
        return 1;
    }
}

void enregistrerTouche(Bateaux* b)
{
 b->touche = b->touche + 1;
}

int estCoule(Bateaux* b)
{
    if(b->touche == b->taille)
    {
        return 1; // Oui
    }
    else 
    {
        return 0;
    }
}

int estTouche(Bateaux* b)
{
    if(b->touche > 0 && b->touche < b->taille)
    {
        return 1; //Oui
    }
    else 
    {
        return 0;
    }
}

char obtenirEtat(Bateaux* b)
{
    if (estCoule(b) == 1)
    {
        return 'C';
    }
    else if (estTouche(b) == 1)
    {
        return 'T';
    }
    else 
    {
        return 'O';
    }
}

int appartientAuBateau(Bateaux* b, int ligne, int colonne)
{
    // Bateau horizontal
    if (b->orientation == 'H') {
        if (ligne == b->ligne && 
            colonne >= b->colonne && 
            colonne < b->colonne + b->taille) {
            return 1;  // Oui, cette position fait partie du bateau
        }
    }
    
    // Bateau vertical
    if (b->orientation == 'V') {
        if (colonne == b->colonne && 
            ligne >= b->ligne && 
            ligne < b->ligne + b->taille) {
            return 1;  // Oui, cette position fait partie du bateau
        }
    }
    
    return 0;  // Non, pas partie du bateau
}

int tousCoules(Bateaux* flotte)
{
    // Vérifier si TOUS les bateaux sont coulés
    for (int i = 0; i < NB_BATEAUX; i++) {
        if (estCoule(&flotte[i]) == 0) {
            return 0;  // Au moins un bateau encore vivant
        }
    }
    
    return 1;  // Tous coulés = victoire !
}


int trouverBateau(Bateaux* flotte, int ligne, int colonne)
{
    // Parcourir tous les bateaux de la flotte
    for (int i = 0; i < NB_BATEAUX; i++) {
        if (appartientAuBateau(&flotte[i], ligne, colonne) == 1) {
            return i;  // Retourne l'index du bateau touché (0-4)
        }
    }
    
    return -1;  // Aucun bateau à cette position
}

