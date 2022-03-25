/*####################################################################################################################################
PROGRAMME DU CLIENT,IL PREND LE PORT DU SERVEUR EN PARAMETRE ET UNE IP
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
#include <signal.h>
#include <stdio.h>



#define CARTE 1
#define THREAD_STOP 2
#define BOTTOM 258 
#define TOP 259
#define LEFT 260
#define RIGHT 261

carte_t carte_jeu;
pthread_mutex_t mutex= PTHREAD_MUTEX_INITIALIZER;



typedef struct data_to_thread_affichage{

int fd;
WINDOW* fenetre_carte;
WINDOW *fenetre_informations;
WINDOW *fenetre_stat;

}data_to_thread_affichage_t;

int stop_affichage=0;
void handler(int signum) {

    if(signum == SIGINT) {
        stop_affichage = 1;
    }
    
   
}

void* thread_deplacement(void *arg){
int ch;
int left=LEFT;
int right=RIGHT;
int top=TOP;
int bottom=BOTTOM;

data_to_thread_affichage_t* data_to_thread_affichage=(data_to_thread_affichage_t*) arg;

while(stop_affichage==0){

ch=getch();
switch(ch){
    case LEFT:
    if(write(data_to_thread_affichage->fd,&left,sizeof(int))== -1) {
        perror("Erreur lors de l'envoi de la valeur ");
        exit(EXIT_FAILURE);
    }
    break;
    case RIGHT:
     if(write(data_to_thread_affichage->fd,&right,sizeof(int))== -1) {
        perror("Erreur lors de l'envoi de la valeur ");
        exit(EXIT_FAILURE);
     }
     break;
     case BOTTOM:
     if(write(data_to_thread_affichage->fd,&bottom,sizeof(int))== -1) {
        perror("Erreur lors de l'envoi de la valeur ");
        exit(EXIT_FAILURE);
     }
     break;
     case TOP:
     if(write(data_to_thread_affichage->fd,&top,sizeof(int))== -1) {
        perror("Erreur lors de l'envoi de la valeur ");
        exit(EXIT_FAILURE);
     }
    break;
wprintw(data_to_thread_affichage->fenetre_informations,"%d\n",ch);
wrefresh(data_to_thread_affichage->fenetre_informations);
}
//sleep(1);

}

return NULL;
}





/*
Routine du thread d'affichage
*/
void* thread_affichage(void *arg){
data_to_thread_affichage_t* data_to_thread_affichage=(data_to_thread_affichage_t*) arg;
int demande_carte=CARTE;
int demande_deco=THREAD_STOP;
int totallus,lus=0;


while(stop_affichage==0){

if(write(data_to_thread_affichage->fd,&demande_carte,sizeof(int))== -1) {
        perror("Erreur lors de l'envoi de la valeur ");
        exit(EXIT_FAILURE);
}

while(totallus != sizeof(carte_t)){
    if((lus=read(data_to_thread_affichage->fd,&carte_jeu,sizeof(carte_t)-totallus))==-1){
        perror("Erreur lecture de carte");
        exit(EXIT_FAILURE);
    }
    totallus+=lus;
}
totallus=0;
    

afficher_carte(data_to_thread_affichage->fenetre_carte,carte_jeu);

}
   
printf("Deconnexion...");

if(write(data_to_thread_affichage->fd,&demande_deco,sizeof(int))== -1) {
        perror("Erreur lors de l'envoi de la valeur ");
        exit(EXIT_FAILURE);
}
do{
if(read(data_to_thread_affichage->fd,&demande_deco,sizeof(int))== -1) {
        perror("Erreur lors de l'envoi de la valeur ");
        exit(EXIT_FAILURE);
}
}
while(demande_deco!=2);






// Fermeture de la socket
if(close(data_to_thread_affichage->fd) == -1) {
    perror("Erreur lors de la fermeture de la socket ");
    exit(EXIT_FAILURE);
}


return NULL;


}


int main(int argc,char* argv[]){
int fd;
struct sockaddr_in adresse;
data_to_thread_affichage_t data_to_thread_affichage;
pthread_t id_thread_affichage;
pthread_t id_thread_deplacement;
struct sigaction action;

// Positionnement du gestionnaire
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;
    action.sa_handler = handler;
    if(sigaction(SIGCHLD, &action, NULL) == -1) {
        perror("Erreur lors du placement du gestionnaire ");
        exit(EXIT_FAILURE);    
    }
    if(sigaction(SIGINT, &action, NULL) == -1) {
        perror("Erreur lors du placement du gestionnaire ");
        exit(EXIT_FAILURE);    
    }
/*
* Verification argument
*/
if(argc!=3){
    fprintf(stderr,"Pas assez d'argument");
    exit(EXIT_FAILURE);
}

  // Création de la socket
    if((fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1) {
        perror("Erreur lors de la création de la socket ");
        exit(EXIT_FAILURE);
    }
    
    // Remplissage de la structure
    memset(&adresse, 0, sizeof(struct sockaddr_in));
    adresse.sin_family = AF_INET;
    adresse.sin_port = htons(atoi(argv[2]));
    if(inet_pton(AF_INET, argv[1], &adresse.sin_addr.s_addr) != 1) {
        perror("Erreur lors de la conversion de l'adresse ");
        exit(EXIT_FAILURE);
    }

    // Connexion au serveur
    if(connect(fd, (struct sockaddr*)&adresse, sizeof(adresse)) == -1) {
        perror("Erreur lors de la connexion ");
        exit(EXIT_FAILURE);
    }
  setlocale(LC_ALL, "");
  ncurses_init();
  ncurses_couleurs();
  palette();
  // PERMET D'AVOIR UN FOND UNI
  bkgd(COLOR_PAIR(1));
  refresh();
  ncurses_initsouris();
 
 data_to_thread_affichage.fenetre_carte=creer_fenetre_carte();
 data_to_thread_affichage.fenetre_informations=creer_fenetre_informations();
 data_to_thread_affichage.fenetre_stat=creer_fenetre_stat();

 wrefresh(data_to_thread_affichage.fenetre_carte);
 wrefresh(data_to_thread_affichage.fenetre_informations);
 wrefresh(data_to_thread_affichage.fenetre_stat);



    data_to_thread_affichage.fd=fd;
    pthread_create(&id_thread_affichage,NULL,thread_affichage,&data_to_thread_affichage);
    pthread_create(&id_thread_deplacement,NULL,thread_deplacement,&data_to_thread_affichage);





pthread_join(id_thread_affichage,NULL);
pthread_join(id_thread_deplacement,NULL);


ncurses_stop();


 
exit(EXIT_SUCCESS);



}