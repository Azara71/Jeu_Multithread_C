#ifndef _MONSTRE_
#define _MONSTRE_
#include "ncurses.h"

typedef struct monstre_type{

char name[1024];
unsigned short health;
unsigned short armor;
unsigned short strength;
unsigned short speed;
unsigned short attackspeed;

}monstre_t;


monstre_t create_monster(char* name,unsigned short health,unsigned short armor,unsigned short strength,unsigned short speed,unsigned short attackspeed);

monstre_t creer_monstre(WINDOW* fenetre_creation_monstre,WINDOW* fenetre_informations);
void afficher_stat_monstre(WINDOW*fenetre_informations,monstre_t monstre);


#endif
