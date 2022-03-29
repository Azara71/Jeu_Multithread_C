#ifndef __HERO__
#define __HERO__
#include "artefact.h"
#include "monstre.h"
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
int carteX;
int carteY;
int nb_piece_grand_tout;
artefact_t liste_artefact[5];


}hero_t;

/*
* Crée un héro avec des stats de base
*/
hero_t create_hero(char* name);
/*
* Fonction permettant de trouver la première place libre pour un artefact dans son inventaire.
*/
int premiere_place_libre_inventaire_artefact(hero_t hero);
/*
* Fonction permettant à un héro de ramasser un artefact
*/
hero_t ramasser_artefact(hero_t hero,artefact_t artefact,int k);
void afficher_stat_hero(WINDOW* win,hero_t hero);
int fight(hero_t hero,monstre_t monstre);


#endif