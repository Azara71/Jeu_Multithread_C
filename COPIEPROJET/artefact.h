#ifndef _ARTEFACT_
#define _ARTEFACT_
#include <ncurses.h>
typedef struct artefact_type{

char name[1024];
unsigned short health;
unsigned short armor;
unsigned short strength;
unsigned short speed;
unsigned short attackspeed;

}artefact_t;


artefact_t create_artefact(char* name,unsigned short health,unsigned short armor,unsigned short strength,unsigned short speed,unsigned short attackspeed);
void afficher_stat_artefact(WINDOW*fenetre_informations,artefact_t artefact);
artefact_t creer_artefact(WINDOW* fenetre_creation_artefact,WINDOW* fenetre_informations);



#endif
