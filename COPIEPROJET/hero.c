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

void afficher_hero(hero_t hero){
        printf("%s\n",hero.name);
        printf("%d / %d\n",hero.health,hero.health_max);
        printf("%d\n", hero.armor);
        printf("%d\n", hero.strength);
        printf("%d\n", hero.speed);
        printf("%d\n", hero.attackspeed);
        printf("%d\n", hero.level);
        printf("%d\n", hero.experience);
        printf("%d\n", hero.cooX);
        printf("%d\n", hero.cooY);
        printf("%d\n", hero.nb_piece_grand_tout);
        printf("%d\n", hero.carteX);
        printf("%d\n", hero.carteY);

 for(int i=0;i<5;i++){
    if(strcmp(hero.liste_artefact[i].name,"-1")!=0){
        printf("%s\n",hero.liste_artefact[i].name);
    }
 }
}
