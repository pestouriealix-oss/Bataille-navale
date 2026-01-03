#ifndef _GRILLE_H_
#define _GRILLE_H_

#include "librairies.h"

/**
 * ============================================================================
 * FICHIER : grille.h
 * ----------------------------------------------------------------------------
 * Gère la structure de données de la grille (plateau de jeu) et les opérations
 * directes dessus : création, affichage, tirs et nettoyage mémoire.
 * ============================================================================
 */


/* =========================================================
 * STRUCT : Grille
 * ---------------------------------------------------------
 * Représente le plateau de 10x10 cases.
 * * Valeurs possibles dans le tableau :
 * 0 : EAU    (Case vide)
 * 1 : BATEAU (Présent mais caché/intact)
 * 2 : RATÉ   (Tir dans l'eau - "Plouf")
 * 3 : TOUCHÉ (Bateau touché par un tir)
 * ========================================================= */
struct grille {
    int tab[10][10];  /* Matrice 10x10 représentant l'état du jeu */
};

typedef struct grille Grille;


/* ============================================================================
 * 1. INITIALISATION ET MÉMOIRE
 * ============================================================================ */

/** * @brief Alloue et initialise une nouvelle grille de jeu vide.
 * * Cette fonction réserve la mémoire pour une structure Grille et remplit
 * toutes les cases avec des 0 (EAU).
 * * @return Grille* Un pointeur vers la grille créée (ou NULL si échec).
 */
Grille* initialiser(void);

/**
 * @brief Libère la mémoire occupée par une grille.
 * * @param g Pointeur vers la grille à détruire.
 */
void detruireGrille(Grille* g);


/* ============================================================================
 * 2. AFFICHAGE
 * ============================================================================ */

/**
 * @brief Affiche la grille complète avec le design du jeu (Couleurs, Cadres).
 * * C'est la fonction utilisée pour afficher SA PROPRE grille.
 * On voit tout : l'eau, les bateaux, les tirs réussis et ratés.
 * * @param g Pointeur vers la grille.
 */
void afficherCoordonnees(Grille* g);

/**
 * @brief Affiche la grille en CACHANT les bateaux intacts.
 * * C'est la fonction utilisée pour afficher la grille de l'ADVERSAIRE.
 * - Les bateaux (1) sont affichés comme de l'eau (~) pour ne pas tricher.
 * - Les tirs ratés (2) et touchés (3) sont visibles.
 * * @param g Pointeur vers la grille.
 */
void afficherMasque(Grille* g);

/**
 * @brief Affiche le contenu brut de la grille (Mode Débogage).
 * * Affiche simplement une matrice de chiffres (0, 1, 2, 3) sans décoration.
 * Utile pour vérifier la logique interne si l'affichage bug.
 * * @param g Pointeur vers la grille.
 */
void afficher(Grille* g);


/* ============================================================================
 * 3. ACTIONS DE JEU
 * ============================================================================ */

/**
 * @brief Gère un tir sur une case donnée.
 * * Vérifie les coordonnées, l'état de la case cible, et met à jour la grille.
 * * @param g Pointeur vers la grille cible.
 * @param ligne Index de la ligne (0-9).
 * @param colonne Index de la colonne (0-9).
 * * @return int Code de résultat :
 * 1 : TOUCHÉ (Bateau touché)
 * 0 : RATÉ (Tir dans l'eau)
 * -1 : DÉJÀ JOUÉ (Case déjà visée auparavant)
 * -2 : HORS GRILLE (Coordonnées invalides)
 */
int tirer(Grille* g, int ligne, int colonne);

#endif
