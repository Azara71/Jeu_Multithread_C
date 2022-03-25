#include <locale.h>     // Pour setlocal
#include <stdlib.h>     // Pour EXIT_SUCCESS
#include <ncurses.h>    // Pour les fonctions/constantes ncurses
#include "fonction_ncurses.h"  // Les fonctions personnalisées
#include "monstre.h"
#include "artefact.h"
#include "carte.h"
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "enregistrement.h"
#include <string.h>

#define BLUE 2
#define YELLOW 3
#define GREEN 4
#define RED 5
#define NBMAXMOB 20
#define TAILLENOMMONSTRE 1024
#define NBMAXARTEFACT 20
/*
*FONCTION DE RECHERCHE DU PREMIER INDICE DISPO
*/
int premiere_place_libre_monstre(monstre_t tab[20]){
int i=0;
int trouve=0;
while(i<20 && trouve!=1){
  if(strcmp(tab[i].name,"-1")==0){
    return i;
  }
  else{
    i++;
  }
}
return -1;
}
/*
*FONCTION DE RECHERCHE DU PREMIER INDICE DISPO
*/
int premiere_place_libre_artefact(artefact_t tab[20]){
int i=0;
int trouve=0;
while(i<20 && trouve!=1){
  if(strcmp(tab[i].name,"-1")==0){
    return i;
  }
  else{
    i++;
  }
}
return -1;
}


int main(int argc,char *argv[]) {
int fd;
int posX,posY;
carte_t carte_jeu;

/* GESTION ARGUMENT  ET OUVERTURE DE FICHIER*/
if(argv[1]==NULL){
    fprintf(stderr,"Erreur,aucune carte renseignée"); // SI AUCUN ARGUMENT DE PASSER, ALORS UNE ERREUR
    exit(EXIT_FAILURE);                               
}
char* path;
size_t sizepath;
sizepath=strlen(argv[1])+strlen("map/")+strlen(".crt");
path=malloc(sizepath);
strcat(path,"map/");
strcat(path,argv[1]);
strcat(path,".crt");


if((fd = open(path, O_RDWR, S_IRUSR|S_IWUSR)) == -1){// SI NOUS N'ARRIVONS PAS A OUVRIR CAR :
    if(errno==ENOENT){        // LE FICHIER N'EXISTE PAS, ALORS ON CREE LA CARTE
      carte_jeu=charger_carte_default();
      }
    else{                       // CAR AUTRE PROBLEME, ON RENVOI UNE ERREUR
      perror("Ouverture de carte ratée");
      exit(EXIT_FAILURE);
    }
}
else {                    // SI AUCUN SOUCIS d'ARGUMENT, ON CHARGE LA CARTE GRACE AU NOM DU FICHIER
printf("\nCHARGEMENT DE :%s\n",path); // SI elle existe
carte_jeu=charger_carte(path);
}

  // Initialisation de ncurses
  setlocale(LC_ALL, "");
  ncurses_init();
  ncurses_couleurs();
  palette();
  // PERMET D'AVOIR UN FOND UNI
  bkgd(COLOR_PAIR(1));
  refresh();

  // Init souris
  ncurses_initsouris();
  // CREATION DES FENETRES
  WINDOW *fenetre_informations=creer_fenetre_informations();
  wrefresh(fenetre_informations);
  WINDOW *fenetre_outils=creer_fenetre_outils();
  WINDOW *fenetre_carte=creer_fenetre_carte();
  WINDOW *fenetre_creation_monstre=creer_fenetre_creation_monstre();


refresh_fenetre(fenetre_outils,fenetre_carte,fenetre_creation_monstre);
// LA SELECTION DE COULEUR DE BASE EST VERTE, LE CARACTERE EST UN ESPACE
int selected_color=GREEN;
char selected_char='s';
afficher_carte(fenetre_carte,carte_jeu);
// ON AFFICHAGE LA CARTE



int ch=getch();
  

while(ch!='q'){// ON MET EN ATTENTE D'UN CLIQUE TANT QU'IL EST DIFFERENT DE Q

if(ch==KEY_MOUSE){
// SI ON CLIQUE DANS LA FENETRE
int cooX=0,cooY=0;
souris_getpos(&posX, &posY);
cooX=posX;
cooY=posY;
wmouse_trafo(fenetre_carte,&posY,&posX,FALSE);
if(cooX>=1 && cooX<=40){ // Si c'est bien dans les cases
  
  if(selected_char=='c'){/* Si c'est un changement de couleur simple à faire */
    wattron(fenetre_carte,COLOR_PAIR(selected_color));

    carte_jeu.cases[posX-1][posY-1].code_couleur=selected_color; /* On change la couleur de la case*/
    if(carte_jeu.cases[posX-1][posY-1].elem!='s'){/*Si c'est pas un espace on print l'élément, sinon on print l'espace*/
        mvwprintw(fenetre_carte,posY,posX,&carte_jeu.cases[posX-1][posY-1].elem);
    }
    else{
      mvwprintw(fenetre_carte,posY,posX," ");
    }
    wattroff(fenetre_carte,COLOR_PAIR(selected_color));

  }
else{ // Si ce n'est pas un changement de couleur simple
    wattron(fenetre_carte,COLOR_PAIR(carte_jeu.cases[posX-1][posY-1].code_couleur)); //On garde la couleur de la case

  if(selected_char=='s'){ /* S= GOMME,Suppresion de l'identité dessus */
           wattron(fenetre_carte,COLOR_PAIR(GREEN));
           mvwprintw(fenetre_carte,posY,posX," ");
             //Si l'élément sur lequel on clique est un monstre, on repasse dans le tableau de monstre, à son indice, un monstre par défault, on remet son indice de monstre à -1, son élément à s.
            
             switch(carte_jeu.cases[posX-1][posY-1].elem){
               case 'M':// Si c'est un M, on y remet un monstre par default, on passe l'indice à -1, l'élément à s, le code la case on le garde.
                carte_jeu.monstre[carte_jeu.cases[posX-1][posY-1].indelem]=create_monster("-1",0,0,0,0,0);
                carte_jeu.cases[posX-1][posY-1].indelem=-1;
                carte_jeu.cases[posX-1][posY-1].elem='s';
                carte_jeu.cases[posX-1][posY-1].code_couleur=selected_color;
                carte_jeu.nbmob--;
               break;
               case 'A':
               carte_jeu.artefacts[carte_jeu.cases[posX-1][posY-1].indelem]=create_artefact("-1",0,0,0,0,0);
               carte_jeu.cases[posX-1][posY-1].indelem=-1;
               carte_jeu.cases[posX-1][posY-1].elem='s';
               carte_jeu.cases[posX-1][posY-1].code_couleur=selected_color;
               carte_jeu.nbartefacts--;
               break;
               default:
               carte_jeu.cases[posX-1][posY-1].elem='s';
               carte_jeu.cases[posX-1][posY-1].code_couleur=selected_color;
               break;
             }
              wattroff(fenetre_carte,COLOR_PAIR(GREEN));
}
  if(selected_char=='$'){
       if(carte_jeu.cases[posX-1][posY-1].elem =='$' ||carte_jeu.cases[posX-1][posY-1].elem =='X' || carte_jeu.cases[posX-1][posY-1].elem =='s'){// Si on ne va pas remplacer un M ou un A
           mvwprintw(fenetre_carte,posY,posX,"$");
           carte_jeu.cases[posX-1][posY-1].elem='$';
       }
       else{
       /* Si la case est déjà pris par un monstre ou un artefact */
         wprintw(fenetre_informations,"CASE OCCUPEE\n");
         wrefresh(fenetre_informations);
       }
  }
  if(selected_char=='A'){
    if(carte_jeu.nbartefacts<NBMAXARTEFACT){// Si il reste de la place dans le tableau
     if(carte_jeu.cases[posX-1][posY-1].elem =='$' ||carte_jeu.cases[posX-1][posY-1].elem =='X' || carte_jeu.cases[posX-1][posY-1].elem =='s'){ // Et qu'on ne clique pas déjà sur un M ou un A
          int indiceplacementartefact=premiere_place_libre_artefact(carte_jeu.artefacts); // La place disponible
          mvwprintw(fenetre_carte,posY,posX,"A");
             carte_jeu.cases[posX-1][posY-1].elem='A'; //Element = Artefact, la case pointera sur la première place libre dans le tableau d'artefact.
             carte_jeu.cases[posX-1][posY-1].indelem=indiceplacementartefact;
             carte_jeu.artefacts[indiceplacementartefact]=creer_artefact(fenetre_creation_monstre,fenetre_informations);
             carte_jeu.nbartefacts++;
     }
     else{
         /* Si la case est déjà pris par un monstre ou un artefact */
         wprintw(fenetre_informations,"CASE OCCUPEE\n");
         wrefresh(fenetre_informations);
      }       
  }
  }
  if(selected_char=='M'){
    /* Si la place est disponible */
    if(carte_jeu.nbmob<NBMAXMOB){
     if(carte_jeu.cases[posX-1][posY-1].elem =='$' ||carte_jeu.cases[posX-1][posY-1].elem =='X' || carte_jeu.cases[posX-1][posY-1].elem =='s'){
       int indiceplacementmonstre=premiere_place_libre_monstre(carte_jeu.monstre);
          mvwprintw(fenetre_carte,posY,posX,"M");
             carte_jeu.cases[posX-1][posY-1].elem='M';
             carte_jeu.cases[posX-1][posY-1].indelem=indiceplacementmonstre;
             carte_jeu.monstre[indiceplacementmonstre]=creer_monstre(fenetre_creation_monstre,fenetre_informations);
             carte_jeu.nbmob++;
      }
      else{
         /* Si la case est déjà pris par un monstre ou un artefact */
         wprintw(fenetre_informations,"CASE OCCUPEE\n");
         wrefresh(fenetre_informations);
      }       
    }
  
  }

  if(selected_char=='X'){
    if(carte_jeu.cases[posX-1][posY-1].elem =='$' ||carte_jeu.cases[posX-1][posY-1].elem =='X' || carte_jeu.cases[posX-1][posY-1].elem =='s'){
          mvwprintw(fenetre_carte,posY,posX,"X");
          carte_jeu.cases[posX-1][posY-1].elem='X';
    }
     else{
         /* Si la case est déjà pris par un monstre ou un artefact */
         wprintw(fenetre_informations,"CASE OCCUPEE\n");
         wrefresh(fenetre_informations);
      }     
  }

  if(selected_char=='i'){
  
    switch(carte_jeu.cases[posX-1][posY-1].elem){
      case 'M':
         wprintw(fenetre_informations,"INFORMATIONS SUR LE MONSTRE :\n");
         wprintw(fenetre_informations,"%d",carte_jeu.cases[posX-1][posY-1].indelem);
         afficher_stat_monstre(fenetre_informations,carte_jeu.monstre[carte_jeu.cases[posX-1][posY-1].indelem]); // Fonction affichant les attributs de chaque monstre demandé
         break;
      case 'A':
         wprintw(fenetre_informations,"INFORMATIONS SUR L'ARTEFACT' :\n");
         wprintw(fenetre_informations,"%d",carte_jeu.cases[posX-1][posY-1].indelem);
         afficher_stat_artefact(fenetre_informations,carte_jeu.artefacts[carte_jeu.cases[posX-1][posY-1].indelem]);// Fonction affichant les attributs de chaque monstre/artefact demandé
         break;
      default:
        wprintw(fenetre_informations,"CASE DE COULEUR %d\n",carte_jeu.cases[posX-1][posY-1].code_couleur);

    }
  }
  wattroff(fenetre_carte,COLOR_PAIR(carte_jeu.cases[posX-1][posY-1].code_couleur));

  }
}
wrefresh(fenetre_carte);


// SI ON CLIQUE DANS LA FENETRE OUTILS
wmouse_trafo(fenetre_outils,&cooY,&cooX,FALSE);
if(cooX==1){
      // SI LE CLICK EST DANS LA COLONNE NUMERO UNE 
if(cooY==2){
//BLEU
  selected_color=BLUE;
  selected_char='c';
}
if(cooY==3){
//YELLOW
   selected_color=YELLOW;
   selected_char='c';
}
if(cooY==4){
//VERT
selected_color=GREEN;
      selected_char='c';
}
if(cooY==5){
//ROUGE
selected_color=RED;
selected_char='c';

}
// SI CEST UN CARACTERE
if(cooY==7){
selected_char='$';
}
if(cooY==8){
selected_char='M';
}
if(cooY==9){
selected_char='A';
}
if(cooY==10){
selected_char='X';
}
if(cooY==12){
selected_char='i';
}
if(cooY==13){
selected_char='s';
selected_color=GREEN;
}
     wprintw(fenetre_informations,"\nCARACTERE CHOISIT :%c",selected_char);
     wprintw(fenetre_informations,"COULEUR CHOISIT %d",selected_color);
}

}


wrefresh(fenetre_informations);
ch=getch();
}

wprintw(fenetre_informations,"\nFIN DES OPERATIONS");
wrefresh(fenetre_informations);
ch=getch();



ncurses_stop();
enregistrer_carte(&carte_jeu,path);
printf("INFORMATION SUR LA CARTE ENREGISTRÉE :\n");
printf("ARTEFACTS :\n\n");
for(int i=0;i<20;i++){
  if(strcmp(carte_jeu.artefacts[i].name,"-1")!=0){
   printf("%s\n",carte_jeu.artefacts[i].name);
  }
}
printf("MONSTRES :\n\n");
for(int i=0;i<20;i++){
  if(strcmp(carte_jeu.monstre[i].name,"-1")!=0){
   printf("%s\n",carte_jeu.monstre[i].name);
  }

}


printf("NOMBRE DE MONSTRE TOTAL:%d\n",carte_jeu.nbmob);
printf("NOMBRE TOTAL D'ARTEFACT:%d\n",carte_jeu.nbartefacts);
printf("%s",path);
  return EXIT_SUCCESS;
}