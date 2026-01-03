#include "librairies.h"  // Doit être en premier pour les macros OS et Couleurs
#include "jeu.h"
#include "grille.h"
#include "bateau.h"
#include "affichage.h"
#include "sauvegarde.h"

// Gestion de la pause (Sleep) selon l'OS
#ifdef _WIN32
    #include <windows.h>
    #define SLEEP(x) Sleep((x) * 1000)
#else
    #include <unistd.h>
    #define SLEEP(x) sleep(x) 
#endif


/* ============================================================================
 * 1. GESTION MÉMOIRE DES JOUEURS
 * ============================================================================ */

/* Alloue un joueur, ses grilles et sa flotte */
Joueur* creerJoueur(void){
    // 1. Allocation de la structure principale
    Joueur* j = malloc(sizeof(Joueur));
    
    // 2. Allocation et initialisation des grilles (remplies d'eau)
    j->mesNavires = initialiser();  // Grille défensive
    j->mesTirs = initialiser();     // Grille offensive (brouillard)
    
    // 3. Création de la flotte (5 bateaux par défaut)
    j->flotte = creerFlotte();

    return j;
}

/* Libère proprement toute la mémoire d'un joueur */
void detruireJoueur(Joueur* j)
{
    if (j != NULL) {
        detruireGrille(j->mesNavires);
        detruireGrille(j->mesTirs);
        detruireFlotte(j->flotte);
        free(j);
    }
}


/* ============================================================================
 * 2. PHASE DE PRÉPARATION
 * ============================================================================ */

/* Gère le placement des bateaux (Manuel ou Aléatoire) */
int phasePreparation(Joueur* j) 
{
    char reponse[15];
    
    // Boucle pour forcer un choix valide
    while((strcmp(reponse, "random") != 0) && (strcmp(reponse, "non") != 0)){
        printf(EFFACE);
        printf("\n");
        printf("========================================\n");
        printf(CIEL "       %s, placez vos bateaux !\n" RESET, j->NomJoueur);
        printf("========================================\n");
        printf("\nPour placer les bateaux aléatoirement, tapez " CYAN "random" RESET);
        printf("\nSinon, tapez " CYAN "non" RESET);
        printf("\n\nVotre reponse : ");
        scanf(" %s", reponse);
    }

    // --- CAS 1 : PLACEMENT ALÉATOIRE ---
    if(strcmp(reponse, "random") == 0){
        placerBateauxAleatoire(j);
    }
    // --- CAS 2 : PLACEMENT MANUEL ---
    else{
        // On demande le placement pour chacun des 5 bateaux
        for (int i = 0; i < NB_BATEAUX; i++) {
            printf(EFFACE);
            printf("\n");
            printf("========================================\n");
            printf(CIEL "       %s, placez vos bateaux !\n" RESET, j->NomJoueur);
            printf("========================================\n");
            printf("\nBateau %d/%d\n", i + 1, NB_BATEAUX);
            afficherCoordonnees(j->mesNavires);
        
            // Appel interactif (position, orientation)
            int resultat = positionnerBateau(&j->flotte[i], j->mesNavires);
        
            // Si le joueur tape -1, on annule tout
            if (resultat == 0) {
                printf("🚫 Placement annulé. Retour au menu...\n");
                return 0; 
            }
        }

        // Récapitulatif final
        printf(EFFACE);
        printf("\n");
        printf("========================================\n");
        printf(CIEL "                 %s\n" RESET, j->NomJoueur);
        printf("========================================\n");
        
        printf("\n✅ Tous vos bateaux sont placés !\n");
        afficherCoordonnees(j->mesNavires);
        
        printf("\nAppuyez sur Entrée pour continuer...");
        getchar();
    }

    return 1; // Succès
}

/* Affiche les deux grilles (Pour le mode Vs Ordi) */
void afficherPlateauOrdi(Joueur* j)
{
    printf("\n=== MES NAVIRES ===\n");
    afficherCoordonnees(j->mesNavires);
    
    printf("\n=== MES TIRS ===\n");
    afficherCoordonnees(j->mesTirs);
}

/* Affiche uniquement les tirs (Pour le mode 1vs1) */
void afficherPlateau1vs1(Joueur* j)
{   
    printf("\n            === MES TIRS ===\n");
    afficherCoordonnees(j->mesTirs);
}


/* ============================================================================
 * 3. LOGIQUE DES TOURS
 * ============================================================================ */

/* Tour d'un joueur humain (Saisie, Tir, Résultat) */
int tourJoueur1vs1(Joueur* attaquant, Joueur* defenseur)
{
    int ligne, colonne;
    char conversion;
    int resultat;
    char buffer[10]; 
    
    printf("\n");
    printf("========================================\n");
    printf(BLEUGRIS "              Tour de %s\n" RESET, attaquant->NomJoueur);
    printf("========================================\n");
    
    afficherPlateau1vs1(attaquant);
    
    // Boucle de validation de la saisie
    while (1) {
        printf("\nEntrez une case (ex: A5), '-1' pour quitter, ou " CYAN "save" RESET " pour sauvegarder : ");
        scanf("%s", buffer);

        // --- COMMANDES SPÉCIALES ---
        if (strcmp(buffer, "-1") == 0) return 1;   // Quitter
        if (strcmp(buffer, "save") == 0) return 2; // Sauvegarder

        // --- VALIDATION FORMAT ---
        if (strlen(buffer) < 2) {
            printf("❌ Format invalide ! Utilisez le format A8.\n");
            continue;
        }

        // --- CONVERSION (A5 -> Ligne 4, Col 0) ---
        conversion = buffer[0];
        if (conversion >= 'a' && conversion <= 'j') conversion -= 32; // Minuscule -> Majuscule
        
        colonne = conversion - 'A';
        ligne = atoi(&buffer[1]) - 1; 

        // --- ACTION DE TIR ---
        resultat = tirer(defenseur->mesNavires, ligne, colonne);
        
        if (resultat == -2) {
            printf("❌ Hors de la grille ! Réessayez.\n");
            continue;
        }
        if (resultat == -1) {
            printf("❌ Vous avez déjà tiré ici ! Réessayez.\n");
            continue;
        }
        
        break; // Tir valide, on sort
    }
    
    // --- MISE À JOUR DU BROUILLARD DE GUERRE ---
    attaquant->mesTirs->tab[ligne][colonne] = defenseur->mesNavires->tab[ligne][colonne];
    
    // --- AFFICHAGE DU RÉSULTAT ---
    if (resultat == 0) {
        printf(BLEUGRIS "\n💦 PLOUF ! Dans l'eau...\n" RESET);
    } 
    else if (resultat == 1) {
        printf(VERT "\n💥 TOUCHÉ !\n" RESET);
        
        // Gestion des dégâts sur la flotte adverse
        int index = trouverBateau(defenseur->flotte, ligne, colonne);
        if (index != -1) {
            enregistrerTouche(&defenseur->flotte[index]);
            
            if (estCoule(&defenseur->flotte[index])) {
                printf(ROUGE "🚢 COULÉ ! Vous avez coulé le %s !\n" RESET, defenseur->flotte[index].nom);
            }
        }
    }
    
    printf("\nAppuyez sur Entrée pour continuer...");
    getchar(); getchar(); 

    return 0; // Le tour continue
}


/* Tour d'un joueur humain contre l'ordi (Interface différente) */
int tourJoueurOrdi(Joueur* attaquant, Joueur* defenseur)
{
    int ligne, colonne;
    char conversion;
    int resultat;
    
    printf("\n");
    printf("========================================\n");
    printf(BLEUGRIS "              Tour de %s\n" RESET, attaquant->NomJoueur);
    printf("========================================\n");
    
    // On affiche TOUT (Bateaux + Tirs) pour aider le joueur
    afficherPlateauOrdi(attaquant);
    
    while (1) {
        char buffer[10]; 
        
        printf("\nEntrez une case (ex: A5), '-1' pour quitter, ou " CYAN "save" RESET " pour sauvegarder : ");
        scanf("%s", buffer);

        if (strcmp(buffer, "-1") == 0) return 1; 
        if (strcmp(buffer, "save") == 0) return 2;

        if (strlen(buffer) < 2) {
            printf("❌ Format invalide.\n");
            continue;
        }

        conversion = buffer[0];
        if (conversion >= 'a' && conversion <= 'j') conversion -= 32;
        colonne = conversion - 'A';
        ligne = atoi(&buffer[1]) - 1;

        resultat = tirer(defenseur->mesNavires, ligne, colonne);
        
        if (resultat == -2) { printf("❌ Hors grille !\n"); continue; }
        if (resultat == -1) { printf("⚠️ Déjà tiré ici !\n"); continue; }
        
        break; 
    }
    
    attaquant->mesTirs->tab[ligne][colonne] = defenseur->mesNavires->tab[ligne][colonne];
    
    if (resultat == 0) {
        printf(BLEUGRIS "\n💦 PLOUF ! Dans l'eau...\n" RESET);
    } else if (resultat == 1) {
        printf(VERT "\n💥 TOUCHÉ !\n" RESET);

        int index = trouverBateau(defenseur->flotte, ligne, colonne);
        if (index != -1) {
            enregistrerTouche(&defenseur->flotte[index]);
            if (estCoule(&defenseur->flotte[index])) {
                printf(ROUGE "🚢 COULÉ ! Vous avez coulé le %s !\n" RESET, defenseur->flotte[index].nom);
            }
        }
    }
    
    printf("\nAppuyez sur Entrée pour continuer...");
    getchar(); getchar();

    return 0;
}


/* ============================================================================
 * 4. BOUCLES DE JEU
 * ============================================================================ */

/* Utilitaires pour vérifier si la partie est finie */
int verifierVictoire(Joueur* adversaire)
{
    return tousCoules(adversaire->flotte);
}


/* Boucle principale : MODE 1 VS 1 */
void jouerPartie(Joueur *j1, Joueur *j2, int tourDepart, long tempsPrecedent) {
    int tour = tourDepart;
    
    // Initialisation du chrono
    time_t debutSession = time(NULL);
    long tempsTotal = 0;

    // --- VÉRIFICATION : Est-ce une nouvelle partie ? ---
    // On vérifie si le J1 a des bateaux placés
    int j1_pret = 0;
    for(int i=0; i<10; i++) for(int k=0; k<10; k++) if(j1->mesNavires->tab[i][k] == 1) j1_pret = 1;

    if (j1_pret == 0) {
        // Nouvelle partie : Placement des bateaux
        if(phasePreparation(j1)==0) { detruireJoueur(j1); detruireJoueur(j2); MenuPrincipal(); return; }
        printf(EFFACE); printf("\nTour du Joueur 2...\n"); getchar();
        if(phasePreparation(j2)==0) { detruireJoueur(j1); detruireJoueur(j2); MenuPrincipal(); return; }
    } else {
        // Reprise : Vérification immédiate (si la save était juste avant la fin)
        if (verifierVictoire(j2)) { MenuFinDePartie(j1); detruireJoueur(j1); detruireJoueur(j2); return; }
        if (verifierVictoire(j1)) { MenuFinDePartie(j2); detruireJoueur(j1); detruireJoueur(j2); return; }
    }

    // --- BOUCLE DE JEU ---
    while (1) {
        tempsTotal = tempsPrecedent + (long)difftime(time(NULL), debutSession);
        
        // --- TOUR JOUEUR 1 ---
        printf(EFFACE); 
        AfficherHUD(tour, tempsTotal, j1->NomJoueur);
        
        int s = tourJoueur1vs1(j1, j2);
        
        // Gestion des arrêts
        if(s==1) break; // Quitter
        if(s==2) { gestionSauvegarde(j1, j2, tour, "1vs1", tempsTotal); break; } // Sauver
        
        // Vérif Victoire
        if(verifierVictoire(j2)) { 
            AjouterVictoire1vs1(j1); MenuFinDePartie(j1); 
            detruireJoueur(j1); detruireJoueur(j2); return;
        }

        printf(EFFACE); printf("Passez à %s...", j2->NomJoueur); getchar();

        // --- TOUR JOUEUR 2 ---
        printf(EFFACE); 
        AfficherHUD(tour, tempsTotal, j2->NomJoueur);
        
        s = tourJoueur1vs1(j2, j1);
        
        if(s==1) break;
        if(s==2) { gestionSauvegarde(j1, j2, tour, "1vs1", tempsTotal); break; }
        
        if(verifierVictoire(j1)) {
            AjouterVictoire1vs1(j2); MenuFinDePartie(j2);
            detruireJoueur(j1); detruireJoueur(j2); return;
        }
        
        printf(EFFACE); printf("Passez à %s...", j1->NomJoueur); getchar();
        tour++;
    }
    // Si on sort de la boucle (Quitter/Save), on nettoie
    detruireJoueur(j1); detruireJoueur(j2); MenuPrincipal();
}


/* Boucle principale : MODE CONTRE ORDI */
void jouerPartieOrdi(Joueur * humain, Joueur * ordi, char* difficulte, int tourDepart, long tempsPrecedent)
{
    int tourNumero = tourDepart;
    int stop = 0;
    
    time_t debutSession = time(NULL); 
    long tempsTotal = 0;

    // --- INITIALISATION ---
    if (ordi == NULL) {
        // Nouvelle partie : Création de l'ordi
        ordi = creerJoueur();
        strcpy(ordi->NomJoueur, "Ordinateur");

        // Placement du joueur humain
        if (phasePreparation(humain) == 0) { 
            detruireJoueur(humain); detruireJoueur(ordi); 
            MenuPrincipal(); return; 
        }
        
        // Placement de l'IA (automatique)
        printf("L'ordinateur place ses bateaux...\n");
        placerBateauxAleatoire(ordi); 
        
        // Petit effet visuel de chargement
        printf("\n" CYAN "Initialisation des systèmes tactiques..." RESET "\n");
        printf(BLANC "[");
        for(int i=0; i<20; i++) {
            printf("="); fflush(stdout);
            #ifdef _WIN32
                Sleep(50);
            #else
                usleep(50000);
            #endif
        }
        printf("] 100%%\n" RESET);
        SLEEP(1);
    } else {
        // Reprise de sauvegarde
        if (verifierVictoire(ordi)) {
            MenuFinDePartie(humain); detruireJoueur(humain); detruireJoueur(ordi); return;
        }
    }

    // --- BOUCLE DE JEU ---
    while (1) {
        tempsTotal = tempsPrecedent + (long)difftime(time(NULL), debutSession);
        
        printf(EFFACE);
        AfficherHUD(tourNumero, tempsTotal, humain->NomJoueur);

        // --- TOUR HUMAIN ---
        stop = tourJoueurOrdi(humain, ordi); 

        // Gestion Arrêts
        if (stop == 1) { 
            detruireJoueur(humain); detruireJoueur(ordi); MenuPrincipal(); return; 
        }
        if (stop == 2) { 
            gestionSauvegarde(humain, ordi, tourNumero, difficulte, tempsTotal);
            detruireJoueur(humain); detruireJoueur(ordi); MenuPrincipal(); return; 
        }

        // Vérif Victoire Humain
        if (verifierVictoire(ordi)) {
            AjouterVictoireOrdi(humain, difficulte);
            MenuFinDePartie(humain);
            detruireJoueur(humain); detruireJoueur(ordi);
            return;
        }

        // --- TOUR ORDI ---
        printf("\n");
        afficherLentement(ROUGE "[!] L'Ordinateur calcule sa trajectoire..." RESET);
        SLEEP(1); 

        // Appel de l'IA selon la difficulté
        if (strcmp(difficulte, "facile") == 0) OrdiFacile(ordi, humain);
        else if (strcmp(difficulte, "moyen") == 0) OrdiMoyen(ordi, humain);
        else OrdiDifficile(ordi, humain);

        // Vérif Victoire Ordi
        if (verifierVictoire(humain)) {
            printf(ROUGE "\n💀 VOUS AVEZ PERDU !\n" RESET);
            SLEEP(3);
            detruireJoueur(humain); detruireJoueur(ordi);
            MenuPrincipal();
            return;
        }

        tourNumero++;
        printf("\nAppuyez sur Entrée...");
        getchar(); 
    }
}


/* Reprise générique d'une sauvegarde */
void reprendrePartie(Joueur * joueur1, Joueur * joueur2, int tourDeDepart, char* difficulte, long tempsDejaJoue)
{
    int tourNumero = tourDeDepart;
    Joueur * gagnant = NULL;
    int stop = 0; 

    time_t debutSession = time(NULL); 
    long tempsTotalActuel = tempsDejaJoue;

    printf(EFFACE);
    printf(VERT "\n✅ Partie reprise (Mode : %s) !\n" RESET, difficulte);
    printf("⏱️  Temps de jeu précédent : %ld min %ld sec\n", tempsDejaJoue / 60, tempsDejaJoue % 60);
    printf("Appuyez sur Entrée pour commencer le combat...");
    getchar(); getchar(); 

    while (gagnant == NULL) {
        
        tempsTotalActuel = tempsDejaJoue + (long)difftime(time(NULL), debutSession);

        printf(EFFACE);
        AfficherHUD(tourNumero, tempsTotalActuel, joueur1->NomJoueur);

        // --- TOUR J1 (ou Humain) ---
        if (strcmp(difficulte, "1vs1") == 0) {
            printf("\n>>> C'est au tour de %s <<<\n", joueur1->NomJoueur);
            stop = tourJoueur1vs1(joueur1, joueur2);
        } else {
            stop = tourJoueurOrdi(joueur1, joueur2); 
        }

        if (stop == 1) { // Abandon
            char rep[10];
            printf(CIEL "\nVoulez-vous sauvegarder avant de quitter ? (oui/non) : " RESET);
            scanf("%s", rep);
            if (strcmp(rep, "oui") == 0) {
                gestionSauvegarde(joueur1, joueur2, tourNumero, difficulte, tempsTotalActuel);
            }
            break; 
        }
        if (stop == 2) { // Save
            gestionSauvegarde(joueur1, joueur2, tourNumero, difficulte, tempsTotalActuel);
            detruireJoueur(joueur1); detruireJoueur(joueur2);
            MenuPrincipal(); return; 
        }
        
        if (verifierVictoire(joueur2)) { gagnant = joueur1; break; }

        // --- TOUR J2 (ou Ordi) ---
        if (strcmp(difficulte, "1vs1") == 0) {
            // Mode J2 Humain
            printf(EFFACE);
            printf("\n>>> Passez le terminal à %s <<<", joueur2->NomJoueur);
            printf("\nAppuyez sur Entrée...");
            getchar();

            printf(EFFACE);
            AfficherHUD(tourNumero, tempsTotalActuel, joueur2->NomJoueur);
            printf("\n>>> C'est au tour de %s <<<\n", joueur2->NomJoueur);
            
            stop = tourJoueur1vs1(joueur2, joueur1); 
            
            if (stop == 1) break;
            if (stop == 2) {
                gestionSauvegarde(joueur1, joueur2, tourNumero, "1vs1", tempsTotalActuel);
                detruireJoueur(joueur1); detruireJoueur(joueur2);
                MenuPrincipal(); return;
            }
            
            printf(EFFACE); printf("\n>>> Passez le terminal à %s <<<", joueur1->NomJoueur);
            printf("\nAppuyez sur Entrée..."); getchar();

        } else {
            // Mode Ordi
            printf("\n--- Tour de l'Ordinateur ---\n");
            #ifdef _WIN32
                SLEEP(1000); 
            #else
                SLEEP(1); 
            #endif

            if (strcmp(difficulte, "facile") == 0) OrdiFacile(joueur2, joueur1);
            else if (strcmp(difficulte, "moyen") == 0) OrdiMoyen(joueur2, joueur1);
            else OrdiDifficile(joueur2, joueur1);
            
            printf("\nAppuyez sur Entrée pour continuer..."); getchar();
        }
        
        if (verifierVictoire(joueur1)) { gagnant = joueur2; break; }
        
        tourNumero++;
    }
    
    if (gagnant != NULL) {
        MenuFinDePartie(gagnant);
    } 
    detruireJoueur(joueur1); detruireJoueur(joueur2); MenuPrincipal();
}


/* ============================================================================
 * 5. INTELLIGENCE ARTIFICIELLE
 * ============================================================================ */

/* IA Facile : Tir totalement aléatoire */
void OrdiFacile(Joueur* ordi, Joueur* humain) 
{
    int ligne, colonne;
    int resultat;

    // Recherche d'une case libre
    do {
        ligne = rand() % 10;
        colonne = rand() % 10;
    } while(ordi->mesTirs->tab[ligne][colonne] != 0); // Tant qu'on a déjà tiré ici

    printf("L'Ordinateur tire en %c%d !\n", 'A' + colonne, ligne + 1);
    resultat = tirer(humain->mesNavires, ligne, colonne);

    // Feedback
    if (resultat == 1) {
        printf("💥 L'Ordinateur a TOUCHÉ un bateau !\n");
        ordi->mesTirs->tab[ligne][colonne] = 3;

        int index = trouverBateau(humain->flotte, ligne, colonne);
        if (index != -1) { 
            enregistrerTouche(&humain->flotte[index]);
            if (estCoule(&humain->flotte[index])) {
                printf("☠️ L'Ordinateur a COULÉ votre %s !\n", humain->flotte[index].nom);
            }
        }
    } else {
        printf("💦 PLOUF ! L'Ordinateur a raté.\n");
        ordi->mesTirs->tab[ligne][colonne] = 2;
    }
}

/* IA Moyenne : Chasse autour des cibles touchées */
void OrdiMoyen(Joueur* ordi, Joueur* humain) {
    int ligne = -1, colonne = -1;
    int resultat;
    int trouveCibleIntelligente = 0;

    // 1. SCAN : On cherche une case "Touchée" (3) qui a des voisins inconnus (0)
    int adjL[] = {-1, 1, 0, 0}; 
    int adjC[] = {0, 0, -1, 1};

    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            if (ordi->mesTirs->tab[i][j] == 3) {
                // On vérifie ses 4 voisins
                for (int k = 0; k < 4; k++) {
                    int testL = i + adjL[k];
                    int testC = j + adjC[k];

                    // Si voisin valide et pas encore tiré
                    if (testL >= 0 && testL < 10 && testC >= 0 && testC < 10) {
                        if (ordi->mesTirs->tab[testL][testC] == 0) {
                            ligne = testL;
                            colonne = testC;
                            trouveCibleIntelligente = 1;
                            goto fin_recherche; // Sortie rapide
                        }
                    }
                }
            }
        }
    }

    fin_recherche:; 

    // 2. RANDOM : Si rien trouvé, tir aléatoire
    if (trouveCibleIntelligente == 0) {
        do {
            ligne = rand() % 10;
            colonne = rand() % 10;
        } while(ordi->mesTirs->tab[ligne][colonne] != 0);
    }

    // 3. TIR
    printf("L'Ordinateur tire en %c%d !\n", 'A' + colonne, ligne + 1);
    resultat = tirer(humain->mesNavires, ligne, colonne);

    if (resultat == 1) {
        printf("💥 L'Ordinateur a TOUCHÉ un bateau !\n");
        ordi->mesTirs->tab[ligne][colonne] = 3;

        int index = trouverBateau(humain->flotte, ligne, colonne);
        if (index != -1) {
            enregistrerTouche(&humain->flotte[index]);
            if (estCoule(&humain->flotte[index])) {
                printf("☠️ L'Ordinateur a COULÉ votre %s !\n", humain->flotte[index].nom);
            }
        }
    } else {
        printf("💦 PLOUF ! L'Ordinateur a raté.\n");
        ordi->mesTirs->tab[ligne][colonne] = 2;
    }
}

/* IA Difficile : Triche (connaît la position des bateaux) */
void OrdiDifficile(Joueur* ordi, Joueur* humain) {
    int ligne = -1, colonne = -1;
    int resultat;
    
    // 1. SCAN TRICHE : On regarde directement la grille de l'adversaire
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            // Si c'est un bateau (1) et qu'on a pas encore tiré (0)
            if (humain->mesNavires->tab[i][j] == 1 && ordi->mesTirs->tab[i][j] == 0) {
                ligne = i;
                colonne = j;
                goto cible_trouvee;
            }
        }
    }

    cible_trouvee:;

    // Sécurité (normalement inutile en difficile)
    if (ligne == -1) {
        do {
            ligne = rand() % 10;
            colonne = rand() % 10;
        } while(ordi->mesTirs->tab[ligne][colonne] != 0);
    }

    // 2. TIR
    printf("L'Ordinateur tire en %c%d !\n", 'A' + colonne, ligne + 1);
    resultat = tirer(humain->mesNavires, ligne, colonne);

    if (resultat == 1) {
        printf("💥 L'Ordinateur a TOUCHÉ un bateau !\n");
        ordi->mesTirs->tab[ligne][colonne] = 3;

        int index = trouverBateau(humain->flotte, ligne, colonne);
        if (index != -1) {
            enregistrerTouche(&humain->flotte[index]);
            if (estCoule(&humain->flotte[index])) {
                printf("☠️ L'Ordinateur a COULÉ votre %s !\n", humain->flotte[index].nom);
            }
        }
    } else {
        printf("💦 PLOUF ! L'Ordinateur a raté.\n");
        ordi->mesTirs->tab[ligne][colonne] = 2;
    }
}


/* ============================================================================
 * 6. UI DE SAUVEGARDE
 * ============================================================================ */

void gestionSauvegarde(Joueur* j1, Joueur* j2, int tour, char* difficulte, long tempsTotal) {
    int slot = 0;
    char filename[30];
    char nomPerso[50];
    
    printf(EFFACE);
    printf("\n=== SAUVEGARDE ===\n");
    printf("Donnez un nom à cette sauvegarde (ex: Partie_Enzo) : ");
    scanf("%s", nomPerso);

    printf("Choisissez un emplacement (1 à %d) : ", NB_SLOTS);
    if (scanf("%d", &slot) != 1) {
        int c;
        while ((c = getchar()) != '\n' && c != EOF) {} // Vide le buffer
        slot = 0;
    }
    
    if (slot >= 1 && slot <= NB_SLOTS) {
        sprintf(filename, "save_%d.bin", slot);
        
        // Appel au module de sauvegarde
        sauvegarderPartie(filename, j1, j2, tour, difficulte, nomPerso, tempsTotal);
        
        printf("\n✅ Sauvegardé sous '%s' (Temps: %ld min %ld sec)\n", nomPerso, tempsTotal/60, tempsTotal%60);
        printf("Appuyez sur Entrée...");
        getchar(); getchar(); 
    } else {
        printf("❌ Annulé.\n"); getchar(); getchar();
    }
}
