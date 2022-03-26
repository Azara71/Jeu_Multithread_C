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
#define CARTE 1
#define THREAD_STOP 2
#define BOTTOM 258 
#define TOP 259
#define LEFT 260
#define RIGHT 261

/*
* Structure gérant les informations à passer au thread gérant le client à sa création
*/
typedef struct info_client{
    int num_client;
    int sockclient;
    char* nom_repertoire;
    DIR* directory;
    int world_descriptor;
    hero_t hero;
}info_client_t;

int generer_nombre_aleatoire(int nb_max){
    return rand()%nb_max;
}

/*
* Renvoi le nombre de carte dans une map
*/

int compter_nombre_carte(DIR* directory){
struct dirent* info_directory;
int nb_carte_dans_repertoire=0;
while((info_directory=readdir(directory))!=NULL){
     if (strstr(info_directory->d_name, ".crt") != NULL) {
        nb_carte_dans_repertoire++;
        printf("%d\n",nb_carte_dans_repertoire);
     }   
}
return nb_carte_dans_repertoire;
}

void ramasser_piece_grand_tout(hero_t hero){

    if(hero.nb_piece_grand_tout<3){
        hero.nb_piece_grand_tout++;
    }

}

/*
* Routine du thread gérant les clients du côté serveur.
*/
pthread_mutex_t mutex= PTHREAD_MUTEX_INITIALIZER;
carte_t carte_a_envoyer;
int stop=0;


void *thread_client(void *arg){ 

info_client_t* info_client =(info_client_t*) arg; 
int demande_client_to_server;
int confirmation_eteinte=2;
int stop_thread=0;
// Message d'indication du client
printf("Connexion du client numero %d\n",info_client->num_client); 
if(info_client->num_client==0){

pthread_mutex_lock(&mutex);
carte_a_envoyer=charger_carte_monde_sav("monde.sav",info_client->world_descriptor,0);
pthread_mutex_unlock(&mutex);

}


info_client->hero=create_hero("H");
pthread_mutex_lock(&mutex);
carte_a_envoyer.cases[info_client->hero.cooX][info_client->hero.cooY].elem='H';
pthread_mutex_unlock(&mutex);
/*
* On lit ce que nous a envoyé le client
*/
while(stop_thread==0){
 if(read(info_client->sockclient, &demande_client_to_server, sizeof(int)) == -1) {
        perror("Erreur lors de la lecture de la valeur ");
        exit(EXIT_FAILURE);
 }
  switch(demande_client_to_server){
      /*
      * Demande de la carte.
      */
      /*
      * VARIABLE DE CONDITION SUR LE THREAD QUI ENVOI TOUT LES X TEMPS
      */
      case CARTE: 
      pthread_mutex_lock(&mutex);
      if(write(info_client->sockclient, &carte_a_envoyer, sizeof(carte_a_envoyer)) == -1) {
            perror("Erreur lors de l'envoi de la valeur ");
            exit(EXIT_FAILURE);
      }
      pthread_mutex_unlock(&mutex);
      break;
      /*
      * Demande de l'arrêt du client.
      */
      case THREAD_STOP:
      stop_thread=1;

      if(write(info_client->sockclient,&confirmation_eteinte, sizeof(int)) == -1) {
            perror("Erreur lors de l'envoi de la valeur ");
            exit(EXIT_FAILURE);
      }
      if(close(info_client->sockclient) == -1) {
            perror("Erreur lors de la fermeture de la socket de communication ");
            exit(EXIT_FAILURE);
        }
      break;

      /*
      * Mouvement haut, gauche, droite, bas.
      */
      case TOP:
      pthread_mutex_lock(&mutex);
      if(info_client->hero.cooY-1>=0){
         if(carte_a_envoyer.cases[info_client->hero.cooX][info_client->hero.cooY-1].code_couleur!=2 && carte_a_envoyer.cases[info_client->hero.cooX][info_client->hero.cooY-1].elem!='X'){
                if(carte_a_envoyer.cases[info_client->hero.cooX][info_client->hero.cooY-1].elem=='$'){
                        if(generer_nombre_aleatoire(2)==1){
                            printf("Heal de %d",info_client->num_client);
                            info_client->hero.health=info_client->hero.health_max;
                        }
                        else{
                            ramasser_piece_grand_tout(info_client->hero);
                        }    
                }
                if(carte_a_envoyer.cases[info_client->hero.cooX][info_client->hero.cooY-1].elem=='A'){
                    int  id_cartefact_a_ramasser=carte_a_envoyer.cases[info_client->hero.cooX][info_client->hero.cooY-1].indelem;
                    int premiere_place_libre=premiere_place_libre_inventaire_artefact(info_client->hero);
                    info_client->hero=ramasser_artefact(info_client->hero,carte_a_envoyer.artefacts[id_cartefact_a_ramasser],premiere_place_libre);
                    carte_a_envoyer.artefacts[id_cartefact_a_ramasser]=create_artefact("-1",0,0,0,0,0);
                    carte_a_envoyer.cases[info_client->hero.cooX][info_client->hero.cooY-1].indelem=-1;
                }



                carte_a_envoyer.cases[info_client->hero.cooX][info_client->hero.cooY].elem=' ';
                info_client->hero.cooY=info_client->hero.cooY-1;
                carte_a_envoyer.cases[info_client->hero.cooX][info_client->hero.cooY].elem='H';
            }
      }
      else{
        printf("Génération vers le haut\n");
      }
      pthread_mutex_unlock(&mutex);
      break;

      case LEFT:
      pthread_mutex_lock(&mutex);
      if(info_client->hero.cooX-1>=0){
            if(carte_a_envoyer.cases[info_client->hero.cooX-1][info_client->hero.cooY].code_couleur!=2 && carte_a_envoyer.cases[info_client->hero.cooX-1][info_client->hero.cooY].elem!='X'){
                if(carte_a_envoyer.cases[info_client->hero.cooX-1][info_client->hero.cooY].elem=='$'){
                        if(generer_nombre_aleatoire(2)==1){
                            printf("Heal de %d",info_client->num_client);
                            info_client->hero.health=info_client->hero.health_max;
                        }
                        else{
                            ramasser_piece_grand_tout(info_client->hero);
                        }    
                }
                if(carte_a_envoyer.cases[info_client->hero.cooX-1][info_client->hero.cooY].elem=='A'){
                    int  id_cartefact_a_ramasser=carte_a_envoyer.cases[info_client->hero.cooX-1][info_client->hero.cooY].indelem;
                    int premiere_place_libre=premiere_place_libre_inventaire_artefact(info_client->hero);
                    info_client->hero=ramasser_artefact(info_client->hero,carte_a_envoyer.artefacts[id_cartefact_a_ramasser],premiere_place_libre);
                    carte_a_envoyer.artefacts[id_cartefact_a_ramasser]=create_artefact("-1",0,0,0,0,0);
                    carte_a_envoyer.cases[info_client->hero.cooX-1][info_client->hero.cooY].indelem=-1;
                }



                carte_a_envoyer.cases[info_client->hero.cooX][info_client->hero.cooY].elem=' ';
                info_client->hero.cooX=info_client->hero.cooX-1;
                carte_a_envoyer.cases[info_client->hero.cooX][info_client->hero.cooY].elem='H';
            }
      }
      else{
        printf("Génération vers la gauche\n");
      }
      pthread_mutex_unlock(&mutex);
      break;
      
      case RIGHT:
      pthread_mutex_lock(&mutex);
        if(info_client->hero.cooX+1<40){
            if(carte_a_envoyer.cases[info_client->hero.cooX+1][info_client->hero.cooY].code_couleur!=2 && carte_a_envoyer.cases[info_client->hero.cooX+1][info_client->hero.cooY].elem!='X'){
                if(carte_a_envoyer.cases[info_client->hero.cooX+1][info_client->hero.cooY].elem=='$'){
                        if(generer_nombre_aleatoire(2)==1){
                            printf("Heal de %d",info_client->num_client);
                            info_client->hero.health=info_client->hero.health_max;
                        }
                        else{
                            ramasser_piece_grand_tout(info_client->hero);
                        }    
                }
                if(carte_a_envoyer.cases[info_client->hero.cooX+1][info_client->hero.cooY].elem=='A'){

                    int  id_cartefact_a_ramasser=carte_a_envoyer.cases[info_client->hero.cooX+1][info_client->hero.cooY].indelem;
                    int premiere_place_libre=premiere_place_libre_inventaire_artefact(info_client->hero);
                    info_client->hero=ramasser_artefact(info_client->hero,carte_a_envoyer.artefacts[id_cartefact_a_ramasser],premiere_place_libre);
                    carte_a_envoyer.artefacts[id_cartefact_a_ramasser]=create_artefact("-1",0,0,0,0,0);
                    carte_a_envoyer.cases[info_client->hero.cooX+1][info_client->hero.cooY].indelem=-1;

                } 
                carte_a_envoyer.cases[info_client->hero.cooX][info_client->hero.cooY].elem=' ';
                info_client->hero.cooX=info_client->hero.cooX+1;
                carte_a_envoyer.cases[info_client->hero.cooX][info_client->hero.cooY].elem='H';
            }
      }
      else{
        printf("Génération vers la droite\n");
      }
      pthread_mutex_unlock(&mutex);
      break;

      case BOTTOM:
      pthread_mutex_lock(&mutex);
      if(info_client->hero.cooY+1<20){
           if(carte_a_envoyer.cases[info_client->hero.cooX][info_client->hero.cooY+1].code_couleur!=2 && carte_a_envoyer.cases[info_client->hero.cooX][info_client->hero.cooY+1].elem!='X'){
                if(carte_a_envoyer.cases[info_client->hero.cooX][info_client->hero.cooY+1].elem=='$'){
                        if(generer_nombre_aleatoire(2)==1){
                            printf("Heal de %d",info_client->num_client);
                            info_client->hero.health=info_client->hero.health_max;
                        }
                        else{
                            ramasser_piece_grand_tout(info_client->hero);
                        }    
                }
                if(carte_a_envoyer.cases[info_client->hero.cooX][info_client->hero.cooY+1].elem=='A'){

                    int  id_cartefact_a_ramasser=carte_a_envoyer.cases[info_client->hero.cooX][info_client->hero.cooY+1].indelem;
                    int premiere_place_libre=premiere_place_libre_inventaire_artefact(info_client->hero);
                    info_client->hero=ramasser_artefact(info_client->hero,carte_a_envoyer.artefacts[id_cartefact_a_ramasser],premiere_place_libre);
                    carte_a_envoyer.artefacts[id_cartefact_a_ramasser]=create_artefact("-1",0,0,0,0,0);
                    carte_a_envoyer.cases[info_client->hero.cooX][info_client->hero.cooY+1].indelem=-1;
                }
                   
                carte_a_envoyer.cases[info_client->hero.cooX][info_client->hero.cooY].elem=' ';
                info_client->hero.cooY=info_client->hero.cooY+1;
                carte_a_envoyer.cases[info_client->hero.cooX][info_client->hero.cooY].elem='H';
            }

      }
      else{
        printf("Génération vers le bas\n");
      }
      pthread_mutex_unlock(&mutex);
      break;

  }

}
/*
* Si on éteint le client, on efface l'élément H de la carte
*/
pthread_mutex_lock(&mutex);
carte_a_envoyer.cases[info_client->hero.cooX][info_client->hero.cooY].elem=' ';
pthread_mutex_unlock(&mutex);


return NULL;
}







int main(int argc,char* argv[]){
// Vérification des arguments
int fd,sockclient[1024];
struct sockaddr_in adresse;
info_client_t info_client[1024];
int nb_client=0;
char nom_repertoire[1024];
pthread_t thread[1024];
DIR* directory;
char* cartes[300];
off_t cartes_emplacement_sav[1024][1024];



if(argc!=3){
    fprintf(stderr,"Pas assez d'argument");
    exit(EXIT_FAILURE);
}
strcpy(nom_repertoire,argv[2]);
// Ouverture du repertoire côté serveur
if((directory=opendir(nom_repertoire))==NULL){
    fprintf(stderr,"Erreur d'ouverture du repertoire");
    exit(EXIT_FAILURE);
}
srand(time(NULL));


struct dirent* info_directory;
int nb_carte_dans_repertoire=0;
char* carte_to_load;
while((info_directory=readdir(directory))!=NULL){
     if (strstr(info_directory->d_name, ".crt") != NULL) {
         carte_to_load=malloc(strlen(nom_repertoire)*sizeof(char)+strlen(info_directory->d_name)*sizeof(char));
         strcpy(carte_to_load,nom_repertoire);
         strcat(carte_to_load,"/");
         strcat(carte_to_load,info_directory->d_name);
        cartes[nb_carte_dans_repertoire]=malloc(strlen(carte_to_load)*sizeof(char));
        strcpy(cartes[nb_carte_dans_repertoire],carte_to_load);
        printf("%s\n",cartes[nb_carte_dans_repertoire]);
        nb_carte_dans_repertoire++;
     }   
}
/*
*  OPENING MONDE.SAV
*/ 
        int world_descriptor;
        if((world_descriptor=open("monde.sav",O_CREAT|O_TRUNC|O_RDWR,S_IRUSR|S_IWUSR))==-1){ /*  OPENING   */
          perror("Erreur ouverture fichier 1");                         /*  FILE  */
          exit(EXIT_FAILURE);
        }
        pthread_mutex_lock(&mutex);
         carte_t carte_a_envoyer=charger_carte("map/debut.crt");
         cartes_emplacement_sav[0][0]=enregistrer_new_sav_carte(&carte_a_envoyer,"monde.sav",world_descriptor);
         pthread_mutex_unlock(&mutex);
         printf("%ld",cartes_emplacement_sav[0][0]);

         printf("Enregistrement de la première carte de jeu dans monde.sav");
         


// Création de la socket
if((fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1) {
    perror("Erreur lors de la création de la socket ");
    exit(EXIT_FAILURE);
}
// Création de l'adresse du serveur
memset(&adresse, 0, sizeof(struct sockaddr_in));
adresse.sin_family = AF_INET;
adresse.sin_addr.s_addr = htonl(INADDR_ANY);
adresse.sin_port = htons(atoi(argv[1]));

 // Nommage de la socket
if(bind(fd, (struct sockaddr*)&adresse, sizeof(struct sockaddr_in)) == -1) {
    perror("Erreur lors du nommage de la socket ");
    exit(EXIT_FAILURE);
}

// Mise en mode passif de la socket
if(listen(fd, 1) == -1) {
    perror("Erreur lors de la mise en mode passif ");
    exit(EXIT_FAILURE);
}
 while(stop == 0) {
        // Attente d'une connexion
        printf("Serveur : attente de connexion...\n");
        if((sockclient[nb_client] = accept(fd, NULL, NULL)) == -1) {
            if(errno != EINTR) {
                perror("Erreur lors de la demande de connexion ");
                exit(EXIT_FAILURE);
            }
        }
        else {

             info_client[nb_client].world_descriptor=world_descriptor;
             info_client[nb_client].hero=create_hero("H");
             info_client[nb_client].num_client=nb_client;
             info_client[nb_client].sockclient=sockclient[nb_client];
             info_client[nb_client].nom_repertoire=malloc(sizeof(char)*strlen(nom_repertoire)+1);
             info_client[nb_client].directory=directory;
             strcpy(info_client[nb_client].nom_repertoire,nom_repertoire);
             if(pthread_create(&thread[nb_client],NULL,thread_client,&info_client[nb_client])!=0){
                 perror("Erreur de création du thread d'affichage");
                 exit(EXIT_FAILURE);
             };
        }
        nb_client++;


 }

    for(int j=0;j<nb_client;j++){
        pthread_join(thread[nb_client],NULL);
    }




}