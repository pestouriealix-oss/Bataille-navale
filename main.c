/**
 * ============================================================================
 * FICHIER : main.c
 * ----------------------------------------------------------------------------
 * Auteur  : Julien, Alix, Enzo
 * Date    : 2025
 * * Description :
 * Point d'entrée principal du programme "Bataille Navale".
 * Ce fichier a deux responsabilités uniques :
 * 1. Initialiser le générateur de nombres aléatoires (pour l'IA).
 * 2. Lancer le Menu Principal.
 * * Une fois le menu lancé, le contrôle du programme est transféré à 
 * affichage.c et jeu.c. Le main ne reprend la main qu'à la toute fin
 * pour quitter proprement.
 * ============================================================================
 */

#include "librairies.h" // Pour srand(), rand(), time() et les types standards
#include "affichage.h"  // Pour accéder à MenuPrincipal()

/* Les inclusions suivantes ne sont pas strictement nécessaires ici si le main
   n'utilise pas directement les structures, mais on peut les laisser pour la clarté */
#include "jeu.h"
#include "bateau.h"
#include "grille.h"


int main(void) {
    
    /* ---------------------------------------------------------------------
     * 1. Initialisation de l'aléatoire (CRUCIAL)
     * ---------------------------------------------------------------------
     * Pourquoi cette ligne ?
     * - L'ordinateur ne sait pas générer de "vrai" hasard. Il utilise une suite
     * de calculs mathématiques prévisibles (pseudo-aléatoire).
     * - Si on ne change pas la "graine" (seed) de départ, la suite est toujours
     * la même. L'ordinateur tirerait toujours aux mêmes endroits à chaque partie !
     * * Comment ça marche ?
     * - time(NULL) renvoie le nombre de secondes écoulées depuis 1970.
     * - Comme ce nombre change chaque seconde, la graine change à chaque
     * lancement du programme.
     * --------------------------------------------------------------------- */
    srand(time(NULL));


    /* ---------------------------------------------------------------------
     * 2. Lancement du jeu
     * ---------------------------------------------------------------------
     * On délègue immédiatement le contrôle au module d'affichage.
     * Le programme va rester "bloqué" dans cette fonction tant que le joueur
     * navigue dans les menus ou joue une partie.
     * Il ne reviendra à la ligne suivante que lorsque le joueur choisira "Quitter".
     * --------------------------------------------------------------------- */
    MenuPrincipal();


    /* ---------------------------------------------------------------------
     * 3. Fin du programme
     * ---------------------------------------------------------------------
     * Si on arrive ici, c'est que l'utilisateur a quitté le menu principal.
     * return 0 signale au système d'exploitation que tout s'est bien passé.
     * --------------------------------------------------------------------- */
    return 0;
}
