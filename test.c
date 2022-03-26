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

liste_carte_t* l_carte=init_liste_carte();
liste_carte_t* l_carte_2=init_liste_carte();
node_t node;
node.x=4;
node.y=5;
node.emplacement_carte=2;
//printf("%d\n",l_carte->length);
inserer_liste(l_carte,node);
inserer_liste(l_carte,node);
inserer_liste(l_carte,node);
inserer_liste(l_carte,node);
inserer_liste(l_carte,node);
inserer_liste(l_carte,node);


inserer_liste(l_carte_2,node);
inserer_liste(l_carte_2,node);
inserer_liste(l_carte_2,node);
inserer_liste(l_carte_2,node);
inserer_liste(l_carte_2,node);
inserer_liste(l_carte_2,node);

l_carte->liste_suivante=l_carte_2;
afficher_liste_carte(l_carte);

}