#ifndef _AFFICHAGE_H_
#define _AFFICHAGE_H_

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"jeu.h"


#define RESET "\033[0m"
#define CIEL   "\033[38;5;75m"
#define BLEUGRIS "\033[38;5;67m"
#define CYAN "\033[38;5;51m"
#define EFFACE "\033[2J\033[H"

void MenuPrincipal(void);
void ReglesDuJeu(void);
void ModeDeJeu(void);
void Classement(void);
void NomDesJoueurs(void);
void NomDuJoueur(char difficulte[15]);
void AjouterVictoireOrdi(Joueur * j, char difficulte[15]);
void AjouterVictoire1c1(Joueur * j);
void ChoixClassementOrdi(void);
void Classement1c1(void);
void AfficherClassement(FILE * f);
int ComparerScores(const void * a, const void * b);
void MenuFinDePartie(Joueur * j);
void ChoixDifficulte(void);
void ClassementOrdi(char difficulte[15]);
int Appartient(FILE * f, char NomJoueur[]);

#endif
