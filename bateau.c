#include "librairies.h"
#include "bateau.h"
#include "jeu.h" // Nécessaire pour accéder à la structure 'Joueur'

/* ============================================================================
 * 1. INITIALISATION ET MÉMOIRE
 * ============================================================================ */

Bateaux* creerBateaux(void) {
    Bateaux* b = (Bateaux*)malloc(sizeof(Bateaux));
    if (b == NULL) {
        fprintf(stderr, "Erreur d'allocation mémoire pour un bateau.\n");
        exit(1);
    }
    b->touche = 0;
    b->ligne = -1;
    b->colonne = -1;
    return b;
}

void detruireBateaux(Bateaux* b) {
    if (b != NULL) {
        free(b);
    }
}

Bateaux* creerFlotte(void) {
    // Allocation d'un tableau de 5 bateaux
    Bateaux* flotte = (Bateaux*)malloc(NB_BATEAUX * sizeof(Bateaux));
    if (flotte == NULL) {
        fprintf(stderr, "Erreur d'allocation mémoire pour la flotte.\n");
        exit(1);
    }

    // --- Initialisation des caractéristiques fixes (Règles classiques) ---
    
    // 1. Porte-Avion (5 cases)
    strcpy(flotte[0].nom, "Porte-Avion");
    flotte[0].taille = 5;

    // 2. Croiseur (4 cases)
    strcpy(flotte[1].nom, "Croiseur");
    flotte[1].taille = 4;

    // 3. Contre-Torpilleur A (3 cases)
    strcpy(flotte[2].nom, "Contre-Torpilleur A");
    flotte[2].taille = 3;

    // 4. Contre-Torpilleur B (3 cases)
    strcpy(flotte[3].nom, "Contre-Torpilleur B");
    flotte[3].taille = 3;

    // 5. Torpilleur (2 cases)
    strcpy(flotte[4].nom, "Torpilleur");
    flotte[4].taille = 2;

    // Initialisation des états par défaut pour tous
    for (int i = 0; i < NB_BATEAUX; i++) {
        flotte[i].touche = 0;
        flotte[i].ligne = -1;
        flotte[i].colonne = -1;
        flotte[i].orientation = 'H';
    }

    return flotte;
}

void detruireFlotte(Bateaux* flotte) {
    if (flotte != NULL) {
        free(flotte);
    }
}


/* ============================================================================
 * 2. PLACEMENT ET INTERACTION
 * ============================================================================ */

int positionnerBateau(Bateaux* b, Grille* g) {
    char buffer[10];
    char orientBuffer[10];
    int ligne, colonne;
    char orientation;

    while (1) {
        // 1. Demande de l'orientation
        printf("Orientation (H = Horizontal, V = Vertical) ou -1 pour quitter : ");
        scanf("%s", orientBuffer);
        
        if (strcmp(orientBuffer, "-1") == 0) return 0; // Annulation
        
        // Validation simple
        if (orientBuffer[0] == 'h' || orientBuffer[0] == 'H') orientation = 'H';
        else if (orientBuffer[0] == 'v' || orientBuffer[0] == 'V') orientation = 'V';
        else {
            printf(ROUGE "Orientation invalide. Utilisez H ou V.\n" RESET);
            continue;
        }

        // 2. Demande des coordonnées
        printf("Coordonnees de la premiere case (ex: A5) : ");
        scanf("%s", buffer);

        // Conversion (A -> 0, B -> 1...) et (1 -> 0, 2 -> 1...)
        char lettre = buffer[0];
        if (lettre >= 'a' && lettre <= 'z') lettre -= 32; // Minuscule vers Majuscule
        
        colonne = lettre - 'A';
        ligne = atoi(&buffer[1]) - 1;

        // 3. Vérifications de base (Limites de la grille)
        if (colonne < 0 || colonne > 9 || ligne < 0 || ligne > 9) {
            printf(ROUGE "Coordonnees hors grille !\n" RESET);
            continue;
        }

        // 4. Vérification : Le bateau rentre-t-il dans la grille ?
        if (orientation == 'H' && (colonne + b->taille > 10)) {
            printf(ROUGE "Le bateau depasse a droite !\n" RESET);
            continue;
        }
        if (orientation == 'V' && (ligne + b->taille > 10)) {
            printf(ROUGE "Le bateau depasse en bas !\n" RESET);
            continue;
        }

        // 5. Vérification : Collisions (Y a-t-il déjà un bateau ici ?)
        int collision = 0;
        for (int k = 0; k < b->taille; k++) {
            if (orientation == 'H') {
                if (g->tab[ligne][colonne + k] != 0) collision = 1;
            } else {
                if (g->tab[ligne + k][colonne] != 0) collision = 1;
            }
        }

        if (collision) {
            printf(ROUGE "Impossible : Il y a deja un bateau sur le chemin !\n" RESET);
            continue;
        }

        // --- SI TOUT EST BON ---
        
        // A. Mise à jour visuelle de la grille (1 = Bateau)
        for (int k = 0; k < b->taille; k++) {
            if (orientation == 'H') g->tab[ligne][colonne + k] = 1;
            else g->tab[ligne + k][colonne] = 1;
        }

        // B. Sauvegarde des données dans la structure Bateaux (ESSENTIEL pour la victoire)
        b->ligne = ligne;
        b->colonne = colonne;
        b->orientation = orientation;

        return 1; // Succès
    }
}

void placerBateaux(Joueur *joueur) {
    for (int i = 0; i < NB_BATEAUX; i++) {
        printf(EFFACE);
        afficherCoordonnees(joueur->mesNavires); // Montre où on en est
        printf("\nPlacement du %s " JAUNE "(%d cases)" RESET " :\n", 
               joueur->flotte[i].nom, joueur->flotte[i].taille);
        
        if (positionnerBateau(&joueur->flotte[i], joueur->mesNavires) == 0) {
            // Gestion de l'annulation (optionnel : quitter le jeu ou recommencer)
            printf("Placement annule.\n");
            return;
        }
    }
}

void placerBateauxAleatoire(Joueur* j) {
    int ligne, colonne, dir;
    char orientation;
    int valide;

    // Note: srand() doit être appelé dans le main, pas ici.

    for (int i = 0; i < NB_BATEAUX; i++) {
        valide = 0;
        Bateaux* b = &j->flotte[i]; 

        // On essaie tant qu'on a pas trouvé une place valide
        while (!valide) {
            // 1. Tirage au sort
            ligne = rand() % 10;
            colonne = rand() % 10;
            dir = rand() % 2;  // 0 ou 1
            orientation = (dir == 0) ? 'H' : 'V';

            // 2. Vérif Limites
            if (orientation == 'H' && (colonne + b->taille > 10)) continue;
            if (orientation == 'V' && (ligne + b->taille > 10)) continue;

            // 3. Vérif Collisions
            int collision = 0;
            for (int k = 0; k < b->taille; k++) {
                if (orientation == 'H') {
                    if (j->mesNavires->tab[ligne][colonne + k] != 0) collision = 1;
                } else {
                    if (j->mesNavires->tab[ligne + k][colonne] != 0) collision = 1;
                }
            }
            if (collision == 1) continue;

            // 4. Placement VALIDÉ
            
            // Mise à jour Grille
            for (int k = 0; k < b->taille; k++) {
                if (orientation == 'H') j->mesNavires->tab[ligne][colonne + k] = 1;
                else j->mesNavires->tab[ligne + k][colonne] = 1;
            }

            // Mise à jour Structure Bateau (ESSENTIEL)
            b->ligne = ligne;      
            b->colonne = colonne;  
            b->orientation = orientation;
            b->touche = 0; // On s'assure qu'il est neuf
            
            valide = 1; // On passe au bateau suivant
        }
    }
}


/* ============================================================================
 * 3. GESTION DES DÉGÂTS
 * ============================================================================ */

void enregistrerTouche(Bateaux* b) {
    if (b->touche < b->taille) {
        b->touche++;
    }
}

int estCoule(Bateaux* b) {
    return (b->touche >= b->taille);
}

int estTouche(Bateaux* b) {
    return (b->touche > 0 && b->touche < b->taille);
}

char obtenirEtat(Bateaux* b) {
    if (estCoule(b)) return 'C'; // Coulé
    if (estTouche(b)) return 'T'; // Touché
    return 'O'; // Opérationnel (Intact)
}


/* ============================================================================
 * 4. LOGIQUE SPATIALE (Détection)
 * ============================================================================ */

int appartientAuBateau(Bateaux* b, int ligne, int colonne) {
    // Si le bateau n'est pas placé (coords -1), il n'est pas là
    if (b->ligne == -1 || b->colonne == -1) return 0;

    if (b->orientation == 'H') {
        // Horizontal : même ligne, colonne comprise entre [début, début + taille[
        if (ligne == b->ligne && colonne >= b->colonne && colonne < (b->colonne + b->taille)) {
            return 1;
        }
    } else {
        // Vertical : même colonne, ligne comprise entre [début, début + taille[
        if (colonne == b->colonne && ligne >= b->ligne && ligne < (b->ligne + b->taille)) {
            return 1;
        }
    }
    return 0;
}

int trouverBateau(Bateaux* flotte, int ligne, int colonne) {
    // On parcourt les 5 bateaux pour voir si la case (l,c) appartient à l'un d'eux
    for (int i = 0; i < NB_BATEAUX; i++) {
        if (appartientAuBateau(&flotte[i], ligne, colonne)) {
            return i; // Retourne l'index du bateau trouvé
        }
    }
    return -1; // Aucun bateau ici
}

int tousCoules(Bateaux* flotte) {
    for (int i = 0; i < NB_BATEAUX; i++) {
        if (!estCoule(&flotte[i])) {
            return 0; // Il reste au moins un bateau vivant
        }
    }
    return 1; // Tous les bateaux sont coulés, c'est la victoire
}
