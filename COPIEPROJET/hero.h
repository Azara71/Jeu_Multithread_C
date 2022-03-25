#ifndef __HERO__
#define __HERO__
#include "artefact.h"

typedef struct hero_type{

char name[1024];
unsigned short health;
unsigned short health_max;
unsigned short armor;
unsigned short strength;
unsigned short speed;
unsigned short attackspeed;
unsigned short level;
unsigned short experience;
unsigned short cooX;
unsigned short cooY;
unsigned short carteX;
unsigned short carteY;
int nb_piece_grand_tout;
artefact_t liste_artefact[5];


}hero_t;


hero_t create_hero(char* name);
void afficher_hero(hero_t hero);
#endif