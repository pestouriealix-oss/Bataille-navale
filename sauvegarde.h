#ifndef _SAUVEGARDE_H_
#define _SAUVEGARDE_H_

#include "librairies.h"
#include "jeu.h" // Nécessaire pour connaître la structure Joueur

/**
 * ============================================================================
 * FICHIER : sauvegarde.h
 * ----------------------------------------------------------------------------
 * Gère la persistance des données : sauvegarde de la partie en cours dans un
 * fichier binaire et rechargement de celle-ci.
 * ============================================================================
 */

/** Nombre maximum d'emplacements de sauvegarde (slots) prévus par l'interface */
#define NB_SLOTS 20


/* ============================================================================
 * 1. FONCTIONS DE SAUVEGARDE
 * ============================================================================ */

/**
 * @brief Sauvegarde l'état complet de la partie dans un fichier binaire.
 * * Enregistre dans l'ordre :
 * 1. Les métadonnées (Nom de la partie, Temps de jeu, Difficulté, Tour actuel).
 * 2. Les données complètes du Joueur 1 (Navires, Tirs, Flotte, Victoires).
 * 3. Les données complètes du Joueur 2.
 * * @param nomFichier Chemin/Nom du fichier cible (ex: "savegame.dat").
 * @param j1 Pointeur vers le Joueur 1.
 * @param j2 Pointeur vers le Joueur 2 (ou l'Ordinateur).
 * @param tourActuel Numéro du tour en cours.
 * @param difficulte Niveau de difficulté ("1vs1", "facile", "moyen", "difficile").
 * @param nomPartie Nom donné à la sauvegarde par l'utilisateur.
 * @param tempsJeu Temps écoulé en secondes depuis le début.
 * * @return int 1 si succès, 0 si échec (ouverture fichier impossible).
 */
int sauvegarderPartie(char* nomFichier, Joueur* j1, Joueur* j2, int tourActuel, char* difficulte, char* nomPartie, long tempsJeu);


/* ============================================================================
 * 2. FONCTIONS DE CHARGEMENT
 * ============================================================================ */

/**
 * @brief Charge une partie complète depuis un fichier.
 * * Alloue la mémoire pour les joueurs et remplit toutes les structures
 * avec les données lues dans le fichier.
 * * @param nomFichier Nom du fichier à charger.
 * @param j1 Adresse du pointeur du Joueur 1 (sera alloué à l'intérieur).
 * @param j2 Adresse du pointeur du Joueur 2 (sera alloué à l'intérieur).
 * @param difficulte Tampon pour récupérer la difficulté.
 * @param nomPartie Tampon pour récupérer le nom de la partie.
 * @param tempsJeu Pointeur pour récupérer le temps de jeu.
 * * @return int Le numéro du tour chargé, ou -1 en cas d'erreur.
 */
int chargerPartie(char* nomFichier, Joueur** j1, Joueur** j2, char* difficulte, char* nomPartie, long* tempsJeu);

/**
 * @brief Lit uniquement les métadonnées d'une sauvegarde (pour l'affichage menu).
 * * Permet d'afficher "Nom de la partie - 5 min" dans le menu sans avoir
 * à charger toute la grosse structure des joueurs en mémoire.
 * * @param nomFichier Fichier à analyser.
 * @param nomPartie Tampon de sortie pour le nom.
 * @param tempsJeu Pointeur de sortie pour le temps.
 * @param difficulte Tampon de sortie pour la difficulté.
 */
void lireInfosSauvegarde(char* nomFichier, char* nomPartie, long* tempsJeu, char* difficulte);

#endif
