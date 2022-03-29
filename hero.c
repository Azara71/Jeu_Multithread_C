#include "hero.h"
#include "artefact.h"
#include "string.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

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
hero_t ramasser_artefact(hero_t hero,artefact_t artefact,int k){
    hero.liste_artefact[k]=artefact;
    hero.health_max=hero.health_max+artefact.health;
    hero.armor=hero.armor+artefact.armor;
    hero.strength=hero.strength+artefact.strength;
    hero.speed=hero.speed+artefact.speed;
    hero.attackspeed=hero.attackspeed+artefact.attackspeed;

return hero;
}
/*
* FONCTION AFFICHANT LES STATS D'UN HERO DANS UNE FENETRE
*/
void afficher_stat_hero(WINDOW* win,hero_t hero){
char dataconverted[45];

mvwprintw(win,1,1,"NOM:");
mvwprintw(win,1,6,hero.name);

if(sprintf(dataconverted,"%d",hero.health)==EOF){
    perror("ERREUR DE CONVERSION DES HP");
    exit(EXIT_FAILURE);
};

mvwprintw(win,2,1,"HEALTH :");
mvwprintw(win,2,10,dataconverted);
mvwprintw(win,2,15,"/");

if(sprintf(dataconverted,"%d",hero.health_max)==EOF){
    perror("ERREUR DE CONVERSION DES HP");
    exit(EXIT_FAILURE);
};
mvwprintw(win,2,17,dataconverted);

if(sprintf(dataconverted,"%d",hero.armor)==EOF){
    perror("ERREUR DE CONVERSION DES HP");
    exit(EXIT_FAILURE);
};

mvwprintw(win,3,1,"ARMOR :");
mvwprintw(win,3,9,dataconverted);

if(sprintf(dataconverted,"%d",hero.strength)==EOF){
    perror("ERREUR DE CONVERSION DES HP");
    exit(EXIT_FAILURE);
};
mvwprintw(win,4,1,"FORCE :");
mvwprintw(win,4,9,dataconverted);

if(sprintf(dataconverted,"%d",hero.strength)==EOF){
    perror("ERREUR DE CONVERSION DES HP");
    exit(EXIT_FAILURE);
};
mvwprintw(win,4,1,"FORCE :");
mvwprintw(win,4,9,dataconverted);

if(sprintf(dataconverted,"%d",hero.level)==EOF){
    perror("ERREUR DE CONVERSION DES HP");
    exit(EXIT_FAILURE);
};
mvwprintw(win,5,1,"LEVEL :");
mvwprintw(win,5,9,dataconverted);

if(sprintf(dataconverted,"%d",hero.experience)==EOF){
    perror("ERREUR DE CONVERSION DES HP");
    exit(EXIT_FAILURE);
};
mvwprintw(win,6,1,"EXP :");
mvwprintw(win,6,9,dataconverted);


mvwprintw(win,6,1,"COORDONNEES:(");

if(sprintf(dataconverted,"%d",hero.carteX)==EOF){
    perror("ERREUR DE CONVERSION DES HP");
    exit(EXIT_FAILURE);
};
for(int i=14;i<17;i++){ // Suppression des - ou des décimaux pour affichage propre
    mvwprintw(win,6,i," ");
}
mvwprintw(win,6,14,dataconverted);
mvwprintw(win,6,17,",");
if(sprintf(dataconverted,"%d",hero.carteY)==EOF){
    perror("ERREUR DE CONVERSION DES HP");
    exit(EXIT_FAILURE);
};
for(int i=18;i<21;i++){ // Suppression des - ou des décimaux pour affichage propre
    mvwprintw(win,6,i," ");
}
mvwprintw(win,6,19,dataconverted);
mvwprintw(win,6,23,")");
mvwprintw(win,7,1,"PIECE DU GRAND TOUT:");

if(sprintf(dataconverted,"%d",hero.nb_piece_grand_tout)==EOF){
    perror("ERREUR DE CONVERSION DES HP");
    exit(EXIT_FAILURE);
};

mvwprintw(win,7,20,dataconverted);
mvwprintw(win,8,1,"__________");
mvwprintw(win,9,1,"EQUIPEMENT");

for(int i=10;i<15;i++)
{   if(strcmp(hero.liste_artefact[i-10].name,"-1")==0){
      mvwprintw(win,i,1,"SLOT LIBRE");
    }
    else{
        // EFFACAGE DE SLOT LIBRE
        for(int j=1;j<30;j++){ // Suppression des - ou des décimaux pour affichage propre
              mvwprintw(win,i,j," ");
            }
        mvwprintw(win,i,1,"%s",hero.liste_artefact[i-10].name);
    }
    
}


wrefresh(win);


}

int fight(hero_t hero,monstre_t monstre){/*
    int mort=0;

    while(mort==0){
        if(monstre.attackspeed>hero.attackspeed){
            hero.health=hero.health-monstre.strength;
            if(hero.health<=0){ 
                mort=0;
                return 1;
            }
        }
        else{
             monstre.health=monstre.health-monstre.strength;
               if(hero.health<=0){
                mort=0;
                return 2;

            }
        }
    }*/
    return 0;
    

}