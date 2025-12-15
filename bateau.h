#ifndef _BATEAU_H_
#define _BATEAU_H_

#include "librairies.h"
#include "grille.h"

#define NB_BATEAUX 5

struct bateaux{
    char nom[20];
    int ligne;
    int colonne;
    int taille; 
    char orientation; // H ou V
    int touche;
};

typedef struct bateaux Bateaux;

//Initialisation 
Bateaux* creerFlotte(void);

void detruireFlotte (Bateaux*);

Bateaux* creerBateaux(void);

void detruireBateaux(Bateaux*);

//Placement et validation
int positionnerBateau(Bateaux*, Grille*);

//Gestion des touches
void enregistrerTouche(Bateaux*);

int estCoule(Bateaux*);

int estTouche(Bateaux*);

char obtenirEtat(Bateaux*);

int appartientAuBateau(Bateaux* b, int ligne, int colonne);

int tousCoules(Bateaux* flotte);

int trouverBateau(Bateaux* flotte, int ligne, int colonne);

#endif
