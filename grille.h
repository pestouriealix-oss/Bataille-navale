#ifndef _GRILLE_H_
#define _GRILLE_H_

#include "librairies.h"

/* =========================================================
 * STRUCT : Grille
 * ---------------------------------------------------------
 * Représente la grille de jeu pour un joueur dans la
 * bataille navale.
 *
 * Chaque case du tableau correspond à une position sur
 * la grille (ligne, colonne).
 * 
 * Convention possible pour les valeurs du tableau :
 * 0 : EAU (case vide)
 * 1 : BATEAU (Présent mais non touché)
 * 2 : PLOUF (Tir raté dans l'eau)
 * 3 : TOUCHÉ (Bateau touché par un tir)
 *
 * Dimensions :
 *   La grille est de 10x10 cases 
 * 
 * Détails techniques :
 *   La grille est implémentée sous forme d’un tableau 10x10
 *   d’entiers. Les fonctions qui doivent modifier son contenu
 *   (ex : initialiser, placer un bateau, enregistrer un tir)
 *   recevront un *pointeur vers la structure Grille*.
 *
 *   - Sans pointeur, une fonction travaillerait sur une copie
 *     locale de la grille et les modifications ne seraient
 *     pas visibles à l’extérieur.
 *   - En passant un pointeur (Grille *g), on agit directement
 *     sur la grille originale du joueur.
 *
 * ========================================================= */

struct grille {
    int tab[10][10];  /* Matrice 10x10 représentant l'état du jeu */
};

typedef struct grille Grille;  /* Alias pour simplifier l’utilisation */

/* ============================================================================
 * FONCTIONS PRINCIPALES
 * ============================================================================ */

/** 
 * @brief Alloue et initialise une nouvelle grille de jeu.
* * Cette fonction réserve la mémoire pour une strucure Grille et remplit
* toutes les cases avec des 0 (EAU).
* * @return Grille* Un pointeur vers la grille créée (ou NULL si échec).
*/
Grille* initialiser(void);

/**
 * @brief Affiche le contenu brut de la grille (pour débogage)
 * Affiche simplement les 0, 1, 2, 3 sans coordonnées.
 * @param g pointeur vers la grille à afficher.
 */
void afficher(Grille*);

/**
 * @brief Libère la mémoire occupée par une grille.
 * *@param g Pointeur vers la grille à détruire.
 */
void detruireGrille(Grille*);

/**
 * @brief Affiche la grille avec les coordonnées (A-J, 1-10).
 * C'est la fonction principale d'affichage pour le joueur actif.
 * * @param g Pointeur vers la grille.
 */
void afficherCoordonnees(Grille*);

/**
 * @brief Affiche la grille en cachant les bateaux 
 * * Utile pour afficher la grille de l'adversaire :
 *  - Les bateaux intacts (1) sont affichés comme de l'eau (0).
 *  - Seules les tirs ratés (2) et touchés (3) sont révélés.
 * * @param g pointeur vers la grille.
 */
void afficherMasque(Grille*);

/**
 * @brief Gère un tir sur une case donnée.
 * * Vérifie si le tir est valide, met à jour la grille, et renvoie le résultat.
 * * @param g Pointeur vers la grille cible.
 * @param ligne Index de la ligne (0-9).
 * @param colonne Index de la colonne (0-9).
 * * @return int Code de résultat :
 * 1 : TOUCHÉ (Bateau touché)
 * 0 : RATÉ (Tir dans l'eau)
 * -1 : DÉJÀ JOUÉ (Case déjà visée auparavant)
 * -2 : HORS GRILLE (Coordonnées invalides)
 */
int tirer(Grille*, int, int);

#endif
