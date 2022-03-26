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

/*
* INITIALISE UNE LISTE DE CARTE
*/
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
/*
* INSERER UNE MAP SITUÉ DANS UNE LISTE
*/
liste_carte_t *inserer_liste(liste_carte_t *liste, int x,int y)
{
    if (liste != NULL)
    {
        carte_chainee_t *carte = malloc(sizeof *carte);
        if (carte != NULL)
        {
            carte->x = x;
            carte->y = y;
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
/*
* DETRUIRE UNE MAP SITUÉ DANS UNE LISTE
*/
liste_carte_t *remove_map_from_list(liste_carte_t *liste,int x,int y)
{
    if (liste != NULL)
    {
        carte_chainee_t *current = liste->tete;
        int found = 0;
        while (current != NULL && !found)
        {
            if (current->x==x && current->y==y)
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
/*
* AFFICHAGE D'UNE LISTE CHAINÉE DE CARTE
*/
void afficher_liste_carte(liste_carte_t *liste)
{
   printf("\n");
    if (liste != NULL)
    {
        carte_chainee_t *current = liste->tete;
        while (current != NULL)
        {
            printf("(%d,%d) -> ",current->x,current->y);
            fflush(stdout);
            current = current->suivant;
        }
    }
    printf("NULL\n");
}



/*
* INITIALISE UN TABLEAU DE NODES
*/
tabnodes_t *init_tabnodes_t(){
    tabnodes_t *tabnode = malloc(sizeof *tabnode);
    if (tabnode != NULL)
    {
        tabnode->length = 0;
        tabnode->tab_suivant = NULL;
        tabnode->endroit_liste_suivante = 0;
    }
    return tabnode;
}
/*
* INSERE UN NODE DANS UN TABLEAU
*/
tabnodes_t *inserer_tab_nodes(tabnodes_t *tabnode, node_t node)
{
    
    if (tabnode != NULL)
    {
        if(tabnode->length<5){
            tabnode->nodes[tabnode->length]=node;
            tabnode->length++; 
        }
        else{
             if(tabnode->tab_suivant==NULL){
                tabnodes_t* new_tabnodes_suivant=init_tabnodes_t();
                tabnode->tab_suivant=new_tabnodes_suivant;
                inserer_tab_nodes(tabnode->tab_suivant,node);
            }else{
            inserer_tab_nodes(tabnode->tab_suivant,node);

            }

        }

    }
    return tabnode;
}
/*
* AFFICHAGE UN TABLEAU DE NODES ET CHACUN DE SES TABLEAUX SUIVANTS
*/
void afficher_tabnodes_t(tabnodes_t *tabnode){
    printf("\n");

    if (tabnode != NULL)
    {
        printf("|________|\n");
        for(int i=0;i<5;i++){
            printf("(%d,%d) : %ld\n",tabnode->nodes[i].x,tabnode->nodes[i].y,tabnode->nodes[i].emplacement_carte);
        }
        afficher_tabnodes_t(tabnode->tab_suivant);
}
}

