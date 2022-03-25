
#include <ncurses.h>
#include "string.h"
#include "stdlib.h"
#include "artefact.h"

artefact_t create_artefact(char* name,unsigned short health,unsigned short armor,unsigned short strength,unsigned short speed,unsigned short attackspeed){
artefact_t artefact;
strcpy(artefact.name,name);
artefact.health=health;
artefact.armor=armor;
artefact.strength=strength;
artefact.speed=speed;
artefact.attackspeed=attackspeed;
return artefact;
}

artefact_t creer_artefact(WINDOW* fenetre_creation_artefact,WINDOW* fenetre_informations){ 
artefact_t artefacttoreturn=create_artefact("-1",1,1,1,1,1);

/*
 PARTI
* 
NAME
*/
echo();
wprintw(fenetre_creation_artefact,"\n Donnez le nom de l'artefact");
box(fenetre_creation_artefact,0,0);
wrefresh(fenetre_creation_artefact);
wscanw(fenetre_creation_artefact,"%1023s",artefacttoreturn.name);
/*PARTI
*
 HP*/
wprintw(fenetre_creation_artefact,"\n Donnez le nombre d'hp");
box(fenetre_creation_artefact,0,0);
wrefresh(fenetre_creation_artefact);
wscanw(fenetre_creation_artefact,"%d",&artefacttoreturn.health);
/* PARTI
*
ARMOR */
wprintw(fenetre_creation_artefact,"\n Point d'armure");
box(fenetre_creation_artefact,0,0);
wrefresh(fenetre_creation_artefact);
wscanw(fenetre_creation_artefact,"%d",&artefacttoreturn.armor);
/*PARTI
*
STRENGTH */
wprintw(fenetre_creation_artefact,"\n Point de force");
box(fenetre_creation_artefact,0,0);
wrefresh(fenetre_creation_artefact);
wscanw(fenetre_creation_artefact,"%d",&artefacttoreturn.strength);
/* PARTI 
*SPEED 
*/
wprintw(fenetre_creation_artefact,"\n Point de speed");
wrefresh(fenetre_creation_artefact);
wscanw(fenetre_creation_artefact,"%d",&artefacttoreturn.speed);
/* PARTI 
*
ATTACKSPEED*/
wprintw(fenetre_creation_artefact,"\n Donnez la vitesse d'attaque");
wrefresh(fenetre_creation_artefact);
wscanw(fenetre_creation_artefact,"%d\n",&artefacttoreturn.attackspeed);
box(fenetre_creation_artefact,0,0);

wrefresh(fenetre_creation_artefact);

noecho();

return artefacttoreturn;

}

void afficher_stat_artefact(WINDOW*fenetre_informations,artefact_t artefact){

wprintw(fenetre_informations,"\nNOM :%s",artefact.name);
wprintw(fenetre_informations," health :%d",artefact.health);
wprintw(fenetre_informations," armor :%d",artefact.armor);
wprintw(fenetre_informations," strength :%d",artefact.strength);
wprintw(fenetre_informations," speed :%d",artefact.speed);
wprintw(fenetre_informations," attackspeed :%d",artefact.attackspeed);

wrefresh(fenetre_informations);
}


