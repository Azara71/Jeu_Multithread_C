#ifndef _FONCTIONS_
#define _FONCTIONS_
#include <ncurses.h>   // Pour toutes les fonctions/constantes ncurses

/**
 * Initialisation de ncurses.
 */
void ncurses_init();
/*
* INI DES COULEURS DE LA PALETTE
*/
void palette();
/**
 * Fin de ncurses.
 */
void ncurses_stop();

/**
 * Initialisation des couleurs.
 */
void ncurses_couleurs();

/**
 * Initialisation de la souris.
 */
void ncurses_initsouris();

/**
 * Récupération de la position de la souris.
 */
int souris_getpos(int *x, int *y);

WINDOW *creer_fenetre_outils();
WINDOW *creer_fenetre_carte();
WINDOW *creer_fenetre_informations();
WINDOW* creer_fenetre_creation_monstre();
WINDOW* creer_fenetre_stat();
void refresh_fenetre(WINDOW* fenetre_1,WINDOW* fenetre_2,WINDOW* fenetre_3);
#endif
