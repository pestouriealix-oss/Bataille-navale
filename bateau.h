#ifndef _BATEAU_H_
#define _BATEAU_H_

#include "librairies.h"
#include "grille.h"

/**
 * Constante : Nombre de bateaux par joueur.
 * Dans la version classique, chaque joueur possède 5 navires :
 * 1 Porte-avion (5), 1 Croiseur (4), 2 Contre-torpilleurs (3), 1 Torpilleur (2).
 */
#define NB_BATEAUX 5

/**
 * ============================================================================
 * STRUCT : Bateaux
 * ----------------------------------------------------------------------------
 * Représente un navire individuel et son état actuel.
 * * Attributs :
 * - nom : Le nom affiché (ex: "Porte-avion").
 * - ligne, colonne : Coordonnées de la PREMIÈRE case du bateau (la "tête").
 * - taille : Longueur du bateau (nombre de cases occupées).
 * - orientation : 'H' (Horizontal) ou 'V' (Vertical).
 * - touche : Nombre de fois que ce bateau a été touché.
 * Si touche == taille, le bateau est COULÉ.
 * ============================================================================
 */
struct bateaux{
    char nom[20];
    int ligne;
    int colonne;
    int taille; 
    char orientation; // 'H' ou 'V'
    int touche;       // Compteur de dégâts
};

typedef struct bateaux Bateaux;

/* ============================================================================
 * INITIALISATION ET MÉMOIRE
 * ============================================================================ */

/**
 * @brief Crée la flotte standard complète d'un joueur.
 * * Alloue un tableau de 5 structures Bateaux et initialise leurs noms
 * et tailles selon les règles classiques. Les positions sont mises à -1.
 * * @return Bateaux* Pointeur vers le tableau de bateaux (la flotte).
 */
Bateaux* creerFlotte(void);

/**
 * @brief Libère la mémoire d'une flotte entière.
 * * @param flotte Pointeur vers le tableau à libérer.
 */
void detruireFlotte(Bateaux* flotte);

/**
 * @brief Crée un seul bateau vide (utilitaire).
 * * @return Bateaux* Pointeur vers la structure créée.
 */
Bateaux* creerBateaux(void);

/**
 * @brief Libère la mémoire d'un seul bateau.
 * * @param b Pointeur vers le bateau.
 */
void detruireBateaux(Bateaux* b);


/* ============================================================================
 * PLACEMENT ET INTERACTION
 * ============================================================================ */

/**
 * @brief Gère le placement interactif d'un bateau sur la grille.
 * * Demande à l'utilisateur de saisir des coordonnées et une orientation.
 * Vérifie la validité (collisions, hors limites) et met à jour la grille.
 * * @param b Pointeur vers le bateau à placer.
 * @param g Pointeur vers la grille du joueur.
 * * @return int 1 si succès, 0 si le joueur a annulé (avec -1).
 */
int positionnerBateau(Bateaux* b, Grille* g);


/* ============================================================================
 * GESTION DES DÉGÂTS
 * ============================================================================ */

/**
 * @brief Ajoute un dégât au bateau (incrémente le compteur 'touche').
 * * Doit être appelé quand un tir touche ce bateau spécifique.
 * * @param b Pointeur vers le bateau touché.
 */
void enregistrerTouche(Bateaux* b);

/**
 * @brief Vérifie si le bateau est coulé.
 * * Un bateau est coulé si son nombre de touches est égal à sa taille.
 * * @param b Pointeur vers le bateau.
 * @return int 1 si COULÉ, 0 sinon.
 */
int estCoule(Bateaux* b);

/**
 * @brief Vérifie si le bateau est touché mais pas encore coulé.
 * * @param b Pointeur vers le bateau.
 * @return int 1 si TOUCHÉ (endommagé), 0 sinon.
 */
int estTouche(Bateaux* b);

/**
 * @brief Obtient un caractère représentant l'état du bateau.
 * * @param b Pointeur vers le bateau.
 * @return char 'C' (Coulé), 'T' (Touché), 'O' (Opérationnel/Intact).
 */
char obtenirEtat(Bateaux* b);


/* ============================================================================
 * LOGIQUE SPATIALE 
 * ============================================================================ */

/**
 * @brief Vérifie si une coordonnée précise appartient à ce bateau.
 * * Calcule si (ligne, colonne) se trouve sur le segment occupé par le bateau,
 * en tenant compte de son point de départ, sa taille et son orientation.
 * * @param b Pointeur vers le bateau.
 * @param ligne Ligne à tester.
 * @param colonne Colonne à tester.
 * * @return int 1 si la case appartient au bateau, 0 sinon.
 */
int appartientAuBateau(Bateaux* b, int ligne, int colonne);

/**
 * @brief Cherche quel bateau de la flotte se trouve à une position donnée.
 * * Parcourt toute la flotte et utilise 'appartientAuBateau' pour chaque navire.
 * * @param flotte Tableau des bateaux.
 * @param ligne Ligne ciblée.
 * @param colonne Colonne ciblée.
 * * @return int L'index du bateau trouvé (0 à 4), ou -1 si aucun bateau n'est là.
 */
int trouverBateau(Bateaux* flotte, int ligne, int colonne);

/**
 * @brief Vérifie la condition de victoire (Tous les bateaux coulés).
 * * @param flotte Tableau des bateaux.
 * @return int 1 si TOUTE la flotte est coulée (Victoire adverse), 0 sinon.
 */
int tousCoules(Bateaux* flotte);

#endif
