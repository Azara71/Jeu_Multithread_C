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

/*
* Fonction permettant de trouver la première place libre pour un artefact dans son inventaire.
*/
int premiere_place_libre_inventaire_artefact(hero_t hero){
int i=0;
    for(i=0;i<5;i++){
       if(strcmp(hero.liste_artefact[i].name,"-1")==0){
           return i;
       };
    }
return i;

}
/*
* Fonction permettant à un héro de ramasser un artefact
*/
void ramasser_artefact(hero_t hero,artefact_t artefact){
int k=premiere_place_libre_inventaire_artefact(hero);
    if(k<5){
        hero.liste_artefact[k]=artefact;
    }
}
