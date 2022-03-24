#include "monstre.h"
#include "string.h"
#include "stdlib.h"
#include <ncurses.h>    // Pour les fonctions/constantes ncurses
#define TAILLENOMMONSTRE 1024


monstre_t create_monster(char* name,unsigned short health,unsigned short armor,unsigned short strength,unsigned short speed,unsigned short attackspeed){
monstre_t monstre;
strcpy(monstre.name,name);
monstre.health=health;
monstre.armor=armor;
monstre.strength=strength;
monstre.speed=speed;
monstre.attackspeed=attackspeed;
return monstre;
}

/* FONCTION PERMETTANT A PARTIR DE 
LA FENETRE DE CREATION D'UN MONSTRE DE 
LE CREER
*/
monstre_t creer_monstre(WINDOW* fenetre_creation_monstre,WINDOW* fenetre_informations){ 
monstre_t monstretoreturn=create_monster("-1",1,1,1,1,1);

/*
 PARTI
* 
NAME
*/
echo();
wprintw(fenetre_creation_monstre,"\n Donnez le nom du monstre");
box(fenetre_creation_monstre,0,0);
wrefresh(fenetre_creation_monstre);
wscanw(fenetre_creation_monstre,"%1023s",monstretoreturn.name);
/*PARTI
*
 HP*/
wprintw(fenetre_creation_monstre,"\n Donnez le nombre d'hp");
box(fenetre_creation_monstre,0,0);
wrefresh(fenetre_creation_monstre);
wscanw(fenetre_creation_monstre,"%d",&monstretoreturn.health);
/* PARTI
*
ARMOR */
wprintw(fenetre_creation_monstre,"\n Point d'armure");
box(fenetre_creation_monstre,0,0);
wrefresh(fenetre_creation_monstre);
wscanw(fenetre_creation_monstre,"%d",&monstretoreturn.armor);
/*PARTI
*
STRENGTH */
wprintw(fenetre_creation_monstre,"\n Point de force");
box(fenetre_creation_monstre,0,0);
wrefresh(fenetre_creation_monstre);
wscanw(fenetre_creation_monstre,"%d",&monstretoreturn.strength);
/* PARTI 
*SPEED 
*/
wprintw(fenetre_creation_monstre,"\n Point de speed");
wrefresh(fenetre_creation_monstre);
wscanw(fenetre_creation_monstre,"%d",&monstretoreturn.speed);
/* PARTI 
*
ATTACKSPEED*/
wprintw(fenetre_creation_monstre,"\n Donnez la vitesse d'attaque");
wrefresh(fenetre_creation_monstre);
wscanw(fenetre_creation_monstre,"%d\n",&monstretoreturn.attackspeed);
box(fenetre_creation_monstre,0,0);

wrefresh(fenetre_creation_monstre);

noecho();

return monstretoreturn;

}

void afficher_stat_monstre(WINDOW*fenetre_informations,monstre_t monstre){
wprintw(fenetre_informations,"\nNOM :%s",monstre.name);
wprintw(fenetre_informations," health :%d",monstre.health);
wprintw(fenetre_informations," armor :%d",monstre.armor);
wprintw(fenetre_informations," strength :%d",monstre.strength);
wprintw(fenetre_informations," speed :%d",monstre.speed);
wprintw(fenetre_informations," attackspeed :%d",monstre.attackspeed);


wrefresh(fenetre_informations);

}
