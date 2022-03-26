#include "carte.h"
#include <stdlib.h>
#include <string.h>
void afficher_carte(WINDOW* fen_affichage,carte_t carte){
// AFFICHAGE D'UNE CARTE AU COMPLET
for(int i=0;i<40;i++){
    for(int j=0;j<20;j++){
         wattron(fen_affichage,COLOR_PAIR(carte.cases[i][j].code_couleur));
                       
            if(carte.cases[i][j].elem=='s'){
              mvwprintw(fen_affichage,j+1,i+1," ");
            }
            else{
               
               mvwprintw(fen_affichage,j+1,i+1,&carte.cases[i][j].elem);
            }
           wattroff(fen_affichage,COLOR_PAIR(carte.cases[i][j].code_couleur));


    }
}
wrefresh(fen_affichage);
}

liste_carte_t *init_liste_carte(){
    liste_carte_t *liste = malloc(sizeof *liste);
    if (liste != NULL)
    {
        liste->length = 0;
        liste->liste_suivante = NULL;
        liste->endroit_liste_suivante = 0;
    }
    return liste;
}

liste_carte_t *inserer_liste(liste_carte_t *liste, node_t node)
{
    
    if (liste != NULL)
    {
        if(liste->length>=5){
            inserer_liste(liste->liste_suivante,node);
        }
        else{
            liste->nodes[liste->length]=node;
            liste->length++;

        }

    }
    return liste;
}


void afficher_liste_carte(liste_carte_t *liste)
{
   printf("\n");

    if (liste != NULL)
    {
        printf("|________|\n");
        for(int i=0;i<5;i++){
            printf("(%d,%d) : %ld\n",liste->nodes[i].x,liste->nodes[i].y,liste->nodes[i].emplacement_carte);
        }
        afficher_liste_carte(liste->liste_suivante);

    }

   
}
