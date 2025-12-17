#include"affichage.h"
#include"jeu.h"

void MenuPrincipal(void){
    char reponse[15];
    while((strcmp(reponse,"regle") != 0)&&(strcmp(reponse,"play") != 0)&&(strcmp(reponse, "classement") != 0)&&(strcmp(reponse, "quit") != 0)){
        printf(EFFACE);
        printf("\n======================================== \n");
        printf(CIEL "           LA BATAILLE NAVALE" RESET);
        printf(BLEUGRIS "\n       made by Julien, Alix, Enzo" RESET);
        printf("\n======================================== \n");
        printf("\nPour démarrer une partie, tapez " CYAN "play \n" RESET);
        printf("Pour afficher les règles, tapez " CYAN "regle \n" RESET);
        printf("Pour accéder au classement, tapez " CYAN "classement \n" RESET);
        printf("\nVotre réponse : ");
        scanf(" %s", reponse);
    }
    if(strcmp(reponse, "regle") == 0){
        ReglesDuJeu();
    }
    if(strcmp(reponse, "play") == 0){
        ModeDeJeu();
    }
    if(strcmp(reponse, "classement") == 0){
        Classement();
    }
    if(strcmp(reponse, "quit") == 0){
        
    }
}

void ReglesDuJeu(void){
    char reponse[15];
    while(strcmp(reponse, "quit") != 0){
        printf(EFFACE);
        printf("\n======================================== \n");
        printf(CIEL "           LES REGLES DE JEU" RESET);
        printf("\n======================================== \n");
        printf("\n- La bataille navale oppose deux joueurs");
        printf("\n- Chaque joueur dispose de deux grilles carrées dont les lignes sont numérotées de 1 à 10 et les colonnes de A à J");
        printf("\n- Chaque joueur possède une flotte de cinq bateau qu'il doit disposer sur sa première grille en début de partie");
        printf("\n- L'autre grille représente la zone adverse, où il cherchera à couler les bateaux de son adversaire");
        printf("\n- Chaque joueur, à son tour, annonce une case, et son adversaire lui répond si le tir tombe à l'eau ou au contraire s'il touche un bateau.");
        printf("\n- La partie se termine lorsque l'un des deux joueurs ne possède plus de bateau viable sur se grille \n");
        printf("\nPour retourner au menu principal, tapez " CYAN "quit \n" RESET);
        printf("\nVotre réponse : ");
        scanf(" %s", reponse);
    }
    MenuPrincipal();
}

void ModeDeJeu(void){
    char reponse[15];
    while((strcmp(reponse, "quit") != 0) && (strcmp(reponse, "1vs1") != 0) && (strcmp(reponse, "ordi") != 0)){
        printf(EFFACE);
        printf("\n======================================== \n");
        printf(CIEL "        SELECTION DU MODE DE JEU" RESET);
        printf("\n======================================== \n");
        printf("\nPour démarrer une partie joueur conte joueur, tapez " CYAN "1vs1" RESET);
        printf("\nPour démarrer une partie contre l'ordinateur, tapez " CYAN "ordi \n" RESET);
        printf("\nPour retourner au menu principal, tapez " CYAN "quit \n" RESET);
        printf("\nVotre réponse : ");
        scanf(" %s", reponse);
    }
    if(strcmp(reponse, "quit") == 0){
        MenuPrincipal();
    }
    if(strcmp(reponse, "1vs1") == 0){
        NomDesJoueurs();
    }
    if(strcmp(reponse, "ordi") == 0){
        ChoixDifficulte();
    }
}

void Classement(void){
    char reponse[15];
    while(strcmp(reponse, "quit") != 0 && (strcmp(reponse, "1vs1") != 0) && (strcmp(reponse, "ordi") != 0)){
        printf(EFFACE);
        printf("\n======================================== \n");
        printf(CIEL "              CLASSEMENT" RESET);
        printf("\n======================================== \n");
        printf("\nPour accéder au classement joueur contre joueur, tapez " CYAN "1vs1" RESET);
        printf("\nPour accéder au classement joueur contre ordinateur, tapez " CYAN "ordi \n" RESET);
        printf("\nPour retourner au menu principal, tapez " CYAN "quit \n" RESET);
        printf("\nVotre réponse : ");
        scanf(" %s", reponse);
    }
    if(strcmp(reponse, "quit") == 0){
        MenuPrincipal();
    }
    if(strcmp(reponse, "1vs1") == 0){
        Classement1vs1();
    }
    if(strcmp(reponse, "ordi") == 0){
        ChoixClassementOrdi();
    }
}

void NomDesJoueurs(void){
    Joueur * j1 = creerJoueur();
    Joueur * j2 = creerJoueur();
    printf(EFFACE);
    printf("\n======================================== \n");
    printf(CIEL "         SELECTION DES JOUEURS" RESET);
    printf("\n======================================== \n");
    printf("\nEntrer le nom du premier joueur \n");
    printf("\nPour retourner au menu précédent, tapez " CYAN "quit \n" RESET);
    printf("\nVotre réponse : ");
    scanf(" %s", j1 -> NomJoueur);
    if(strcmp(j1 -> NomJoueur, "quit") == 0){
        ModeDeJeu();
    }
    else{
        printf(EFFACE);
        printf("\n======================================== \n");
        printf(CIEL "         SELECTION DES JOUEURS" RESET);
        printf("\n======================================== \n");
        printf("\nEntrer le nom du deuxième joueur \n");
        printf("\nPour retourner au menu précédent, tapez " CYAN "quit \n" RESET);
        printf("\nVotre réponse : ");
        scanf(" %s", j2 -> NomJoueur);
        if(strcmp(j2 -> NomJoueur, "quit") == 0){
            ModeDeJeu();
        }
        else{
            FILE * f = fopen("registre_1vs1.txt", "a+");
            if(Appartient(f, j1 -> NomJoueur) == 0){
                fprintf(f,"%s %d \n", j1 -> NomJoueur, 0);
            }
            if(Appartient(f, j2 -> NomJoueur) == 0){
                fprintf(f,"%s %d \n", j2 -> NomJoueur, 0);
            }
            fclose(f);
        }
    }
    jouerPartie(j1, j2);
}

void NomDuJoueur(char difficulte[15]){
    Joueur * j = creerJoueur();
    
    printf(EFFACE);
    printf("\n======================================== \n");
    printf(CIEL "         SELECTION DU JOUEUR" RESET);
    printf("\n======================================== \n");
    printf("\nEntrer le nom du joueur \n");
    printf("\nPour retourner au menu précédent, tapez " CYAN "quit \n" RESET);
    printf("\nVotre réponse : ");
    scanf(" %s",  j -> NomJoueur);

    if(strcmp(j -> NomJoueur, "quit") == 0){
        ChoixDifficulte();
    }
    else{
        // Enregistrement des fichiers (Ton code existant)
        if(strcmp(difficulte, "facile") == 0){
            FILE * f = fopen("registre_facile.txt", "a+");
            if(Appartient(f, j -> NomJoueur) == 0){
            fprintf(f,"%s %d \n", j -> NomJoueur, 0);
            fclose(f);
            }
        }
        if(strcmp(difficulte, "moyen") == 0){
            FILE * f = fopen("registre_moyen.txt", "a+");
            if(Appartient(f, j -> NomJoueur) == 0){
            fprintf(f,"%s %d \n", j -> NomJoueur, 0);
            fclose(f);
            }
        }
        if(strcmp(difficulte, "difficile") == 0){
            FILE * f = fopen("registre_difficile.txt", "a+");
            if(Appartient(f, j -> NomJoueur) == 0){
            fprintf(f,"%s %d \n", j -> NomJoueur, 0);
            fclose(f);
            }
        }
        
        jouerPartieOrdi(j, difficulte);
    }
}

void AjouterVictoireOrdi(Joueur * j, char difficulte[15]){
    char NomRegistre[30];
     if(strcmp(difficulte, "facile") == 0){
        strcpy(NomRegistre, "registre_facile.txt");
    }
    if(strcmp(difficulte, "moyen") == 0){
        strcpy(NomRegistre, "registre_moyen.txt");
    }
    if(strcmp(difficulte, "difficile") == 0){
        strcpy(NomRegistre, "registre_difficile.txt");
    }
    FILE * f = fopen(NomRegistre, "r");
    FILE * fichiertemporaire = fopen("registretemporaire.txt", "w");
    Joueur joueurtemporaire;
    rewind(f);
    while(fscanf(f, "%s %d", joueurtemporaire.NomJoueur, &joueurtemporaire.NbVictoire) == 2){
        if(strcmp(joueurtemporaire.NomJoueur, j -> NomJoueur) == 0){
            joueurtemporaire.NbVictoire += 1;
            fprintf(fichiertemporaire, "%s %d \n", joueurtemporaire.NomJoueur, joueurtemporaire.NbVictoire);
        }
        else{
            fprintf(fichiertemporaire, "%s %d \n", joueurtemporaire.NomJoueur, joueurtemporaire.NbVictoire);
        }
    }
    fclose(f);
    fclose(fichiertemporaire);
    remove(NomRegistre);
    rename("registretemporaire.txt", NomRegistre);
}

void AjouterVictoire1vs1(Joueur * j){
    FILE * f = fopen("registre_1vs1.txt", "r");
    FILE * fichiertemporaire = fopen("registretemporaire.txt", "w");
    Joueur joueurtemporaire;
    rewind(f);
    while(fscanf(f, "%s %d", joueurtemporaire.NomJoueur, &joueurtemporaire.NbVictoire) == 2){
        if(strcmp(joueurtemporaire.NomJoueur, j -> NomJoueur) == 0){
            joueurtemporaire.NbVictoire += 1;
            fprintf(fichiertemporaire, "%s %d \n", joueurtemporaire.NomJoueur, joueurtemporaire.NbVictoire);
        }
        else{
            fprintf(fichiertemporaire, "%s %d \n", joueurtemporaire.NomJoueur, joueurtemporaire.NbVictoire);
        }
    }
    fclose(f);
    fclose(fichiertemporaire);
    remove("registre_1vs1.txt");
    rename("registretemporaire.txt", "registre_1vs1.txt");
}

void ChoixClassementOrdi(void){
    char reponse[15];
    while((strcmp(reponse, "quit") != 0)&&(strcmp(reponse, "facile") != 0)&&(strcmp(reponse, "moyen") != 0)&&(strcmp(reponse, "difficile") != 0)){
        printf(EFFACE);
        printf("\n======================================== \n");
        printf(CIEL "  CLASSEMENT JOUEUR CONTRE ORDINATEUR" RESET);
        printf("\n======================================== \n");
        printf("\nPour accéder au classement difficulté facile, tapez " CYAN "facile" RESET);
        printf("\nPour accéder au classement difficulté moyenne tapez " CYAN "moyen" RESET);
        printf("\nPour accéder au classement difficulté difficile, tapez " CYAN "difficile" RESET);
        printf("\n\nPour retourner au menu principal, tapez " CYAN "quit \n" RESET);
        printf("\nVotre réponse : ");
        scanf(" %s", reponse);
    }
    if(strcmp(reponse, "quit") == 0){
        Classement();
    }
    if(strcmp(reponse, "facile") == 0){
        ClassementOrdi(reponse);
    }
    if(strcmp(reponse, "moyen") == 0){
        ClassementOrdi(reponse);
    }
    if(strcmp(reponse, "difficile") == 0){
        ClassementOrdi(reponse);
    }
}

void Classement1vs1(void){
    char reponse[15];
    while(strcmp(reponse, "quit") != 0){
        FILE * f = fopen("registre_1c1.txt", "r");
        printf(EFFACE);
        printf("\n======================================== \n");
        printf(CIEL "    CLASSEMENT JOUEUR CONTRE JOUEUR" RESET);
        printf("\n======================================== \n");
        AfficherClassement(f);
        fclose(f);
        printf("\n\nPour retourner au menu principal, tapez " CYAN "quit \n" RESET);
        printf("\nVotre réponse : ");
        scanf(" %s", reponse);
    }
    Classement();
}

int ComparerScores(const void * a, const void * b){
    Joueur *joueurA = (Joueur *)a;
    Joueur *joueurB = (Joueur *)b;
    return joueurB->NbVictoire - joueurA->NbVictoire;
}

void AfficherClassement(FILE * f){
    if (f == NULL) {
        printf("Erreur : impossible d'ouvrir le fichier \n");
        return;
    }
    Joueur tabjoueur[100];
    int NbJoueur = 0;
    while(fscanf(f, "%s %d", tabjoueur[NbJoueur].NomJoueur, &tabjoueur[NbJoueur].NbVictoire) == 2){
        NbJoueur++;
    }
    qsort(tabjoueur, NbJoueur, sizeof(Joueur), ComparerScores);
    for (int i = 0; i < NbJoueur; i++) {
        printf("\n%d. %s : %d", i+1, tabjoueur[i].NomJoueur, tabjoueur[i].NbVictoire);
    }
}

void MenuFinDePartie(Joueur * j){
    char reponse[15];
    while((strcmp(reponse, "quit") != 0) && (strcmp(reponse, "stat") != 0)){
        printf(EFFACE);
        printf("\n======================================== \n");
        printf(CIEL "           Victoire de %s" RESET, j -> NomJoueur);
        printf("\n======================================== \n");
        printf("\nPour redémarrer une partie, tapez " CYAN "play" RESET);
        printf("\nPour visualiser les statistiques de la partie, tapez " CYAN "stat" RESET);
        printf("\n\nPour retourner au menu principal, tapez " CYAN "quit \n" RESET);
        printf("\nVotre réponse : ");
        scanf(" %s", reponse);
    }
    if(strcmp(reponse, "quit") == 0){
        MenuPrincipal();
    }
    if(strcmp(reponse, "stat") == 0){
        // Afficher les statistiques de la partie (moyenne de temps des coups, nombre de case explore, nombre de bateau coulé)
    }
    if(strcmp(reponse, "play") == 0){
        ModeDeJeu();
    }
}


void ChoixDifficulte(void){
    char reponse[15];
    while((strcmp(reponse, "quit") != 0)&&(strcmp(reponse, "facile") != 0)&&(strcmp(reponse, "moyen") != 0)&&(strcmp(reponse, "difficile") != 0)){
        printf(EFFACE);
        printf("\n======================================== \n");
        printf(CIEL "         CHOIX DE LA DIFFICULTÉ" RESET);
        printf("\n======================================== \n");
        printf("\nPour choisir la difficulté facile, tapez " CYAN "facile" RESET);
        printf("\nPour choisir la difficulté moyenne, tapez " CYAN "moyen" RESET);
        printf("\nPour choisir la difficulté difficile, tapez " CYAN "difficile" RESET);
        printf("\n\nPour retourner au menu précédent, tapez " CYAN "quit \n" RESET);
        printf("\nVotre réponse : ");
        scanf(" %s", reponse);
    }
    if(strcmp(reponse, "quit") == 0){
        ModeDeJeu();
    }
    if(strcmp(reponse, "facile") == 0){
        NomDuJoueur(reponse);
    }
    if(strcmp(reponse, "moyen") == 0){
        NomDuJoueur(reponse);
    }
    if(strcmp(reponse, "difficile") == 0){
        NomDuJoueur(reponse);
    }
}

void ClassementOrdi(char difficulte[15]){
    char reponse[15];
    while(strcmp(reponse, "quit") != 0){
        if(strcmp(difficulte, "facile") == 0){
            FILE * f = fopen("registre_facile.txt", "r");
            printf(EFFACE);
            printf("======================================== \n");
            printf(CIEL "  CLASSEMENT JOUEUR CONTRE ORDINATEUR" RESET);
            printf("\n======================================== \n");
            AfficherClassement(f);
            fclose(f);
            printf("\n\nPour retourner au menu principal, tapez " CYAN "quit \n" RESET);
            printf("\nVotre réponse : ");
            scanf(" %s", reponse);
        }
        if(strcmp(difficulte, "moyen") == 0){
            FILE * f = fopen("registre_moyen.txt", "r");
            printf(EFFACE);
            printf("======================================== \n");
            printf(CIEL "  CLASSEMENT JOUEUR CONTRE ORDINATEUR" RESET);
            printf("\n======================================== \n");
            AfficherClassement(f);
            fclose(f);
            printf("\n\nPour retourner au menu principal, tapez " CYAN "quit \n" RESET);
            printf("\nVotre réponse : ");
            scanf(" %s", reponse);
        }
        if(strcmp(difficulte, "difficile") == 0){
            FILE * f = fopen("registre_difficile.txt", "r");
            printf(EFFACE);
            printf("======================================== \n");
            printf(CIEL "  CLASSEMENT JOUEUR CONTRE ORDINATEUR" RESET);
            printf("\n======================================== \n");
            AfficherClassement(f);
            fclose(f);
            printf("\n\nPour retourner au menu principal, tapez " CYAN "quit \n" RESET);
            printf("\nVotre réponse : ");
            scanf(" %s", reponse);
        }
    }
    ChoixClassementOrdi();
}

int Appartient(FILE * f, char NomJoueur[]){
    int v = 0;
    Joueur temporaire;
    rewind(f);
    while(fscanf(f, "%s %d", temporaire.NomJoueur, &temporaire.NbVictoire) == 2){
        if(strcmp(temporaire.NomJoueur, NomJoueur) == 0){
            v = 1;
            break;
        }
    }
    return v;
}

