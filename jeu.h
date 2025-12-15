#ifndef _JEU_H_
#define _JEU_H_

#include "librairies.h"

#include "grille.h"
#include "bateau.h"

typedef struct {
    char NomJoueur[20];
    int NbVictoire;
    Grille* mesNavires;    // Où sont placés MES bateaux
    Grille* mesTirs;       // Où j'ai TIRÉ sur l'adversaire
    Bateaux* flotte;       // Mes 5 bateaux
} Joueur;

// === Gestion des joueurs ===
Joueur* creerJoueur(void);
void detruireJoueur(Joueur* j);

// === Phase de préparation ===
void phasePreparation(Joueur* j);

// === Déroulement du jeu ===
void tourJoueur(Joueur* attaquant, Joueur* defenseur);
int verifierVictoire(Joueur* j);
void afficherPlateau(Joueur* j);

// === Fonction principale ===
void jouerPartie(Joueur * joueur1, Joueur * joueur2);

#endif

