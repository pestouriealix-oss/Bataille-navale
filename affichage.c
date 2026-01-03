#include "librairies.h"
#include "affichage.h"
#include "sauvegarde.h"
#include "jeu.h"

/* * GESTION DE LA PAUSE (Multi-plateforme)
 * Permet de faire des pauses en millisecondes sur Windows et Mac/Linux
 */
#ifdef _WIN32
    #include <windows.h>
    #define PAUSE_MS(x) Sleep(x)
#else
    #include <unistd.h>
    #define PAUSE_MS(x) usleep((x)*1000)
#endif


// =================================================================
//                    1. OUTILS VISUELS
// =================================================================

/* Affiche du texte lettre par lettre pour un effet "rétro" */
void afficherLentement(char* texte) {
    for (int i = 0; texte[i] != '\0'; i++) {
        printf("%c", texte[i]);
        fflush(stdout); // Force l'écriture immédiate dans le terminal
        PAUSE_MS(25);   // Petite pause pour l'effet de frappe
    }
    printf("\n");
}


// =================================================================
//                    2. MENUS PRINCIPAUX
// =================================================================

/* Affiche le grand menu de démarrage avec le logo ASCII */
void MenuPrincipal(void){
    int choix = -1;
    
    while(1) {
        printf(EFFACE);
        
        // --- LOGO DU JEU ---
        printf(CIEL "                                     |__\n");
        printf("                                     |\\/\n");
        printf("                                     ---\n");
        printf("                                     / | [\n");
        printf("                              !      | |||\n");
        printf("                            _/|     _/|-++'\n");
        printf("                        +  +--|    |--|--|_ |-\n");
        printf("                     { /|__|  |/\\__|  |--- |||__/\n");
        printf("                    +---------------___[}-_===_.'____                 /\\\n");
        printf("                ____`-' ||___-{]_| _[}-  |     |_[___\\==--            \\/   _\n");
        printf(" __..._____--==/___]_|__|_____________________________[___\\==--____,------' .7\n");
        printf("|                        BATAILLE NAVALE                        BB-61/ \n");
        printf(" \\_________________________________________________________________________|\n" RESET);
        
        printf("\n");
        printf("        " VERT "╔══════════════════════════════════════════╗" RESET "\n");
        printf("        " VERT "║            MENU PRINCIPAL                ║" RESET "\n");
        printf("        " VERT "╚══════════════════════════════════════════╝" RESET "\n");
        
        // --- OPTIONS ---
        printf("\n        " BLANC "[ 1 ]" RESET "  >   JOUER UNE PARTIE");
        printf("\n        " BLANC "[ 2 ]" RESET "  +   CHARGER UNE SAUVEGARDE");
        printf("\n        " BLANC "[ 3 ]" RESET "  ?   REGLES DU JEU");
        printf("\n        " BLANC "[ 4 ]" RESET "  *   CLASSEMENT");
        printf("\n        " BLANC "[ 0 ]" RESET "  X   QUITTER");
        
        printf("\n\n" JAUNE "👉 Votre ordre, Capitaine ? : " RESET);

        // --- SAISIE SÉCURISÉE ---
        // Si l'utilisateur tape une lettre, on vide le tampon pour éviter le plantage
        if (scanf("%d", &choix) != 1) { 
            int c; 
            while ((c = getchar()) != '\n' && c != EOF) {} 
            choix = -1; 
        }
        
        // --- NAVIGATION ---
        switch(choix) {
            case 1: ModeDeJeu(); break;
            case 2: 
                // La logique de chargement sera appelée ici
                break;
            case 3: ReglesDuJeu(); break;
            case 4: Classement(); break;
            case 0: printf("\nRompez les rangs !\n"); exit(0);
            default: break;
        }
    }
}

/* Choix entre 1vs1 ou Contre l'Ordinateur */
void ModeDeJeu(void){
    int choix = -1;
    while(1) {
        printf(EFFACE);
        printf("\n=== MODE DE JEU ===\n");
        printf("  1. 1 vs 1\n");
        printf("  2. Contre Ordi\n");
        printf("  0. Retour\n");
        printf("\n👉 Votre choix : ");
        
        if (scanf("%d", &choix) != 1) { int c; while ((c = getchar()) != '\n' && c != EOF) {} choix = -1; }
        
        if (choix == 1) { NomDesJoueurs(); return; }   // Vers config 1vs1
        if (choix == 2) { ChoixDifficulte(); return; } // Vers config IA
        if (choix == 0) return;
    }
}

/* Choix de la difficulté pour l'IA */
void ChoixDifficulte(void){
    int choix = -1;
    while(1){
        printf(EFFACE);
        printf("\n=== DIFFICULTÉ ===\n");
        printf("  1. Facile\n");
        printf("  2. Moyen\n");
        printf("  3. Difficile\n");
        printf("  0. Retour\n");
        printf("\n👉 Votre choix : ");
        
        if (scanf("%d", &choix) != 1) { int c; while ((c = getchar()) != '\n' && c != EOF) {} choix = -1; }
        
        if (choix == 1) { NomDuJoueur("facile"); return; }
        if (choix == 2) { NomDuJoueur("moyen"); return; }
        if (choix == 3) { NomDuJoueur("difficile"); return; }
        if (choix == 0) return;
    }
}

/* Affichage simple du texte des règles */
void ReglesDuJeu(void){
    printf(EFFACE);
    printf(JAUNE "\n=== REGLES DU JEU ===\n" RESET);
    printf("1. Placez vos 5 bateaux sur la grille (sans chevauchement).\n");
    printf("2. Tirez chacun votre tour sur la grille adverse.\n");
    printf("3. 'Touché' = bateau touché, 'Coulé' = bateau détruit.\n");
    printf("4. Le premier qui coule toute la flotte adverse GAGNE !\n");
    printf("\nAppuyez sur Entrée pour revenir...");
    getchar(); getchar();
}

/* Ecran de victoire animé à la fin de la partie */
void MenuFinDePartie(Joueur *j){
    int choix = -1;
    while(1){
        printf(EFFACE);
        printf("\n"); 
        
        // Animation du texte de victoire
        printf(VERT); 
        afficherLentement("**********************************");
        afficherLentement("      VICTOIRE CONFIRMEE !        ");
        afficherLentement("**********************************");
        printf(RESET); 
        
        printf("\nFelicitations %s.\n", j->NomJoueur);
        printf("\n");
        
        printf("  [1] Rejouer une bataille\n");
        printf("  [0] Retour au Quartier General\n");
        
        printf(JAUNE "\n> Ordre : " RESET);
        
        if (scanf("%d", &choix) != 1) { 
            int c; while ((c = getchar()) != '\n' && c != EOF) {} 
            choix = -1; 
        }
        
        if (choix == 1) { ModeDeJeu(); return; }
        if (choix == 0) { MenuPrincipal(); return; }
    }
}


// =================================================================
//                    3. INITIALISATION PARTIE
// =================================================================

/* Configuration pour le mode 1 vs 1 */
void NomDesJoueurs(void){
    Joueur * j1 = creerJoueur();
    Joueur * j2 = creerJoueur();
    
    printf(EFFACE);
    printf("Nom Joueur 1 : "); scanf("%s", j1->NomJoueur);
    printf("Nom Joueur 2 : "); scanf("%s", j2->NomJoueur);
    
    // On initialise le fichier de score s'il n'existe pas
    AjouterVictoire1vs1(j1); 
    
    // On lance le jeu (Tour 1, 0 secondes de jeu)
    jouerPartie(j1, j2, 1, 0); 
}

/* Configuration pour le mode Contre Ordi */
void NomDuJoueur(char difficulte[15]){
    Joueur * j = creerJoueur();
    printf(EFFACE);
    printf("Votre Nom : "); scanf("%s", j->NomJoueur);
    
    // On lance le jeu contre l'IA
    jouerPartieOrdi(j, NULL, difficulte, 1, 0);
}


// =================================================================
//                    4. INTERFACE EN JEU (HUD)
// =================================================================

/* Affiche la barre d'info en haut (Tour, Chrono, Joueur) */
void AfficherHUD(int tour, long temps, char* nomJoueur) {
    long min = temps / 60;
    long sec = temps % 60;

    // Bordure haute
    printf("          " BG_NOIR BLANC " ╔══════════════════════════════════════════════════════════╗ " RESET "\n");
    
    // Ligne d'information formatée
    // %-3d aligne le nombre à gauche, %02ld ajoute un zéro si < 10 (ex: 09)
    printf("          " BG_NOIR BLANC " ║ " JAUNE "TOUR: %-3d" BLANC " │ " CYAN "CHRONO: %02ld:%02ld" BLANC " │ " VERT "JOUEUR: %-14.14s" BLANC " ║ " RESET "\n",
           tour, min, sec, nomJoueur);
           
    // Bordure basse
    printf("          " BG_NOIR BLANC " ╚══════════════════════════════════════════════════════════╝ " RESET "\n");
}


// =================================================================
//                    5. CLASSEMENT (Scores)
// =================================================================

/* Fonction générique pour lire un fichier, trier et afficher le tableau */
void AfficherClassement(FILE * f){
    if (!f) return;
    Joueur tab[100]; 
    int n = 0;
    
    // 1. Lecture des données dans le fichier
    rewind(f);
    while(fscanf(f, "%s %d", tab[n].NomJoueur, &tab[n].NbVictoire) == 2 && n<100) n++;
    
    // 2. Tri à bulles (Du plus grand nombre de victoires au plus petit)
    for(int i=0; i<n-1; i++) {
        for(int j=0; j<n-i-1; j++) {
            if (tab[j].NbVictoire < tab[j+1].NbVictoire) {
                Joueur temp = tab[j]; 
                tab[j] = tab[j+1]; 
                tab[j+1] = temp;
            }
        }
    }
    
    // 3. Affichage du tableau décoré
    printf("\n");
    printf("   " BLANC "╔════╤══════════════════════╤═══════════╗" RESET "\n");
    printf("   " BLANC "║" RESET " Rg " BLANC "│" RESET " NOM DU CAPITAINE     " BLANC "│" RESET " VICTOIRES " BLANC "║" RESET "\n");
    printf("   " BLANC "╠════╪══════════════════════╪═══════════╣" RESET "\n");

    for(int i=0; i<n; i++) {
        char* coul = RESET;
        char medaille = ' ';
        
        // Coloration du podium (Or, Argent, Bronze)
        if (i == 0)      { coul = JAUNE; medaille = '*'; } 
        else if (i == 1) { coul = CYAN;  medaille = '+'; } 
        else if (i == 2) { coul = ROUGE; medaille = '-'; } 
        else             { medaille = ' '; }
        
        // Formatage précis pour aligner les colonnes
        printf("   " BLANC "║" RESET " %s%-2d%c" RESET BLANC "│" RESET " %s%-20.20s " RESET BLANC "│" RESET " %s   %3d    " RESET BLANC "║" RESET "\n", 
               coul, i+1, medaille, coul, tab[i].NomJoueur, coul, tab[i].NbVictoire);
    }
    
    printf("   " BLANC "╚════╧══════════════════════╧═══════════╝" RESET "\n");
    
    if (n > 0) printf("\n   " JAUNE "* Or   " CYAN "+ Argent   " ROUGE "- Bronze" RESET "\n");
}

/* Menu de sélection du classement à voir */
void Classement(void){
    int choix = -1;
    while(1) {
        printf(EFFACE);
        printf("\n=== CLASSEMENT ===\n");
        printf("  1. 1 vs 1\n");
        printf("  2. Contre Ordi\n");
        printf("  0. Retour\n");
        printf("\n👉 Choix : ");
        
        if (scanf("%d", &choix) != 1) { int c; while ((c = getchar()) != '\n' && c != EOF) {} choix = -1; }
        
        if (choix == 1) Classement1vs1();
        else if (choix == 2) ChoixClassementOrdi();
        else if (choix == 0) return;
    }
}

/* Prépare et affiche le classement 1vs1 */
void Classement1vs1(void){
    FILE* f = fopen("registre_1vs1.txt", "r");
    
    printf(EFFACE); 
    printf(CIEL "\n=== CLASSEMENT 1VS1 ===\n" RESET);
    
    if(f) {
        AfficherClassement(f);
        fclose(f);
    } else {
        printf("Aucun classement pour le moment.\n");
    }
    
    printf("\nAppuyez sur Entrée pour revenir..."); 
    getchar(); getchar();
}

/* Choix de la difficulté pour voir le classement Ordi */
void ChoixClassementOrdi(void){
    int choix = 0;
    while(1){
        printf(EFFACE);
        printf("\n=== CLASSEMENT ORDI ===\n");
        printf("  1. Facile\n");
        printf("  2. Moyen\n");
        printf("  3. Difficile\n");
        printf("  0. Retour\n");
        printf("\n👉 Votre choix : ");
        
        if (scanf("%d", &choix) != 1) { int c; while ((c = getchar()) != '\n' && c != EOF) {} choix = -1; }
        
        if(choix == 0) return;
        if(choix == 1) ClassementOrdi("facile");
        if(choix == 2) ClassementOrdi("moyen");
        if(choix == 3) ClassementOrdi("difficile");
    }
}

/* Prépare et affiche le classement Ordi selon la difficulté */
void ClassementOrdi(char *difficulte){
    char nomFichier[30];
    sprintf(nomFichier, "registre_%s.txt", difficulte); // Construit le nom ex: "registre_facile.txt"

    FILE * f = fopen(nomFichier, "r");
    printf(EFFACE);
    printf(CIEL "\n=== CLASSEMENT ORDI (%s) ===\n" RESET, difficulte);
    
    if(f) {
        AfficherClassement(f);
        fclose(f);
    } else {
        printf("Aucun classement pour le moment.\n");
    }
    
    printf("\nAppuyez sur Entrée pour revenir...");
    getchar(); getchar();
}

/* Ajoute une victoire dans le fichier Ordi (via fichier temporaire) */
void AjouterVictoireOrdi(Joueur * j, char *difficulte){
    char nomFichier[30];
    sprintf(nomFichier, "registre_%s.txt", difficulte);
    
    FILE * f = fopen(nomFichier, "r");
    
    // Cas où le fichier n'existe pas encore : on le crée
    if (!f) {
        f = fopen(nomFichier, "w");
        if(f) { fprintf(f, "%s %d\n", j->NomJoueur, 1); fclose(f); }
        return;
    }

    FILE * temp = fopen("temp_ordi.txt", "w");
    if(!temp) { fclose(f); return; }

    char nomLu[50];
    int victoiresLu;
    int trouve = 0;
    
    // On recopie tout en mettant à jour le score du joueur
    while(fscanf(f, "%s %d", nomLu, &victoiresLu) == 2){
        if(strcmp(nomLu, j->NomJoueur) == 0){
            fprintf(temp, "%s %d\n", nomLu, victoiresLu + 1); 
            trouve = 1;
        } else {
            fprintf(temp, "%s %d\n", nomLu, victoiresLu); 
        }
    }
    
    // Si le joueur n'était pas dans la liste, on l'ajoute
    if (!trouve) {
        fprintf(temp, "%s %d\n", j->NomJoueur, 1);
    }
    
    fclose(f);
    fclose(temp);
    remove(nomFichier);                // Supprime l'ancien fichier
    rename("temp_ordi.txt", nomFichier); // Renomme le temporaire
}

/* Ajoute une victoire dans le fichier 1vs1 (via fichier temporaire) */
void AjouterVictoire1vs1(Joueur * j){
    const char* FICHIER = "registre_1vs1.txt";
    FILE * f = fopen(FICHIER, "r");
    
    if (!f) {
        f = fopen(FICHIER, "w");
        if(f) { fprintf(f, "%s %d\n", j->NomJoueur, 1); fclose(f); }
        return;
    }

    FILE * temp = fopen("temp_1vs1.txt", "w");
    if(!temp) { fclose(f); return; }
    
    char nomLu[50];
    int victoiresLu;
    int trouve = 0;
    
    while(fscanf(f, "%s %d", nomLu, &victoiresLu) == 2){
        if(strcmp(nomLu, j->NomJoueur) == 0){
            fprintf(temp, "%s %d\n", nomLu, victoiresLu + 1);
            trouve = 1;
        } else {
            fprintf(temp, "%s %d\n", nomLu, victoiresLu);
        }
    }
    
    if (!trouve) {
        fprintf(temp, "%s %d\n", j->NomJoueur, 1);
    }
    
    fclose(f);
    fclose(temp);
    remove(FICHIER);
    rename("temp_1vs1.txt", FICHIER);
}
