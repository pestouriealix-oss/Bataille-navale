#ifndef _LIBRAIRIES_H_
#define _LIBRAIRIES_H_

/**
 * ============================================================================
 * FICHIER : librairies.h
 * ----------------------------------------------------------------------------
 * Ce fichier agit comme un "hub" central pour toutes les bibliothèques
 * standards du C nécessaires au projet.
 * ============================================================================
 */

/* --- Entrées / Sorties --- */
#include <stdio.h>      // Indispensable pour printf(), scanf(), et la gestion des fichiers (fopen, fprintf...)

/* --- Gestion Mémoire et Utilitaires --- */
#include <stdlib.h>     // Pour l'allocation dynamique (malloc, free) et l'aléatoire (rand, srand)
#include <stddef.h>     // Pour la définition de NULL et size_t

/* --- Manipulation de Données --- */
#include <string.h>     // Pour manipuler les noms des joueurs et commandes (strcpy, strcmp, strlen)
#include <stdint.h>     // Pour des types d'entiers précis (int8_t, int32_t) - Bonnes pratiques
#include <stdbool.h>    // Pour utiliser les types booléens (bool, true, false) au lieu de 0 et 1

/* --- Mathématiques et Temps --- */
#include <math.h>       // Fonctions mathématiques (si besoin de calculs complexes)
#include <time.h>       // Pour time(), essentiel pour initialiser le générateur aléatoire (srand)

/* --- Débogage --- */
#include <assert.h>     // Pour utiliser assert() lors du développement (vérifier des conditions critiques)

#endif
