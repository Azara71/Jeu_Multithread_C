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
tabnodes_t* l_carte_lu=init_tabnodes_t();

node_t node;
node.x=4;
node.y=5;
node.emplacement_carte=2;

node_t nodetest;
nodetest.x=20;
nodetest.y=3;
nodetest.emplacement_carte=4;



for(int i=0;i<17;i++){
    inserer_tab_nodes(l_carte,node);
}

int fd;
if((fd=open("LALALAL",O_CREAT|O_RDWR,S_IRUSR|S_IWUSR))==-1){
         perror("Erreur ouverture fichier 1");
         exit(EXIT_FAILURE);
}

//afficher_tabnodes_t(l_carte);
ecrire_tab_nodes_dans_fichier(l_carte,fd,0);
l_carte_lu=lire_tab_nodes_dans_fichier(fd,0);
remplacer(l_carte_lu,nodetest,10);
afficher_tabnodes_t(l_carte_lu);

}