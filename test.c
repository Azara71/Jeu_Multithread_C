/*####################################################################################################################################
PROGRAMME DU SERVEUR, IL PREND LE PORT TCP EN ENTREE, LE NOM DU REPETOIRE DE CARTE
####################################################################################################################################*/
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
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <unistd.h>
#include "enregistrement.h"
#include <dirent.h>
#include "hero.h"


int main(){

tabnodes_t* l_carte=init_tabnodes_t();

node_t node;
node.x=4;
node.y=5;
node.emplacement_carte=2;

   
for(int i=0;i<20;i++){
    inserer_tab_nodes(l_carte,node);
}      
                int  world_descriptor=open("monde.sav",O_CREAT|O_RDWR,S_IRUSR|S_IWUSR);
                if(lseek(world_descriptor,0,SEEK_SET)==-1){
                     perror("Déplacement fichier ");                         /*  FILE  */
                     exit(EXIT_FAILURE);
                }
                ecrire_tab_nodes_dans_fichier(l_carte,world_descriptor,0);
                
}