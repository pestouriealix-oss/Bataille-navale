#include "librairies.h"
#include "jeu.h"
#include "grille.h"
#include "bateau.h"
#include "affichage.h"


/* ============================================================================
 * GESTION MÉMOIRE DES JOUEURS
 * ============================================================================ */

/* -------------------------------------------------------------------------
 * Création d'un joueur
 * Alloue la structure principale, puis les sous-structures (Grilles, Flotte).
 * ------------------------------------------------------------------------- */
Joueur* creerJoueur(void){
    // 1. Allocation de la structure Joueur
    Joueur* j = malloc(sizeof(Joueur));
    
    // 2. Initialisation des grilles
    // mesNavires : La grille où je place mes bateaux (Défense)
    // mesTirs    : La grille où je marque mes tirs (Attaque/Brouillard)
    j->mesNavires = initialiser();  // Grille de mes bateaux
    j->mesTirs = initialiser();     // Grille de mes tirs
    
    // 3. Initialisation de la flotte (Tableau de 5 bateaux)
    j->flotte = creerFlotte();

    return j;
}

/* -------------------------------------------------------------------------
 * Destruction d'un joueur
 * Libère la mémoire dans l'ordre inverse de la création (Enfants puis Parent).
 * ------------------------------------------------------------------------- */
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
 * PHASE DE PRÉPARATION
 * ============================================================================ */

/* -------------------------------------------------------------------------
 * Phase de placement des bateaux
 * Boucle sur les 5 bateaux pour demander leur placement au joueur.
 * Gère l'annulation si le joueur tape -1.
 * ------------------------------------------------------------------------- */
int phasePreparation(Joueur* j) 
{
    char reponse[15];
    while((strcmp(reponse, "aleatoirement") != 0) && (strcmp(reponse, "non") != 0)){
        printf(EFFACE); // Nettoie le terminal
    printf("\n");
    printf("========================================\n");
    printf(CIEL "       %s, placez vos bateaux !\n" RESET, j->NomJoueur);
    printf("========================================\n");
    printf("Pour placer les bateaux aléatoirement, tapez :" CYAN "aleatoirement" RESET);
    printf("Sinon, tapez " CYAN "non" RESET);
    printf("Votre reponse : ");
    scanf(" %s", reponse);
    }
    if(strcmp(reponse, "aleatoirement") == 0){
        placerBateauxAleatoire(j);
    }
    else{
        printf(EFFACE); // Nettoie le terminal
    printf("\n");
    printf("========================================\n");
    printf(CIEL "       %s, placez vos bateaux !\n" RESET, j->NomJoueur);
    printf("========================================\n");

    
    // Boucle sur chaque bateau de la flotte
    for (int i = 0; i < NB_BATEAUX; i++) {
        printf("\nBateau %d/%d\n", i + 1, NB_BATEAUX);
        afficherCoordonnees(j->mesNavires);
        
        // Appel de la fonction interactive de placement (dans bateau.c)
        int resultat = positionnerBateau(&j->flotte[i], j->mesNavires);
        
        // GESTION DU QUIT : Si positionnerBateau renvoie 0 (utilisateur a fait -1)
        if (resultat == 0) {
            printf("🚫 Placement annulé. Retour au menu...\n");
            return 0; // Signal d'arrêt immédiat
        }
    }
    
    printf("\n✅ Tous vos bateaux sont placés !\n");
    afficherCoordonnees(j->mesNavires);
    
    printf("\nAppuyez sur Entrée pour continuer...");
    getchar();
    
    }

    return 1; // Succès : Tout est placé
}

/* -------------------------------------------------------------------------
 * Affichage du plateau de jeu complet
 * Montre la vue stratégique du joueur : ses navires et ses tirs passés.
 * ------------------------------------------------------------------------- */
void afficherPlateau(Joueur* j)
{
    printf("\n=== MES NAVIRES ===\n");
    afficherCoordonnees(j->mesNavires);
    
    printf("\n=== MES TIRS ===\n");
    afficherCoordonnees(j->mesTirs);
}



/* ============================================================================
 * LOGIQUE DU TOUR DE JEU
 * ============================================================================ */

/* -------------------------------------------------------------------------
 * Tour d'un joueur humain
 * Gère la saisie, le tir, et le retour d'information (Touché/Coulé).
 * Retourne 1 si le joueur veut quitter, 0 sinon.
 * ------------------------------------------------------------------------- */
int tourJoueur(Joueur* attaquant, Joueur* defenseur)
{
    int ligne, colonne;
    char conversion;
    int resultat;
    
    printf("\n");
    printf("========================================\n");
    printf(BLEUGRIS "              Tour de %s\n" RESET, attaquant->NomJoueur);
    printf("========================================\n");
    
    afficherPlateau(attaquant);
    
    // Boucle de saisie des coordonnées (jusqu'à avoir un tir valide ou un quit)
    while (1) {
        // --- 2. Saisie de la colonne ---
        char buffer[10]; // On crée une boite pour stocker du texte
        
        printf("Entrez la colonne (A-J) : ");
        scanf("%s", buffer); // On lit tout le texte (ex: "A", "b" ou "-1")

        // GESTION DU QUIT : On compare le texte avec "-1"
        // (strcmp renvoie 0 si les deux textes sont identiques)
        if (strcmp(buffer, "-1") == 0) {
            // Pas besoin de vider le buffer ici car scanf("%s") a tout mangé
            return 1; 
        }

        // Si ce n'est pas "-1", on prend juste la première lettre
        conversion = buffer[0];
        
        printf("\nEntrez la ligne (ou -1 pour quitter) : ");
        scanf("%d", &ligne);

        // --- GESTION DU QUIT ---
        if (ligne == -1) {
            printf("🏳️  Vous abandonnez la partie...\n");
            return 1; // On renvoie 1 pour dire "STOP"
        }

        // Convertir de 1-10 à 0-9
        ligne = ligne - 1;
        colonne = conversion - 65;


        
        // Tentative de tir sur la grille adverse
        resultat = tirer(defenseur->mesNavires, ligne, colonne);
        
        // Gestion des erreurs de tir
        if (resultat == -2) {
            printf("❌ Hors de la grille ! Réessayez.\n");
            continue;
        }
        
        if (resultat == -1) {
            printf("❌ Vous avez déjà tiré ici ! Réessayez.\n");
            continue;
        }
        
        break; // Tir valide, on sort de la boucle de saisie
    }
    
    // MISE A JOUR DE LA CARTE DES TIRS DE L'ATTAQUANT
    // L'attaquant note le résultat sur sa propre carte pour s'en souvenir
    attaquant->mesTirs->tab[ligne][colonne] = defenseur->mesNavires->tab[ligne][colonne];
    
    if (resultat == 0) {
        printf("\n💦 PLOUF ! Dans l'eau...\n");
    } else if (resultat == 1) {
        printf("\n💥 TOUCHÉ !\n");

        // Important : On doit trouver QUEL bateau a été touché pour gérer ses PV
        int index = trouverBateau(defenseur->flotte, ligne, colonne);
        if (index != -1) {
            enregistrerTouche(&defenseur->flotte[index]);

            // Si les PV tombent à 0 (touches == taille)
            if (estCoule(&defenseur->flotte[index])) {
                printf("🚢 COULÉ ! Vous avez coulé le %s !\n", defenseur->flotte[index].nom);
            }
        }
    }
    
    printf("\nAppuyez sur Entrée pour continuer...");
    getchar();
    getchar(); // Double getchar souvent nécessaire après scanf pour vider le buffer

    return 0; // Le tour s'est bien passé (0 = Continue)
}

/* -------------------------------------------------------------------------
 * Vérification de victoire
 * Délègue la vérification à la fonction de bateau.c
 * ------------------------------------------------------------------------- */
int verifierVictoire(Joueur* adversaire)
{
    return tousCoules(adversaire->flotte);
}


/* ============================================================================
 * BOUCLES PRINCIPALES DE JEU
 * ============================================================================ */

/* -------------------------------------------------------------------------
 * Mode : Joueur contre Joueur (1v1)
 * Gère l'alternance des tours et le passage de terminal.
 * ------------------------------------------------------------------------- */
void jouerPartie(Joueur * joueur1, Joueur * joueur2)
{
    printf(EFFACE);
    printf("\n========================================\n");
    printf(CIEL "           LA BATAILLE NAVALE" RESET);
    printf(BLEUGRIS "\n       made by Julien, Alix, Enzo" RESET);
    printf("\n========================================\n");
    
    
    // 1. PRÉPARATION JOUEUR 1
    if (phasePreparation(joueur1) == 0) {
        // Si J1 quitte, on détruit tout et on retourne au menu
        detruireJoueur(joueur1);
        detruireJoueur(joueur2);
        MenuPrincipal();
        return;
    }
    
    printf("\n\n>>> Passez le terminal à %s <<<\n", joueur2 -> NomJoueur);
    printf("Appuyez sur Entrée ...");
    getchar();
    
    // 2. PRÉPARATION JOUEUR 2
    if (phasePreparation(joueur2) == 0) {
        // Si J2 quitte
        detruireJoueur(joueur1);
        detruireJoueur(joueur2);
        MenuPrincipal();
        return;
    }
    
    // 3. BOUCLE DE JEU (Combat)
    int tourNumero = 1;
    Joueur * gagnant = NULL;
    int stop = 0; // Variable pour savoir si on doit arrêter (0=Joue, 1=Quitte)

    while (gagnant == NULL) {

        printf(EFFACE);
        printf("\n\n");
        printf("========================================\n");
        printf(BLEUGRIS "           TOUR %d\n" RESET, tourNumero);
        printf("========================================\n");

        
        // --- TOUR JOUEUR 1 ---
        stop = tourJoueur(joueur1, joueur2); // On récupère le résultat
        if (stop == 1) break; // Si J1 veut quitter, on casse la boucle
        
        if (verifierVictoire(joueur2)) {
            gagnant = joueur1;
            break;
        }
        
        // Passage de main 
        printf(EFFACE);
        printf("\n>>> Passez le terminal à %s <<<", joueur2 -> NomJoueur);
        printf("\nAppuyez sur Entrée...");
        getchar();

        
        // --- TOUR JOUEUR 2 ---
        stop = tourJoueur(joueur2, joueur1); // On récupère le résultat
        if (stop == 1) break; // Si J2 veut quitter
        
        if (verifierVictoire(joueur1)) {
            gagnant = joueur2;
            break;
        }
        
        printf("\n>>> Passez le terminal à %s <<<", joueur1 -> NomJoueur);
        printf("\nAppuyez sur Entrée...");
        getchar();
        
        tourNumero++;
    }
    
    // 4. FIN DE PARTIE
    detruireJoueur(joueur1);
    detruireJoueur(joueur2);

    // On affiche le menu de fin SEULEMENT s'il y a un vrai gagnant (pas un abandon)
    if (gagnant != NULL) {
        // Partie finie normalement par une victoire
        AjouterVictoire1c1(gagnant); 
        MenuFinDePartie(gagnant);
    } else {
        // Partie arrêtée par un joueur (Abandon)
        MenuPrincipal(); 
    }
}

/* ============================================================================
 * ORDINATEUR
 * ============================================================================ */

/* -------------------------------------------------------------------------
 * OrdiFacile : Tir Aléatoire
 * L'ordinateur tire au hasard tant qu'il ne trouve pas une case libre.
 * ------------------------------------------------------------------------- */
void OrdiFacile(Joueur* ordi, Joueur* humain) 
{
    // Variables pour stocker les coordonnées ciblées (0 à 9)
    int ligne, colonne;
    
    // Variable pour stocker le résultat du tir
    int resultat;

    printf("🤖 L'Ordi (Facile) réfléchit...\n");

    // --- 1. CHOIX DES COORDONNÉES ---
    do {
        // Générer des nombres aléatoires
        ligne = rand() % 10;
        colonne = rand() % 10;

        // VERIFICATION DANS LA MÉMOIRE DE L'ORDI :
        // On regarde dans "mesTirs" de l'ordi.
        // Tant que la case n'est pas 0, c'est qu'on a déjà joué là.
    } while(ordi->mesTirs->tab[ligne][colonne] != 0);


    // --- 2. ACTION (Le tir) ---
    printf("L'Ordi tire en %c%d !\n", 'A' + colonne, ligne + 1);

    // On tire sur la grille de l'HUMAIN
    resultat = tirer(humain->mesNavires, ligne, colonne);


    // --- 3. TRAITEMENT DU RÉSULTAT ---
    
    if (resultat == 1) {
        printf("💥 L'Ordi a TOUCHÉ un bateau !\n");
        
        // Mise à jour de la grille de tirs de l'ordi
        ordi->mesTirs->tab[ligne][colonne] = 3;

        // Gestion des dégâts sur le bateau de l'humain
        int index = trouverBateau(humain->flotte, ligne, colonne);
        
        if (index != -1) { 
            enregistrerTouche(&humain->flotte[index]);
            
            if (estCoule(&humain->flotte[index])) {
                printf("☠️ L'Ordi a COULÉ votre %s !\n", humain->flotte[index].nom);
            }
        }

    } else {
        printf("💦 PLOUF ! L'Ordi a raté.\n");
        
        // Mise à jour de la grille de tirs de l'ordi
        ordi->mesTirs->tab[ligne][colonne] = 2;
    }
}


/* -------------------------------------------------------------------------
 * OrdiMoyen : Mode Chasseur (Stratégie)
 * 1. Scanne sa mémoire pour trouver un bateau touché (3).
 * 2. Si trouvé, tire sur les cases adjacentes (Haut/Bas/Gauche/Droite).
 * 3. Sinon, tire au hasard.
 * ------------------------------------------------------------------------- */
void OrdiMoyen(Joueur* ordi, Joueur* humain) {
    int ligne = -1, colonne = -1;
    int resultat;
    int trouveCibleIntelligente = 0;

    printf("🤔 L'Ordi (Moyen) calcule sa stratégie...\n");

    // --- 1. RECHERCHE INTELLIGENTE (Mode "Chasse") ---
    
    // Tableaux pour regarder autour : Haut, Bas, Gauche, Droite
    int adjL[] = {-1, 1, 0, 0}; 
    int adjC[] = {0, 0, -1, 1};

    // On parcourt toute la grille mémoire de l'ordi
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            
            // Si on trouve une case "Touché" (3)
            if (ordi->mesTirs->tab[i][j] == 3) {
                
                // On vérifie ses 4 voisins
                for (int k = 0; k < 4; k++) {
                    int testL = i + adjL[k];
                    int testC = j + adjC[k];

                    // Est-ce que le voisin est DANS la grille ?
                    if (testL >= 0 && testL < 10 && testC >= 0 && testC < 10) {
                        
                        // Est-ce que le voisin est INCONNU (0) ?
                        if (ordi->mesTirs->tab[testL][testC] == 0) {
                            // On a trouvé une cible logique !
                            ligne = testL;
                            colonne = testC;
                            trouveCibleIntelligente = 1;
                            
                            // On sort des boucles (goto est mal vu, on utilise des breaks)
                            goto fin_recherche; 
                        }
                    }
                }
            }
        }
    }

    fin_recherche:; // Etiquette pour sortir des boucles imbriquées


    // --- 2. FALLBACK (Mode Aléatoire si rien trouvé) ---
    if (trouveCibleIntelligente == 0) {
        // C'est exactement comme le niveau Facile
        do {
            ligne = rand() % 10;
            colonne = rand() % 10;
        } while(ordi->mesTirs->tab[ligne][colonne] != 0);
    }


    // --- 3. ACTION (Identique au niveau Facile) ---
    // (Idéalement, on pourrait créer une fonction commune "ExecuterTir", 
    // mais on copie-colle pour l'instant pour garder le code simple)

    printf("L'Ordi tire en %c%d !\n", 'A' + colonne, ligne + 1);
    resultat = tirer(humain->mesNavires, ligne, colonne);

    if (resultat == 1) {
        printf("💥 L'Ordi a TOUCHÉ un bateau !\n");
        ordi->mesTirs->tab[ligne][colonne] = 3;

        int index = trouverBateau(humain->flotte, ligne, colonne);
        if (index != -1) {
            enregistrerTouche(&humain->flotte[index]);
            if (estCoule(&humain->flotte[index])) {
                printf("☠️ L'Ordi a COULÉ votre %s !\n", humain->flotte[index].nom);
            }
        }
    } else {
        printf("💦 PLOUF ! L'Ordi a raté.\n");
        ordi->mesTirs->tab[ligne][colonne] = 2;
    }
}

/* -------------------------------------------------------------------------
 * OrdiDifficile : Mode Tricheur
 * L'ordinateur regarde directement la grille du joueur (mesNavires) 
 * au lieu de sa mémoire (mesTirs). Il ne rate jamais.
 * ------------------------------------------------------------------------- */
void OrdiDifficile(Joueur* ordi, Joueur* humain) {
    int ligne = -1, colonne = -1;
    int resultat;
    
    printf("😈 L'Ordi (Hardcore) utilise ses satellites espions...\n");

    // --- 1. TRICHE (Scan de la grille adverse) ---
    // On parcourt toute la grille de l'HUMAIN
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            
            // Si la case contient un bateau (1)
            // ET que l'ordi n'a pas encore tiré dessus (0)
            if (humain->mesNavires->tab[i][j] == 1 && ordi->mesTirs->tab[i][j] == 0) {
                ligne = i;
                colonne = j;
                // On a trouvé un bateau, on arrête de chercher
                goto cible_trouvee;
            }
        }
    }

    cible_trouvee:;

    // Sécurité : Si jamais il ne trouve rien, il tire au hasard
    if (ligne == -1) {
        do {
            ligne = rand() % 10;
            colonne = rand() % 10;
        } while(ordi->mesTirs->tab[ligne][colonne] != 0);
    }

    // --- 2. ACTION (On tire) ---
    printf("L'Ordi tire en %c%d !\n", 'A' + colonne, ligne + 1);
    resultat = tirer(humain->mesNavires, ligne, colonne);

    // --- 3. TRAITEMENT ---
    if (resultat == 1) {
        printf("💥 TOUCHÉ ! (Evidemment...)\n");
        ordi->mesTirs->tab[ligne][colonne] = 3;

        int index = trouverBateau(humain->flotte, ligne, colonne);
        if (index != -1) {
            enregistrerTouche(&humain->flotte[index]);
            if (estCoule(&humain->flotte[index])) {
                printf("☠️ L'Ordi a COULÉ votre %s !\n", humain->flotte[index].nom);
            }
        }
    } else {
        // Ce cas ne devrait JAMAIS arriver en mode Difficile, sauf bug
        printf("💦 PLOUF ! (Impossible en difficile)\n");
        ordi->mesTirs->tab[ligne][colonne] = 2;
    }
}

/* -------------------------------------------------------------------------
 * Mode : Joueur contre Ordinateur
 * Gère la boucle de jeu spécifique à l'Ordi avec choix de difficulté.
 * ------------------------------------------------------------------------- */
void jouerPartieOrdi(Joueur * humain, char * difficulte) {
    Joueur * ordi = creerJoueur();
    strcpy(ordi->NomJoueur, "Ordinateur");

    // 1. Phase de préparation Humain
    // On vérifie si le joueur veut quitter
    if (phasePreparation(humain) == 0) {
        detruireJoueur(humain);
        detruireJoueur(ordi);
        MenuPrincipal(); // Retour direct au menu
        return;
    }

    // 2. Préparation Ordi (PLACEMENT ALEATOIRE)
    printf("\n🤖 L'ordinateur place ses bateaux...\n");
    

    placerBateauxAleatoire(ordi);


    printf("✅ L'ordinateur est prêt au combat !\n");

    // 3. Boucle de Jeu
    int tourNumero = 1;
    Joueur * gagnant = NULL;
    int stop = 0; // Variable de contrôle

    while (gagnant == NULL) {
        printf(EFFACE);
        printf("\n=== TOUR %d ===\n", tourNumero);

        // --- Tour de l'Humain ---
        stop = tourJoueur(humain, ordi); // On capture le retour
        if (stop == 1) break; // ON QUITTE SI -1
        
        if (verifierVictoire(ordi)) {
            gagnant = humain;
            break;
        }

        // --- Tour de l'Ordi ---
        printf("\n--- Tour de l'Ordinateur ---\n");
        // Sélection de l'Ordi selon la difficulté
        if (strcmp(difficulte, "facile") == 0) OrdiFacile(ordi, humain);
        //else if (strcmp(difficulte, "moyen") == 0) OrdiMoyen(ordi, humain);
        else OrdiDifficile(ordi, humain);
        
        printf("\nAppuyez sur Entrée pour continuer...");
        getchar();

        if (verifierVictoire(humain)) {
            gagnant = ordi;
            break;
        }

        tourNumero++;
    }

    // 4. Fin de partie
    detruireJoueur(humain);
    detruireJoueur(ordi);
    
    if (gagnant != NULL) {
        MenuFinDePartie(gagnant);
    } else {
        MenuPrincipal();
    }
}
