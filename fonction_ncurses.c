#include "fonction_ncurses.h"

#include <stdlib.h>    // Pour exit, EXIT_FAILURE

#define BLUE 2
#define YELLOW 3
#define GREEN 4
#define RED 5
/**
 * Initialisation de ncurses.
 */
void ncurses_init() {
  initscr();	        // Démarre le mode ncurses
  cbreak();	            // Désactive la mise en buffer
  noecho();             // Désactive l'affichage des caractères saisis
  keypad(stdscr, TRUE);	// Active les touches spécifiques (flèches)
  refresh();            // Met à jour l'affichage
  curs_set(FALSE);      // Masque le curseur
}

/**
 * Fin de ncurses.
 */
void ncurses_stop() {
  endwin();
}

/*
*PALETTE NCURSES
*/

void palette() {      
  init_pair(BLUE,COLOR_BLACK,COLOR_BLUE);
  init_pair(YELLOW,COLOR_BLACK,COLOR_YELLOW);
  init_pair(GREEN,COLOR_BLACK,COLOR_GREEN);
  init_pair(RED,COLOR_BLACK,COLOR_RED);
  init_pair(38, COLOR_BLUE, COLOR_BLACK); 
  init_pair(169, COLOR_RED, COLOR_BLACK);
  init_pair(34, COLOR_GREEN, COLOR_BLACK);
}
/**
 * Initialisation des couleurs.
 */
void ncurses_couleurs() {
  // Verification du support de la couleur
  if(has_colors() == FALSE) {
    ncurses_stop();
    fprintf(stderr, "Le terminal ne supporte pas les couleurs.\n");
    exit(EXIT_FAILURE);
  }

  // Activation des couleurs
  start_color();
}

/**
 * Initialisation de la souris.
 */
void ncurses_initsouris() {
  if(!mousemask(BUTTON1_PRESSED, NULL)) {
    ncurses_stop();
    fprintf(stderr, "Pas de gestion de la souris.\n");
    exit(EXIT_FAILURE);
  }
}

/**
 * Récupération de la position de la souris.
 */
int souris_getpos(int *x, int *y) {
  MEVENT event;
  int resultat = getmouse(&event);
  if(resultat == OK) {
    *x = event.x;
    *y = event.y;
  }
  return resultat;
}

/*
*CREATION DES DIFFERENTES FENETRES NCURSES
*
*
*
*/


WINDOW *creer_fenetre_outils(){
  // CREATION DE LA FENETRE D'OUTILS
  WINDOW *fenetre_outils;
  fenetre_outils=newwin(18,22,10,42);
  box(fenetre_outils,0,0);
  mvwprintw(fenetre_outils,0,1,"Outils");
 wattron(fenetre_outils,A_UNDERLINE);	
  mvwprintw(fenetre_outils,1,2,"___COULEUR___");
   wattroff(fenetre_outils,A_UNDERLINE);	


  // EAU
  wattron(fenetre_outils,COLOR_PAIR(BLUE));
  mvwprintw(fenetre_outils,2,1," ");
  wattroff(fenetre_outils,COLOR_PAIR(BLUE));
  mvwprintw(fenetre_outils,2,2,"Eau");
  // SABLE
  wattron(fenetre_outils,COLOR_PAIR(YELLOW));
  mvwprintw(fenetre_outils,3,1," ");
  wattroff(fenetre_outils,COLOR_PAIR(YELLOW));
  mvwprintw(fenetre_outils,3,2,"Sable");
  // Herbe
  wattron(fenetre_outils,COLOR_PAIR(GREEN));
  mvwprintw(fenetre_outils,4,1," ");
  wattroff(fenetre_outils,COLOR_PAIR(GREEN));
  mvwprintw(fenetre_outils,4,2,"Herbe");
  // Montagne
  wattron(fenetre_outils,COLOR_PAIR(RED));
  mvwprintw(fenetre_outils,5,1," ");
  wattroff(fenetre_outils,COLOR_PAIR(RED));
  mvwprintw(fenetre_outils,5,2,"Montagne");
  // Trésor
  wattron(fenetre_outils,A_UNDERLINE);	
  mvwprintw(fenetre_outils,6,2,"__ELEMENT__");
  wattroff(fenetre_outils,A_UNDERLINE);	

  mvwprintw(fenetre_outils,7,1,"$:");
  mvwprintw(fenetre_outils,7,3,"Tresor");

  //Monstre
  mvwprintw(fenetre_outils,8,1,"M:");
  mvwprintw(fenetre_outils,8,3,"Monstre");
 // wrefresh(fenetre_outils);
  
  //Monstre
  mvwprintw(fenetre_outils,9,1,"A:");
  mvwprintw(fenetre_outils,9,3,"Artefact");
// Obstacle
  mvwprintw(fenetre_outils,10,1,"X:");
  mvwprintw(fenetre_outils,10,3,"Obstacle");

 wattron(fenetre_outils,A_UNDERLINE);	
  mvwprintw(fenetre_outils,11,2,"__AUTRES__");
  wattroff(fenetre_outils,A_UNDERLINE);	

  mvwprintw(fenetre_outils,12,1,"i:");
  mvwprintw(fenetre_outils,12,3,"Informations");
  
  mvwprintw(fenetre_outils,13,1,"G:");
  mvwprintw(fenetre_outils,13,3,"Gomme");
  return fenetre_outils;
}

WINDOW *creer_fenetre_carte(){
  // CREATION DE LA FENETRE CARTE
  WINDOW* fenetre_carte;
  fenetre_carte=newwin(22,42,10,00);
  box(fenetre_carte,0,0);
  mvwprintw(fenetre_carte,0,1,"Cartes");
return fenetre_carte;
}

WINDOW *creer_fenetre_informations(){
  // CREATION DE LA FENETRE INFORMATIONS
  WINDOW *fenetre_informations;
  fenetre_informations = newwin(10, 60, 0, 0);
  box(fenetre_informations, 0, 0);
  mvwprintw(fenetre_informations, 0, 1, "Informations");
  mvwprintw(fenetre_informations,1,1,"Tapez 'Q' pour quitter");
  scrollok(fenetre_informations,TRUE);
  return fenetre_informations;
}

WINDOW* creer_fenetre_creation_monstre(){
  // CREATION DE LA FENETRE INFORMATIONS
  WINDOW* fenetre_creation_monstre;
  fenetre_creation_monstre=newwin(10,50,0,60);
  box(fenetre_creation_monstre,0,0);
  mvwprintw(fenetre_creation_monstre, 0, 1, "Creation Monstre/Artefact");
  scrollok(fenetre_creation_monstre,TRUE);
  return fenetre_creation_monstre;
}

WINDOW* creer_fenetre_stat(){
  // CREATION DE LA FENETRE INFORMATIONS
  WINDOW* fenetre_stat;
  fenetre_stat=newwin(22,40,10,43);
  box(fenetre_stat,0,0);
  mvwprintw(fenetre_stat, 0, 1, "Statistiques et equipements");
  scrollok(fenetre_stat,TRUE);
  return fenetre_stat;
}


void refresh_fenetre(WINDOW* fenetre_1,WINDOW* fenetre_2,WINDOW* fenetre_3){
// REFRESH TOUTES LES FENETRES EN MEME TEMPS
wrefresh(fenetre_1);
wrefresh(fenetre_2);
wrefresh(fenetre_3);

}