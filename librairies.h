#ifndef _LIBRAIRIES_H_
#define _LIBRAIRIES_H_

/**
 * ============================================================================
 * FICHIER : librairies.h
 * ----------------------------------------------------------------------------
 * Ce fichier agit comme un "hub" central. Il inclut toutes les bibliothèques
 * standards du C nécessaires au projet et définit les constantes universelles
 * (couleurs, compatibilité Windows/Mac/Linux).
 * ============================================================================
 */


// =================================================================
//                 1. BIBLIOTHEQUES STANDARDS (C)
// =================================================================

/* --- Entrées / Sorties --- */
#include <stdio.h>      // Indispensable pour printf(), scanf(), et la gestion des fichiers (fopen...)

/* --- Gestion Mémoire et Utilitaires --- */
#include <stdlib.h>     // Pour l'allocation dynamique (malloc, free) et l'aléatoire (rand, srand)
#include <stddef.h>     // Pour la définition standard de NULL et size_t

/* --- Manipulation de Données --- */
#include <string.h>     // Pour manipuler les chaînes de caractères (strcpy, strcmp, strlen)
#include <stdint.h>     // Pour avoir des entiers de taille fixe garantie (ex: int32_t) sur tous les OS
#include <stdbool.h>    // Pour utiliser le type 'bool' (true/false) au lieu de 0 et 1

/* --- Mathématiques et Temps --- */
#include <math.h>       // Fonctions mathématiques (si besoin de calculs complexes)
#include <time.h>       // Pour time(), essentiel pour initialiser le générateur aléatoire (srand)

/* --- Débogage --- */
#include <assert.h>     // Permet d'utiliser assert() pour stopper le programme si une condition critique est fausse


// =================================================================
//                 2. COMPATIBILITÉ SYSTEME (OS)
// =================================================================
/*
 * Ce bloc permet au code de fonctionner aussi bien sur Windows que sur Mac/Linux.
 * Le compilateur détecte automatiquement l'OS et choisit les bonnes commandes.
 */

#ifdef _WIN32
    // --- Configuration pour WINDOWS ---
    #include <windows.h>
    
    // Convertit la pause en secondes pour Windows (Sleep est en ms)
    #define SLEEP(x) Sleep((x) * 1000)
    
    // Commande système pour nettoyer le terminal
    #define EFFACE "cls"

#else
    // --- Configuration pour MAC / LINUX ---
    #include <unistd.h>
    
    // sleep() est déjà en secondes sur Unix
    #define SLEEP(x) sleep(x)
    
    // Code ANSI pour nettoyer le terminal (plus rapide que system("clear"))
    #define EFFACE "\033[H\033[J"
#endif


// =================================================================
//                 3. COULEURS & DESIGN (ANSI)
// =================================================================
/*
 * Codes d'échappement ANSI pour colorer le texte dans le terminal.
 * Format : "\033[xm" où x est le code couleur.
 */

// --- COULEURS DU TEXTE ---
#define RESET    "\033[0m"      // Réinitialise la couleur par défaut
#define NOIR     "\033[30m"
#define ROUGE    "\033[31m"     // Utilisé pour les touchés / erreurs
#define VERT     "\033[32m"     // Utilisé pour les bateaux / succès
#define JAUNE    "\033[33m"     // Utilisé pour les titres / infos
#define BLEU     "\033[34m"     // Utilisé pour l'eau
#define MAGENTA  "\033[35m"
#define CYAN     "\033[36m"
#define BLANC    "\033[37m"
#define CIEL     "\033[36m"     // Alias pour Cyan
#define BLEUGRIS "\033[90m"     // Alias gris clair

// --- COULEURS DE FOND (Background) ---
#define BG_NOIR  "\033[40m"
#define BG_ROUGE "\033[41m"     // Fond rouge (Coup touché)
#define BG_VERT  "\033[42m"     // Fond vert (Bateau intact)
#define BG_BLEU  "\033[44m"     // Fond bleu (Mer)

#endif // Fin de _LIBRAIRIES_H_
