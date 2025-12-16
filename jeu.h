#ifndef _JEU_H_
#define _JEU_H_

#include "librairies.h"
#include "grille.h"

typedef struct bateaux Bateaux;


/**
 * ============================================================================
 * STRUCT : Joueur
 * ----------------------------------------------------------------------------
 * Représente un participant à la bataille (Humain ou Ordinateur).
 * * Cette structure regroupe TOUT ce qui définit un joueur :
 * - Son identité (Nom, Victoires).
 * - Son état de jeu (ses bateaux, ses grilles).
 * * * Concept des deux grilles :
 * 1. mesNavires : C'est la grille DÉFENSIVE. On y place nos bateaux.
 * L'adversaire tire dessus.
 * 2. mesTirs    : C'est la grille OFFENSIVE (Brouillard de guerre).
 * On y note nos tirs (2=Raté, 3=Touché) pour se souvenir où on a tiré.
 * ============================================================================
 */
typedef struct joueur {
    char NomJoueur[20];    // Pseudo du joueur
    int NbVictoire;        // Compteur de victoires (pour le classement)
    Grille* mesNavires;    // Ma grille avec mes bateaux (Vue complète)
    Grille* mesTirs;       // Ma grille de suivi des tirs (Vue masquée de l'ennemi)
    Bateaux* flotte;       // Tableau de mes 5 bateaux (pour gérer leurs PV)
} Joueur;


/* ============================================================================
 * GESTION MÉMOIRE DES JOUEURS
 * ============================================================================ */

/**
 * @brief Alloue et initialise un nouveau joueur.
 * * Cette fonction :
 * 1. Alloue la structure Joueur.
 * 2. Alloue et initialise les 2 grilles (mesNavires, mesTirs).
 * 3. Alloue et initialise la flotte de 5 bateaux.
 * * @return Joueur* Pointeur vers le joueur créé.
 */
Joueur* creerJoueur(void);

/**
 * @brief Libère toute la mémoire associée à un joueur.
 * * Détruit les grilles, la flotte et la structure joueur elle-même.
 * À appeler obligatoirement en fin de partie.
 * * @param j Pointeur vers le joueur à détruire.
 */
void detruireJoueur(Joueur* j);


/* ============================================================================
 * PHASES DE JEU
 * ============================================================================ */

/**
 * @brief Gère la phase de placement des bateaux pour un joueur.
 * * Boucle sur les 5 bateaux pour demander leur placement via 'positionnerBateau'.
 * * @param j Pointeur vers le joueur qui doit placer sa flotte.
 * * @return int 
 * 1 : Succès (Tous les bateaux sont placés).
 * 0 : Abandon (Le joueur a tapé -1 pour quitter).
 */
int phasePreparation(Joueur* j);

/**
 * @brief Gère un tour complet pour un joueur humain.
 * * Affiche les plateaux, demande les coordonnées de tir, effectue le tir,
 * met à jour les grilles et affiche le résultat (Touché/Coulé/Raté).
 * * @param attaquant Le joueur dont c'est le tour.
 * @param defenseur L'adversaire qui reçoit le tir.
 * * @return int 
 * 0 : Le tour s'est bien passé.
 * 1 : Le joueur veut QUITTER la partie (a tapé -1).
 */
int tourJoueur1vs1(Joueur* attaquant, Joueur* defenseur);

int tourJoueurOrdi(Joueur* attaquant, Joueur* defenseur);

/**
 * @brief Vérifie si un joueur a perdu (toute sa flotte est coulée).
 * * @param j Le joueur dont on vérifie la flotte (l'adversaire généralement).
 * * @return int 1 si le joueur a perdu (tous coulés), 0 sinon.
 */
int verifierVictoire(Joueur* j);

/**
 * @brief Affiche l'interface de jeu pour le joueur actif.
 * * Affiche "MES NAVIRES" (Grille complète) et "MES TIRS" (Grille masquée).
 * * @param j Le joueur dont on affiche les infos.
 */
void afficherPlateauOrdi(Joueur* j);

void afficherPlateau1vs1(Joueur* j);




/* ============================================================================
 * MODES DE JEU (Boucles principales)
 * ============================================================================ */

/**
 * @brief Lance une partie en mode "Joueur contre Joueur" (1v1).
 * * Gère l'alternance des tours, le passage de l'écran ("Passer le terminal"),
 * et la détection de victoire.
 * * @param joueur1 Pointeur vers le joueur 1.
 * @param joueur2 Pointeur vers le joueur 2.
 */
void jouerPartie(Joueur * joueur1, Joueur * joueur2);

/**
 * @brief Lance une partie en mode "Joueur contre Ordinateur".
 * * Gère la boucle de jeu où l'humain joue, puis l'IA répond selon son niveau.
 * * @param humain Le joueur humain.
 * @param difficulte Chaîne de caractères : "facile", "moyen" ou "difficile".
 */
void jouerPartieOrdi(Joueur * humain, char * difficulte);


/* ============================================================================
 * ORIDNATEURS
 * ============================================================================ */

/**
 * @brief IA Niveau 1 : Aléatoire.
 * Tire au hasard sur des cases non encore visées.
 */
void OrdiFacile(Joueur* ordi, Joueur* humain);

/**
 * @brief IA Niveau 2 : Chasseur.
 * - Si un bateau est touché mais non coulé, cible les cases adjacentes.
 * - Sinon, tire au hasard.
 */
void OrdiMoyen(Joueur* ordi, Joueur* humain);

/**
 * @brief IA Niveau 3 : Tricheur (Hardcore).
 * Regarde directement la grille adverse pour trouver les bateaux.
 * Ne rate jamais son coup.
 */
void OrdiDifficile(Joueur* ordi, Joueur* humain);

#endif
