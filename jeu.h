#ifndef _JEU_H_
#define _JEU_H_

#include "librairies.h"

#include "grille.h"
#include "bateau.h"

typedef struct {
    char nom[50];
    Grille* mesNavires;    // Où sont placés MES bateaux
    Grille* mesTirs;       // Où j'ai TIRÉ sur l'adversaire
    Bateaux* flotte;       // Mes 5 bateaux
} Joueur;

// === Gestion des joueurs ===
Joueur* creerJoueur(const char* nom);
void detruireJoueur(Joueur* j);

// === Phase de préparation ===
void phasePreparation(Joueur* j);

// === Déroulement du jeu ===
void tourJoueur(Joueur* attaquant, Joueur* defenseur);
int verifierVictoire(Joueur* j);
void afficherPlateau(Joueur* j);

// === Fonction principale ===
void jouerPartie(void);

#endif
