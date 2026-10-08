# Bataille navale

Un jeu de bataille navale en console, écrit en C.

Projet réalisé en équipe de trois étudiants à l'EILCO (première année du cycle ingénieur informatique), dans le cadre du module d'algorithmique avancée et programmation, d'octobre 2025 à janvier 2026.

## Fonctionnalités

- Grille de 10 × 10 et flotte de cinq navires par joueur
- Mode joueur contre joueur sur le même terminal
- Mode joueur contre l'ordinateur, avec trois niveaux de difficulté
- Sauvegarde et reprise d'une partie (fichiers binaires)
- Affichage en couleur dans le terminal

## Compiler et lancer

```bash
gcc -Wall -Wextra *.c -o bataille-navale
./bataille-navale
```

Le jeu fonctionne sous Windows et sous Linux.

## Organisation du code

| Module       | Rôle                                                        |
| ------------ | ----------------------------------------------------------- |
| `main`       | Point d'entrée, lance le menu principal                     |
| `jeu`        | Boucle de jeu, alternance des tours, victoire, adversaire   |
| `grille`     | Allocation et gestion des plateaux, résolution des tirs     |
| `bateau`     | Structure des navires, placement, détection des collisions  |
| `affichage`  | Menus, couleurs et saisies                                  |
| `sauvegarde` | Écriture et lecture des parties sauvegardées                |

## Pistes d'amélioration

- Un mode réseau en sockets TCP/IP, pour jouer depuis deux machines
- Une interface graphique
