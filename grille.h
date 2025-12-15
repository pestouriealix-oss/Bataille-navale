#ifndef _GRILLE_H_
#define _GRILLE_H_

#include "librairies.h"

/* =========================================================
 * Structure : Grille
 * ---------------------------------------------------------
 * Représente la grille de jeu pour un joueur dans la
 * bataille navale.
 *
 * Chaque case du tableau correspond à une position sur
 * la grille (ligne, colonne).
 * 
 * Convention possible pour les valeurs du tableau :
 *   0 → case vide
 *   1 → bateau présent
 *   2 → tir manqué
 *   3 → bateau touché
 *   4 → bateau coulé
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
 * Date    : 10/11/2025
 * Fichier : grille.h
 * ========================================================= */

struct grille {
    int tab[10][10];  /* Tableau représentant les cases du plateau */
};

typedef struct grille Grille;  /* Alias pour simplifier l’utilisation */

//Fonction principales :

//Fonction Initialisation d'une grille 
Grille* initialiser(void);

//Fonction d'affichage d'une grille
void afficher(Grille*);

void detruireGrille(Grille*);

void afficherCoordonnees(Grille*);

void afficherMasque(Grille*);

//Tirer sur une case (retourne : -2=hors grille, -1=déjà tiré, 0=raté, 1=touché)
int tirer(Grille*, int, int);

#endif
