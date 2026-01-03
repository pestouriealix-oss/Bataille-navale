#ifndef _JEU_H_
#define _JEU_H_

#include "librairies.h"
#include "grille.h"

// Pré-déclaration pour éviter les inclusions circulaires
typedef struct bateaux Bateaux;


/**
 * ============================================================================
 * FICHIER : jeu.h
 * ----------------------------------------------------------------------------
 * Ce module est le "Cœur" du programme. Il contient :
 * 1. La structure du Joueur.
 * 2. Les boucles principales de jeu (1vs1, Contre Ordi).
 * 3. La logique des tours (Tirer, Vérifier victoire).
 * 4. L'Intelligence Artificielle (IA).
 * ============================================================================
 */


/* ============================================================================
 * 1. STRUCTURES DE DONNÉES
 * ============================================================================ */

/**
 * @brief Représente un participant à la bataille (Humain ou Ordinateur).
 * * Cette structure regroupe TOUT ce qui définit un joueur :
 * - Son identité (Nom, Victoires).
 * - Son état de jeu (ses bateaux, ses grilles).
 * * * Concept des deux grilles :
 * 1. mesNavires : C'est la grille DÉFENSIVE (Vue complète). On y place nos bateaux.
 * L'adversaire tire dessus.
 * 2. mesTirs    : C'est la grille OFFENSIVE (Brouillard de guerre).
 * On y note nos tirs (2=Raté, 3=Touché) pour se souvenir où on a tiré.
 */
typedef struct joueur {
    char NomJoueur[20];    // Pseudo du joueur
    int NbVictoire;        // Compteur de victoires (pour le classement)
    Grille* mesNavires;    // Ma grille avec mes bateaux
    Grille* mesTirs;       // Ma grille de suivi des tirs
    Bateaux* flotte;       // Tableau de mes 5 bateaux (pour gérer leurs PV)
} Joueur;


/* ============================================================================
 * 2. GESTION MÉMOIRE
 * ============================================================================ */

/**
 * @brief Alloue et initialise un nouveau joueur.
 * * Crée les structures, alloue les grilles (vides) et la flotte (neuve).
 * @return Joueur* Pointeur vers le joueur créé.
 */
Joueur* creerJoueur(void);

/**
 * @brief Libère toute la mémoire associée à un joueur.
 * * Détruit les grilles, la flotte et la structure joueur elle-même.
 * À appeler obligatoirement en fin de partie pour éviter les fuites mémoire.
 * @param j Pointeur vers le joueur à détruire.
 */
void detruireJoueur(Joueur* j);


/* ============================================================================
 * 3. PHASES DE PRÉPARATION & UTILITAIRES
 * ============================================================================ */

/**
 * @brief Gère la phase de placement des bateaux pour un joueur.
 * * Propose le choix entre placement manuel et aléatoire ("random").
 * @param j Pointeur vers le joueur qui doit placer sa flotte.
 * @return int 1 si succès, 0 si le joueur a abandonné.
 */
int phasePreparation(Joueur* j);

/**
 * @brief Vérifie si un joueur a perdu (toute sa flotte est coulée).
 * @param j Le joueur dont on vérifie la flotte (l'adversaire généralement).
 * @return int 1 si le joueur a perdu (tous coulés), 0 sinon.
 */
int verifierVictoire(Joueur* j);

/**
 * @brief Affiche l'interface de jeu complète (Navires + Tirs).
 * Utilisé principalement pour le mode contre l'Ordinateur.
 * @param j Le joueur dont on affiche les infos.
 */
void afficherPlateauOrdi(Joueur* j);

/**
 * @brief Affiche uniquement la grille des tirs.
 * Utilisé pour le mode 1vs1 (pour ne pas voir ses propres bateaux pendant l'attaque).
 * @param j Le joueur dont on affiche les tirs.
 */
void afficherPlateau1vs1(Joueur* j);


/* ============================================================================
 * 4. LOGIQUE DES TOURS
 * ============================================================================ */

/**
 * @brief Gère un tour complet pour un joueur humain en mode 1vs1.
 * * Demande les coordonnées, gère les commandes "save"/"-1", effectue le tir.
 * @param attaquant Le joueur dont c'est le tour.
 * @param defenseur L'adversaire qui reçoit le tir.
 * @return int 0=Continue, 1=Quitter, 2=Sauvegarder.
 */
int tourJoueur1vs1(Joueur* attaquant, Joueur* defenseur);

/**
 * @brief Gère un tour complet pour un joueur humain contre l'IA.
 * * Affiche les deux grilles pour une meilleure visibilité tactique.
 * @param attaquant Le joueur dont c'est le tour.
 * @param defenseur L'ordinateur.
 * @return int 0=Continue, 1=Quitter, 2=Sauvegarder.
 */
int tourJoueurOrdi(Joueur* attaquant, Joueur* defenseur);


/* ============================================================================
 * 5. BOUCLES DE JEU PRINCIPALES
 * ============================================================================ */

/**
 * @brief Lance une partie en mode "Joueur contre Joueur" (1v1).
 * * Gère l'alternance des tours, le passage de l'écran et la victoire.
 * @param j1 Pointeur vers le joueur 1.
 * @param j2 Pointeur vers le joueur 2.
 * @param tourDepart Numéro du tour (1 si nouvelle partie).
 * @param tempsPrecedent Temps déjà joué (0 si nouvelle partie).
 */
void jouerPartie(Joueur *j1, Joueur *j2, int tourDepart, long tempsPrecedent);

/**
 * @brief Lance une partie en mode "Joueur contre Ordinateur".
 * * Gère la boucle de jeu où l'humain joue, puis l'IA répond.
 * @param humain Le joueur humain.
 * @param ordi Le joueur ordinateur (NULL si nouvelle partie).
 * @param difficulte Niveau de l'IA ("facile", "moyen", "difficile").
 * @param tourDepart Numéro du tour.
 * @param tempsPrecedent Temps déjà joué.
 */
void jouerPartieOrdi(Joueur * humain, Joueur * ordi, char* difficulte, int tourDepart, long tempsPrecedent);

/**
 * @brief Reprend une partie sauvegardée.
 * * Identifie le mode de jeu (1vs1 ou Ordi) grâce au paramètre difficulté
 * et relance la bonne boucle de jeu.
 */
void reprendrePartie(Joueur * joueur1, Joueur * joueur2, int tourDeDepart, char* difficulte, long tempsDejaJoue);


/* ============================================================================
 * 6. INTELLIGENCE ARTIFICIELLE
 * ============================================================================ */

/**
 * @brief IA Niveau 1 : Aléatoire.
 * Tire au hasard sur des cases non encore visées.
 */
void OrdiFacile(Joueur* ordi, Joueur* humain);

/**
 * @brief IA Niveau 2 : Chasseur.
 * Si un bateau est touché, cherche autour. Sinon, tire au hasard.
 */
void OrdiMoyen(Joueur* ordi, Joueur* humain);

/**
 * @brief IA Niveau 3 : Sniper.
 * Scan la grille adverse et ne rate jamais.
 */
void OrdiDifficile(Joueur* ordi, Joueur* humain);


/* ============================================================================
 * 7. UTILITAIRES DE SAUVEGARDE (UI)
 * ============================================================================ */

/**
 * @brief Interface utilisateur pour sauvegarder la partie en cours.
 * Demande le nom et le slot de sauvegarde.
 */
void gestionSauvegarde(Joueur* j1, Joueur* j2, int tour, char* difficulte, long tempsTotal);

#endif
