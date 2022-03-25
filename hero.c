#include "hero.h"
#include "artefact.h"
#include "string.h"

hero_t create_hero(char* name){
    hero_t hero;
    strcpy(hero.name,name);
    hero.health=10;
    hero.health_max=10;
    hero.armor=3;
    hero.strength=2;
    hero.speed=1;
    hero.attackspeed=1;
    hero.level=1;
    hero.experience=0;
    hero.cooX=5;
    hero.cooY=5;
    hero.nb_piece_grand_tout=0;
    hero.carteX=0;
    hero.carteY=0;
    

    for(int i=0;i<5;i++){
         hero.liste_artefact[i]=create_artefact("-1",0,0,0,0,0);
    }
return hero;
}
