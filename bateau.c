#include "librairies.h"
#include "grille.h"
#include"jeu.h"
#include "bateau.h"


/* ============================================================================
 * INITIALISATION ET MÉMOIRE
 * ============================================================================ */


Bateaux* creerFlotte(void)
{
    // 1. Allocation du tableau pour 5 structures Bateaux
    Bateaux* flotte = malloc(NB_BATEAUX * sizeof(Bateaux));

    // Sécurité allocation (bonnes pratiques)
    if (flotte == NULL) return NULL;

    // 2. Configuration manuelle des 5 navires
    
    // Bateau 1 : Porte-avion (5 cases)
    strcpy(flotte[0].nom, "Porte-avion");
    flotte[0].taille = 5;
    flotte[0].touche = 0;

    // Bateau 2 : Croiseur (4 cases)
    strcpy(flotte[1].nom, "Croiseur");
    flotte[1].taille = 4;
    flotte[1].touche = 0;

    // Bateau 3 : Contre-torpilleur n°1 (3 cases)
    strcpy(flotte[2].nom, "Contre-torpilleur");
    flotte[2].taille = 3;
    flotte[2].touche = 0;

    // Bateau 4 : Contre-torpilleur n°2 (3 cases)
    strcpy(flotte[3].nom, "Contre-torpilleur");
    flotte[3].taille = 3;
    flotte[3].touche = 0;

    // Bateau 5 : Torpilleur (2 cases)
    strcpy(flotte[4].nom, "Torpilleur");
    flotte[4].taille = 2;
    flotte[4].touche = 0;

    return flotte;
}


void detruireFlotte (Bateaux* flotte)
{
    if (flotte != NULL) {
        free(flotte);
    }
}


Bateaux* creerBateaux(void)
{
    Bateaux* b = malloc(sizeof(Bateaux));
    if (b != NULL) {
        strcpy(b->nom, "Vide");
        b->taille = 0;
        b->ligne = 0;
        b->colonne = 0;
        b->orientation = '\0';
        b->touche = 0;
    }
    return b;
}


void detruireBateaux(Bateaux* b)
{
    if (b != NULL) {
        free(b);
    }
}


/* ============================================================================
 * PLACEMENT ET INTERACTION
 * ============================================================================ */


int positionnerBateau(Bateaux* b, Grille* g)
{
    int ligne, colonne;
    char conversion;
    char orientation;

    // Boucle infinie : on ne sort que si le placement est valide ou si abandon
    while (1) 
    {
        printf("\n--- Placement du %s (Taille %d) ---\n", b->nom, b->taille);
        
        // --- 2. Saisie de la colonne ---
        char buffer[10]; // On crée une boite pour stocker du texte
        
        printf("\nEntrez la case que vous souhaité attaqué colonne (A-J) puis ligne (1-10) ou -1 pour pour abandonner : ");
        scanf("%s", buffer); // On lit tout le texte (ex: "A", "b" ou "-1")

        // GESTION DU QUIT : On compare le texte avec "-1"
        // (strcmp renvoie 0 si les deux textes sont identiques)
        if (strcmp(buffer, "-1") == 0) {
            // Pas besoin de vider le buffer ici car scanf("%s") a tout mangé
            return 0; // Code 0 = Annulation (IMPORTANT : 0, pas 1 !)
        }

        if (strlen(buffer) < 2) {
            printf("❌ Format invalide ! Utilisez le format A8 (lettre + chiffre).\n");
            continue;
        }

        // Si ce n'est pas "-1", on prend juste la première lettre
        conversion = buffer[0];

        if (conversion >= 'a' && conversion <= 'j') {
            conversion = conversion - 32;
        }

        ligne = atoi(&buffer[1]); 

        // --- 3. Saisie de l'orientation ---
        printf("Orientation (H/V) : ");
        scanf(" %c", &orientation); // Espace avant %c pour ignorer les espaces précédents
        
        // Nettoyer le buffer d'entrée (vider les entrées parasites)
        // C'est vital avec scanf pour éviter les boucles infinies en cas d'erreur
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
        
        // Normalisation : Accepter minuscules et convertir en majuscules
        if (orientation == 'h') orientation = 'H';
        if (orientation == 'v') orientation = 'V';

        if(conversion >= 'a' && conversion <= 'j' )
        {
            conversion = conversion - 32;
        }
        
        // CONVERSION COORDONNÉES : Humain (1-10) -> Machine (0-9)
        ligne = ligne - 1;
        colonne = conversion - 'A';

        // --- 4. VALIDATION ---
        
        // A. Orientation correcte ?
        if (orientation != 'H' && orientation != 'V') {
            printf("❌ Erreur : Orientation invalide ! Utilisez H ou V.\n");
            continue; // Retour au début de la boucle
        }

        // B. Dépassement de grille (Hors limites) ?
        // H : Colonne + taille ne doit pas dépasser 10
        // V : Ligne + taille ne doit pas dépasser 10
        if ((orientation == 'H' && (colonne + b->taille > 10 || colonne < 0 || ligne < 0 || ligne >= 10)) ||
            (orientation == 'V' && (ligne + b->taille > 10 || ligne < 0 || colonne < 0 || colonne >= 10))) {
            
            printf("❌ Erreur : Le bateau sort de la grille ! Réessayez.\n");
            continue;
        }

        // C. Collision (Chevauchement) ?
        // On vérifie si une des cases futures contient déjà un bateau (1)
        int collision = 0;
        for (int i = 0; i < b->taille; i++) {
            if (orientation == 'H' && g->tab[ligne][colonne + i] == 1) collision = 1;
            if (orientation == 'V' && g->tab[ligne + i][colonne] == 1) collision = 1;
        }

        if (collision == 1) {
            printf("❌ Erreur : Il y a déjà un bateau ici ! Réessayez.\n");
            continue;
        }

        // --- 5. APPLICATION (Si tout est valide) ---
        
        // On écrit les 1 sur la grille
        for (int i = 0; i < b->taille; i++) {
            if (orientation == 'H') g->tab[ligne][colonne + i] = 1;
            else g->tab[ligne + i][colonne] = 1;
        }

        // On sauvegarde les coordonnées DANS le bateau (pour gestion des dégâts plus tard)
        b->ligne = ligne;
        b->colonne = colonne;
        b->orientation = orientation;

        printf("✅ %s placé avec succès !\n", b->nom);
        return 1; // Code de retour 1 = Succès
    }
}


void placerBateauxAleatoire(Joueur* j) {
    int ligne, colonne, direction;
    char orientation;
    int valide;

    // On parcourt chaque bateau de la flotte
    for (int i = 0; i < NB_BATEAUX; i++) {
        valide = 0; // Le bateau n'est pas encore placé

        // On boucle TANT QUE on n'a pas trouvé une position valide
        while (!valide) {
            // 1. Génération aléatoire
            ligne = rand() % 10;     // 0 à 9
            colonne = rand() % 10;   // 0 à 9
            direction = rand() % 2;  // 0 ou 1
            
            if (direction == 0) orientation = 'H';
            else orientation = 'V';

            // 2. Vérification : Est-ce que ça rentre dans la grille ?
            if (orientation == 'H' && (colonne + j->flotte[i].taille > 10)) continue;
            if (orientation == 'V' && (ligne + j->flotte[i].taille > 10)) continue;

            // 3. Vérification : Est-ce qu'il y a une collision ?
            int collision = 0;
            for (int k = 0; k < j->flotte[i].taille; k++) {
                if (orientation == 'H') {
                    if (j->mesNavires->tab[ligne][colonne + k] == 1) collision = 1;
                } else {
                    if (j->mesNavires->tab[ligne + k][colonne] == 1) collision = 1;
                }
            }

            // Si collision, on recommence la boucle while
            if (collision == 1) continue;

            // 4. Si tout est bon : On place le bateau !
            for (int k = 0; k < j->flotte[i].taille; k++) {
                if (orientation == 'H') {
                    j->mesNavires->tab[ligne][colonne + k] = 1;
                } else {
                    j->mesNavires->tab[ligne + k][colonne] = 1;
                }
            }

            // On sauvegarde les infos dans la structure du bateau
            j->flotte[i].ligne = ligne;
            j->flotte[i].colonne = colonne;
            j->flotte[i].orientation = orientation;
            
            valide = 1; // On peut passer au bateau suivant
        }
    }
}


/* ============================================================================
 * GESTION DES DÉGÂTS
 * ============================================================================ */

void enregistrerTouche(Bateaux* b)
{
    // Incrémente simplement le compteur de dégâts
    b->touche = b->touche + 1;
}

int estCoule(Bateaux* b)
{
    // Si nombre de touches == taille totale, le bateau est mort
    if(b->touche == b->taille) {
        return 1; // Vrai
    } else {
        return 0; // Faux
    }
}

int estTouche(Bateaux* b)
{
    // Si touché au moins une fois, mais pas encore coulé
    if(b->touche > 0 && b->touche < b->taille) {
        return 1; // Vrai
    } else {
        return 0; // Faux
    }
}

char obtenirEtat(Bateaux* b)
{
    if (estCoule(b) == 1) return 'C';
    else if (estTouche(b) == 1) return 'T';
    else return 'O'; // Opérationnel
}


/* ============================================================================
 * LOGIQUE SPATIALE 
 * ============================================================================ */


int appartientAuBateau(Bateaux* b, int ligne, int colonne)
{
    // Cas 1 : Bateau Horizontal
    // Il est sur la même LIGNE, et la COLONNE est comprise entre [début, fin[
    if (b->orientation == 'H') {
        if (ligne == b->ligne && 
            colonne >= b->colonne && 
            colonne < b->colonne + b->taille) {
            return 1;
        }
    }
    
    // Cas 2 : Bateau Vertical
    // Il est sur la même COLONNE, et la LIGNE est comprise entre [début, fin[
    if (b->orientation == 'V') {
        if (colonne == b->colonne && 
            ligne >= b->ligne && 
            ligne < b->ligne + b->taille) {
            return 1;
        }
    }
    
    return 0; // La case ne fait pas partie de ce bateau
}


int tousCoules(Bateaux* flotte)
{
    for (int i = 0; i < NB_BATEAUX; i++) {
        // Si UN SEUL bateau n'est pas coulé, la partie continue
        if (estCoule(&flotte[i]) == 0) {
            return 0;
        }
    }
    // Si on arrive ici, c'est que la boucle s'est finie sans trouver de survivant
    return 1;
}

int trouverBateau(Bateaux* flotte, int ligne, int colonne)
{
    // On teste chaque bateau de la flotte un par un
    for (int i = 0; i < NB_BATEAUX; i++) {
        if (appartientAuBateau(&flotte[i], ligne, colonne) == 1) {
            return i;  // On retourne l'index (0 à 4) du bateau trouvé
        }
    }
    
    return -1;  // Sécurité : Aucun bateau trouvé à cette position
}
