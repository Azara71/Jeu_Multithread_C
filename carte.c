#include "carte.h"
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include "enregistrement.h"
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
liste_carte_t *inserer_liste(liste_carte_t *liste, int x,int y,carte_t carteainserer)
{
    if (liste != NULL)
    {
        carte_chainee_t *carte = malloc(sizeof *carte);
        if (carte != NULL)
        {
            carte->x = x;
            carte->y = y;
            carte->carte = carteainserer;
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
* 
*/
int chercher_map_from_list(liste_carte_t *liste, int x, int y,carte_t carte){
int found=0;
    if (liste != NULL)
    {
        carte_chainee_t *current = liste->tete;
        while (current != NULL && !found)
        {
            if (current->x==x && current->y==y)
            {
                found = 1;
                return found;
            }
            else
            {
                current = current->suivant;
            }
        }
    }
    return found;

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
        tabnode->suivant = 0;
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
            tabnode->suivant=1;


        }

    }
    return tabnode;
}
/*
* REMPLACE UN NODE DANS UN TABLEAU DE NODE 
*/
tabnodes_t *remplacer(tabnodes_t *tabnode, node_t node,int num_node)
{
    if(num_node<5){
        if (tabnode != NULL)
        {
         tabnode->nodes[num_node]=node;
        }
    }
    else{
      num_node=num_node-5;
      remplacer(tabnode->tab_suivant,node,num_node);
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

off_t trouver_emplacement_par_tabnodes(tabnodes_t *tabnode,int x,int y){
    off_t pos_to_return=0;

     if (tabnode != NULL)
    {
        for(int i=0;i<5;i++){
           if(tabnode->nodes[i].x==x && tabnode->nodes[i].y==y){
               pos_to_return=tabnode->nodes[i].emplacement_carte;
               return pos_to_return;
           }

        }
        pos_to_return=trouver_emplacement_par_tabnodes(tabnode->tab_suivant,x,y);
}

return pos_to_return;
}

int dans_tabnodes(tabnodes_t *tabnode,int x,int y){
int found=0;
  if (tabnode != NULL)
    {
        for(int i=0;i<5;i++){
           if(tabnode->nodes[i].x==x && tabnode->nodes[i].y==y){
               found=1;
               return found;
           }

        }
        found=trouver_emplacement_par_tabnodes(tabnode->tab_suivant,x,y);
    }

return found;
}




void ecrire_node(node_t node,int fd){

if(write(fd,&node.x,sizeof(int))==-1){
       perror("Erreur ecriture fichier 1");                         /*  FILE  */
          exit(EXIT_FAILURE);
}
if(write(fd,&node.y,sizeof(int))==-1){
       perror("Erreur ecriture fichier 1");                         /*  FILE  */
          exit(EXIT_FAILURE);
}
if(write(fd,&node.emplacement_carte,sizeof(off_t))==-1){
       perror("Erreur ecriture fichier 1");                         /*  FILE  */
          exit(EXIT_FAILURE);
}

}

node_t lire_node(int fd){
node_t node;
if(read(fd,&node.x,sizeof(int))==-1){
       perror("Erreur ecriture fichier 1");                         /*  FILE  */
          exit(EXIT_FAILURE);
}
if(read(fd,&node.y,sizeof(int))==-1){
       perror("Erreur ecriture fichier 1");                         /*  FILE  */
          exit(EXIT_FAILURE);
}
if(read(fd,&node.emplacement_carte,sizeof(off_t))==-1){
       perror("Erreur ecriture fichier 1");                         /*  FILE  */
          exit(EXIT_FAILURE);
}
return node;
}


void ecrire_tab_nodes_dans_fichier(tabnodes_t *tabnode,int fd,int k){

if(tabnode!=NULL) {
if(write(fd,&tabnode->length,sizeof(int))==-1){
       perror("Erreur ecriture length");                         /*  FILE  */
          exit(EXIT_FAILURE);
}
for(int i=0;i<5;i++){
    ecrire_node(tabnode->nodes[i],fd);
}
if(write(fd,&tabnode->mon_endroit,sizeof(off_t))==-1){
       perror("Erreur ecriture fichier 1");                         /*  FILE  */
          exit(EXIT_FAILURE);
}
if(write(fd,&tabnode->suivant,sizeof(int))==-1){
       perror("Erreur ecriture fichier 1");                         /*  FILE  */
          exit(EXIT_FAILURE);
}

ecrire_tab_nodes_dans_fichier(tabnode->tab_suivant,fd,k);
}


}


tabnodes_t* lire_tab_nodes_dans_fichier(int fd){
tabnodes_t* tabnode=init_tabnodes_t();

if(read(fd,&tabnode->length,sizeof(int))==-1){
       perror("Erreur ecriture fichier 1");                         /*  FILE  */
          exit(EXIT_FAILURE);
};
for(int i=0;i<5;i++){
    tabnode->nodes[i]=lire_node(fd);
}
if(read(fd,&tabnode->mon_endroit,sizeof(off_t))==-1){
       perror("Erreur ecriture fichier 1");                         /*  FILE  */
          exit(EXIT_FAILURE);
}
if(read(fd,&tabnode->suivant,sizeof(int))==-1){
       perror("Erreur ecriture fichier 1");                         /*  FILE  */
          exit(EXIT_FAILURE);
}
if(tabnode->suivant==1){
    tabnode->tab_suivant=lire_tab_nodes_dans_fichier(fd);
}
return tabnode;

}

void rajouter_carte_monde_sav(int world_descriptor,tabnodes_t* l_carte,carte_t carte_a_envoyer){

        off_t k;
        if((k=lseek(world_descriptor,0,SEEK_END))==-1){
             perror("Déplacement fichier ");                         /*  FILE  */
             exit(EXIT_FAILURE);
        }
        node_t node;
        node.x=0;
        node.y=0;
        node.emplacement_carte=k;
        l_carte=remplacer(l_carte,node,0);
        enregistrer_new_sav_carte(&carte_a_envoyer,world_descriptor);
                    if((k=lseek(world_descriptor,0,SEEK_SET))==-1){
                        perror("Déplacement fichier ");                         /*  FILE  */
                        exit(EXIT_FAILURE);
                    }
                    ecrire_tab_nodes_dans_fichier(l_carte,world_descriptor,0);

                   afficher_tabnodes_t(l_carte);

}
