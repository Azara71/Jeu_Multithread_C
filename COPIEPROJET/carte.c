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
        liste->tete = NULL;
        liste->queue = NULL;
    }
    return liste;
}

liste_carte_t *inserer_liste(liste_carte_t *liste, int x,int y, char* nom_map)
{
    if (liste != NULL)
    {
        carte_chainee_t *carte = malloc(sizeof *carte);
        carte->nom_carte=malloc(strlen(nom_map)+1);
        if (carte != NULL)
        {
            carte->x = x;
            carte->y = y;
            strcpy(carte->nom_carte,nom_map);
            if (liste->queue == NULL)
            {
                carte->suivant = NULL;
                liste->tete = carte;
                liste->queue = carte;
            }
            else
            {
                liste->tete->prec = carte;
                carte->suivant = liste->tete;
                liste->tete = carte;
            }
            liste->length++;
       }
    }
    return liste;
}

liste_carte_t *remove_map_from_list(liste_carte_t *liste,char* nom_map)
{
    if (liste != NULL)
    {
        carte_chainee_t *current = liste->tete;
        int found = 0;
        while (current != NULL && !found)
        {
            if (strcmp(current->nom_carte, nom_map)==0)
            {
                if (current->suivant == NULL)
                {
                    liste->queue = current->prec;
                    liste->queue->suivant = NULL;
                }
                else if (current->prec == NULL)
                {
                    liste->tete = current->suivant;
                    liste->tete->prec = NULL;
                }
                else
                {
                    current->suivant->prec = current->prec;
                    current->prec->suivant = current->suivant;
                }
                free(current);
                liste->length--;
                found = 1;
            }
            else
            {
                current = current->suivant;
            }
        }
    }
    return liste;
}

void afficher_liste_carte(liste_carte_t *liste)
{
   printf("\n");
    if (liste != NULL)
    {
        carte_chainee_t *current = liste->tete;
        while (current != NULL)
        {
            printf("%s:(%d,%d) -> ",current->nom_carte,current->x,current->y);
            fflush(stdout);
            current = current->suivant;
        }
    }
    printf("NULL\n");
}


