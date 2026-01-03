#ifndef AFFICHAGE_H
#define AFFICHAGE_H

#include <stdio.h>
#include "jeu.h" // Nécessaire pour la structure Joueur

/**
 * @file affichage.h
 * @brief Gère toute l'interface utilisateur, les menus et les rendus visuels.
 * @author Groupe Bataille Navale
 */

// =================================================================
//                    1. MENUS & NAVIGATION
// =================================================================

/**
 * @brief Affiche le menu principal et gère la navigation initiale.
 * Affiche le logo ASCII et les options (Jouer, Charger, Règles, etc.).
 */
void MenuPrincipal(void);

/**
 * @brief Menu de sélection du mode de jeu (1vs1 ou Contre Ordi).
 */
void ModeDeJeu(void);

/**
 * @brief Menu de sélection de la difficulté pour le mode PvE.
 */
void ChoixDifficulte(void);

/**
 * @brief Affiche les règles du jeu et attend une validation.
 */
void ReglesDuJeu(void);

/**
 * @brief Affiche l'écran de victoire/fin de partie.
 * @param j Pointeur vers le joueur qui a gagné.
 */
void MenuFinDePartie(Joueur *j);


// =================================================================
//                    2. CONFIGURATION & LANCEMENT
// =================================================================

/**
 * @brief Demande les noms pour une partie 1vs1 et lance la partie.
 */
void NomDesJoueurs(void);

/**
 * @brief Demande le nom du joueur pour une partie contre l'Ordi et lance la partie.
 * @param difficulte Chaîne de caractères représentant le niveau ("facile", "moyen", "difficile").
 */
void NomDuJoueur(char difficulte[15]);


// =================================================================
//                    3. INTERFACE EN JEU (HUD)
// =================================================================

/**
 * @brief Affiche le bandeau d'informations en haut de l'écran (HUD).
 * @param tour Numéro du tour actuel.
 * @param temps Temps écoulé en secondes depuis le début.
 * @param nomJoueur Nom du joueur dont c'est le tour.
 */
void AfficherHUD(int tour, long temps, char* nomJoueur);


// =================================================================
//                    4. GESTION DU CLASSEMENT
// =================================================================

/**
 * @brief Menu principal des classements (choix entre 1vs1 et Ordi).
 */
void Classement(void);

/**
 * @brief Affiche le classement spécifique au mode 1vs1.
 */
void Classement1vs1(void);

/**
 * @brief Menu pour choisir la difficulté du classement Ordi à afficher.
 */
void ChoixClassementOrdi(void);

/**
 * @brief Affiche le classement pour une difficulté donnée contre l'Ordi.
 * @param difficulte Niveau de difficulté ("facile", "moyen", "difficile").
 */
void ClassementOrdi(char *difficulte);

/**
 * @brief Lit un fichier de scores et affiche le tableau formaté.
 * Trie les joueurs par nombre de victoires (Tri à bulles).
 * @param f Pointeur vers le fichier ouvert en lecture.
 */
void AfficherClassement(FILE * f);

/**
 * @brief Ajoute ou met à jour une victoire dans le fichier de sauvegarde Ordi.
 * @param j Le joueur gagnant.
 * @param difficulte La difficulté concernée.
 */
void AjouterVictoireOrdi(Joueur * j, char *difficulte);

/**
 * @brief Ajoute ou met à jour une victoire dans le fichier de sauvegarde 1vs1.
 * @param j Le joueur gagnant.
 */
void AjouterVictoire1vs1(Joueur * j);


// =================================================================
//                    5. EFFETS VISUELS
// =================================================================

/**
 * @brief Affiche du texte lettre par lettre (Effet machine à écrire).
 * @param texte La chaîne de caractères à afficher.
 */
void afficherLentement(char* texte);

#endif // AFFICHAGE_H
