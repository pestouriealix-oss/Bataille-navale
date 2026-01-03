#include "librairies.h"
#include "sauvegarde.h"
#include "bateau.h"

/* ============================================================================
 * 1. SAUVEGARDE
 * ============================================================================ */

int sauvegarderPartie(char* nomFichier, Joueur* j1, Joueur* j2, int tourActuel, char* difficulte, char* nomPartie, long tempsJeu) {
    // 1. Ouverture du fichier en mode écriture binaire ("wb")
    FILE* f = fopen(nomFichier, "wb");
    if (f == NULL) {
        fprintf(stderr, "Erreur : Impossible de créer le fichier de sauvegarde.\n");
        return 0; 
    }

    // --- PRÉPARATION DES DONNÉES ---
    // On s'assure que la difficulté fait exactement 15 octets pour éviter les décalages mémoire
    char bufferDiff[15] = {0}; 
    strncpy(bufferDiff, difficulte, 14);

    // 2. Écriture des MÉTADONNÉES (Header du fichier)
    // fwrite(pointeur_donnée, taille_unitaire, nombre_unités, fichier)
    
    fwrite(nomPartie, sizeof(char), 50, f);   // Nom de la save (fixe 50 octets)
    fwrite(&tempsJeu, sizeof(long), 1, f);    // Temps écoulé
    fwrite(bufferDiff, sizeof(char), 15, f);  // Difficulté nettoyée
    fwrite(&tourActuel, sizeof(int), 1, f);   // Tour en cours

    // 3. Écriture du JOUEUR 1
    fwrite(j1->NomJoueur, sizeof(char), 20, f);
    fwrite(&j1->NbVictoire, sizeof(int), 1, f);
    fwrite(j1->mesNavires, sizeof(Grille), 1, f); // On sauvegarde toute la grille brute
    fwrite(j1->mesTirs, sizeof(Grille), 1, f);
    fwrite(j1->flotte, sizeof(Bateaux), NB_BATEAUX, f); // On sauvegarde l'état des bateaux (PV, pos...)

    // 4. Écriture du JOUEUR 2
    fwrite(j2->NomJoueur, sizeof(char), 20, f);
    fwrite(&j2->NbVictoire, sizeof(int), 1, f);
    fwrite(j2->mesNavires, sizeof(Grille), 1, f);
    fwrite(j2->mesTirs, sizeof(Grille), 1, f);
    fwrite(j2->flotte, sizeof(Bateaux), NB_BATEAUX, f);

    // 5. Fermeture et Confirmation
    fclose(f);
    printf("✅ Partie '%s' sauvegardee avec succes (%lds de jeu) !\n", nomPartie, tempsJeu);
    return 1;
}


/* ============================================================================
 * 2. CHARGEMENT
 * ============================================================================ */

int chargerPartie(char* nomFichier, Joueur** j1, Joueur** j2, char* difficulte, char* nomPartie, long* tempsJeu) {
    // 1. Ouverture en lecture binaire ("rb")
    FILE* f = fopen(nomFichier, "rb");
    if (f == NULL) return -1; // Fichier introuvable

    // Allocation dynamique des joueurs (car ils n'existent pas encore)
    *j1 = creerJoueur();
    *j2 = creerJoueur();
    int tour;

    // 2. Lecture des MÉTADONNÉES
    // IMPORTANT : On doit lire EXACTEMENT dans le même ordre que l'écriture !
    fread(nomPartie, sizeof(char), 50, f);
    fread(tempsJeu, sizeof(long), 1, f);
    fread(difficulte, sizeof(char), 15, f);
    fread(&tour, sizeof(int), 1, f);

    // 3. Lecture du JOUEUR 1
    fread((*j1)->NomJoueur, sizeof(char), 20, f);
    fread(&(*j1)->NbVictoire, sizeof(int), 1, f);
    fread((*j1)->mesNavires, sizeof(Grille), 1, f);
    fread((*j1)->mesTirs, sizeof(Grille), 1, f);
    fread((*j1)->flotte, sizeof(Bateaux), NB_BATEAUX, f);

    // 4. Lecture du JOUEUR 2
    fread((*j2)->NomJoueur, sizeof(char), 20, f);
    fread(&(*j2)->NbVictoire, sizeof(int), 1, f);
    fread((*j2)->mesNavires, sizeof(Grille), 1, f);
    fread((*j2)->mesTirs, sizeof(Grille), 1, f);
    fread((*j2)->flotte, sizeof(Bateaux), NB_BATEAUX, f);

    fclose(f);
    return tour; // On renvoie le tour pour que le jeu sache où reprendre
}


/* ============================================================================
 * 3. UTILITAIRES
 * ============================================================================ */

/* Lit juste l'en-tête du fichier pour afficher les infos dans le menu "Charger" */
void lireInfosSauvegarde(char* nomFichier, char* nomPartie, long* tempsJeu, char* difficulte) {
    FILE* f = fopen(nomFichier, "rb");
    
    // Si le fichier n'existe pas ou erreur
    if (f == NULL) {
        strcpy(nomPartie, "Vide");
        *tempsJeu = 0;
        return;
    }
    
    // On lit juste ce qu'il faut
    fread(nomPartie, sizeof(char), 50, f);
    fread(tempsJeu, sizeof(long), 1, f);
    fread(difficulte, sizeof(char), 15, f);

    // On ferme tout de suite, pas besoin de lire les grilles des joueurs
    fclose(f);
}
