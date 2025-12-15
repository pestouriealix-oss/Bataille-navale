#include "librairies.h"
#include "jeu.h"
#include "grille.h"
#include "bateau.h"
#include "affichage.h"

Joueur* creerJoueur(void){
    Joueur* j = malloc(sizeof(Joueur));
    
    // Créer les 2 grilles
    j->mesNavires = initialiser();  // Grille de mes bateaux
    j->mesTirs = initialiser();     // Grille de mes tirs
    
    // Créer la flotte
    j->flotte = creerFlotte();

    return j;
}

void detruireJoueur(Joueur* j)
{
    if (j != NULL) {
        detruireGrille(j->mesNavires);
        detruireGrille(j->mesTirs);
        detruireFlotte(j->flotte);
        free(j);
    }
}

void phasePreparation(Joueur* j)
{
    printf(EFFACE);
    printf("\n");
    printf("========================================\n");
    printf(CIEL "       %s, placez vos bateaux !\n" RESET, j->NomJoueur);
    printf("========================================\n");
    
    // Placer les 5 bateaux un par un
    for (int i = 0; i < NB_BATEAUX; i++) {
        printf("\nBateau %d/%d\n", i + 1, NB_BATEAUX);
        afficherCoordonnees(j->mesNavires);
        
        int resultat = positionnerBateau(&j->flotte[i], j->mesNavires);
        
        if (resultat == 0) {
            printf("Placement annulé. Recommencez ce bateau.\n");
            i--;  // Recommencer ce bateau
        }
    }
    
    printf("\n✅ Tous vos bateaux sont placés !\n");
    afficherCoordonnees(j->mesNavires);
    
    printf("\nAppuyez sur Entrée pour continuer...");
    getchar();
    getchar();  
}

void afficherPlateau(Joueur* j)
{
    printf("\n=== MES NAVIRES ===\n");
    afficherCoordonnees(j->mesNavires);
    
    printf("\n=== MES TIRS ===\n");
    afficherCoordonnees(j->mesTirs);
}

void tourJoueur(Joueur* attaquant, Joueur* defenseur)
{
    int ligne, colonne;
    int resultat;
    
    printf("\n");
    printf("========================================\n");
    printf(BLEUGRIS "              Tour de %s\n" RESET, attaquant->NomJoueur);
    printf("========================================\n");
    
    afficherPlateau(attaquant);
    
    // Demander les coordonnées jusqu'à ce qu'elles soient valides
    while (1) {
        printf("\nEntrez les coordonnées de tir (ligne colonne) : ");
        scanf("%d %d", &ligne, &colonne);
        
        // Convertir de 1-10 à 0-9
        ligne = ligne - 1;
        colonne = colonne - 1;
        
        // Tirer sur la grille de l'adversaire
        resultat = tirer(defenseur->mesNavires, ligne, colonne);
        
        if (resultat == -2) {
            printf("❌ Hors de la grille ! Réessayez.\n");
            continue;
        }
        
        if (resultat == -1) {
            printf("❌ Vous avez déjà tiré ici ! Réessayez.\n");
            continue;
        }
        
        // Tir valide, on sort de la boucle
        break;
    }
    
    // Enregistrer le tir sur MA grille de tirs
    attaquant->mesTirs->tab[ligne][colonne] = defenseur->mesNavires->tab[ligne][colonne];
    
    // Analyser le résultat
    if (resultat == 0) {
        printf("\n💦 PLOUF ! Dans l'eau...\n");
    } else if (resultat == 1) {
        printf("\n💥 TOUCHÉ !\n");
        
        // Trouver quel bateau a été touché
        int index = trouverBateau(defenseur->flotte, ligne, colonne);
        
        if (index != -1) {
            // Enregistrer la touche
            enregistrerTouche(&defenseur->flotte[index]);
            
            // Vérifier si coulé
            if (estCoule(&defenseur->flotte[index])) {
                printf("🚢 COULÉ ! Vous avez coulé le %s !\n", 
                       defenseur->flotte[index].nom);
            }
        }
    }
    
    printf("\nAppuyez sur Entrée pour continuer...");
    getchar();
    getchar();
}

int verifierVictoire(Joueur* adversaire)
{
    return tousCoules(adversaire->flotte);
}


void jouerPartie(Joueur * joueur1, Joueur * joueur2)
{
    printf(EFFACE);
    printf("\n========================================\n");
    printf(CIEL "           LA BATAILLE NAVALE" RESET);
    printf(BLEUGRIS "\n       made by Julien, Alix, Enzo" RESET);
    printf("\n========================================\n");
    
    
    // Phase de préparation
    phasePreparation(joueur1);
    
    printf("\n\n>>> Passez le terminal à %s <<<\n", joueur2 -> NomJoueur);
    printf("Appuyez sur Entrée ...");
    getchar();
    
    phasePreparation(joueur2);
    
    // Boucle de jeu
    int tourNumero = 1;
    Joueur * gagnant = NULL;
    while (gagnant == NULL) {
        printf(EFFACE);
        printf("\n\n");
        printf("========================================\n");
        printf(BLEUGRIS "           TOUR %d\n" RESET, tourNumero);
        printf("========================================\n");
        
        // Tour du joueur 1
        tourJoueur(joueur1, joueur2);
        
        // Vérifier victoire
        if (verifierVictoire(joueur2)) {
            gagnant = joueur1;
            break;
        }
        
        printf("\n>>> Passez le terminal à %s <<<", joueur2 -> NomJoueur);
        printf("\nAppuyez sur Entrée...");
        getchar();
        
        // Tour du joueur 2
        tourJoueur(joueur2, joueur1);
        
        // Vérifier victoire
        if (verifierVictoire(joueur1)) {
            gagnant = joueur2;
            break;
        }
        
        printf("\n>>> Passez le terminal à %s <<<", joueur1 -> NomJoueur);
        printf("\nAppuyez sur Entrée...");
        getchar();
        
        tourNumero++;
    }
    
    // Nettoyage
    detruireJoueur(joueur1);
    detruireJoueur(joueur2);
    AjouterVictoire1c1(joueur1);
    MenuFinDePartie(gagnant);
}

