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
#define DEMANDE_HERO 3
#define USE 101

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

typedef struct data_to_monstre{

    monstre_t monstre;
    carte_t* carte;
    int indice_monstre;
    liste_carte_t* liste_carte;
}data_to_monstre_t;

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

hero_t ramasser_piece_grand_tout(hero_t hero){

    if(hero.nb_piece_grand_tout<3){
        hero.nb_piece_grand_tout++;
    }
return hero;
}

/*
* Routine du thread gérant les clients du côté serveur.
*/
pthread_mutex_t mutex= PTHREAD_MUTEX_INITIALIZER;
int stop=0;
liste_carte_t* liste_carte_active;
tabnodes_t* l_carte;
int world_descriptor;
int nb_carte_dans_repertoire;
pthread_t id_thread_monstre[1024];
char* cartes[300];
pthread_mutex_t mutex_cases[40][20];



void *thread_spawn_aleatoire(void *arg){

while(stop==0){
    // On tire un nombre aléatoire entre 0-60
    int nb_tempos_attente=generer_nombre_aleatoire(60);
    nb_tempos_attente=nb_tempos_attente+60;
    // On prend la liste des maps active qu'on parcours, on gènère deux coordonnées aléatoires et si elles sont dispo on y place un $.
    pthread_mutex_lock(&mutex);
    mettre_tresor_alea(liste_carte_active);
    pthread_mutex_unlock(&mutex);
    sleep(nb_tempos_attente);
}


return NULL;
}

void *thread_monstre(void *arg){
  int stop=0;
 
  data_to_monstre_t* datamonstre=(data_to_monstre_t*) arg;
  // On cherche l'indice grâce à l'ID du monstre (Parcours de 800 cases)
  int posX=0;
  int posY=0;
    for(int i=0;i<40;i++){
        for(int j=0;j<20;j++){
                if(datamonstre->carte->cases[i][j].indelem==datamonstre->indice_monstre){
                    posX=i;
                    posY=j;
                }
        }
    }
    printf("%d,%d",posX,posY);
    while(stop==0){
         // printf("MONSTRE :%d NOM : %s",datamonstre->indice_monstre,datamonstre->monstre.name);

        if(datamonstre->monstre.speed>0){
            int pos=generer_nombre_aleatoire(4);
            switch(pos){ // Il y a un segmentation fault à l'intérieur de ce code, que je ne comprend pas, je l'ai donc laissé en //, ici les monstres sont donc statiques.
                /*
                  case 0:// MOVE GAUCHE
                   if(posX-1>=0){
                    datamonstre->carte->cases[posX][posY].elem='s';
                    datamonstre->carte->cases[posX][posY].indelem=-1;
                    posX=posX-1;
                    datamonstre->carte->cases[posX][posY].elem='M';
                    datamonstre->carte->cases[posX][posY].indelem=datamonstre->indice_monstre;
                    }

                break;

                case 1:
                 if(posX+1<=40){
                    datamonstre->carte->cases[posX][posY].elem='s';
                    datamonstre->carte->cases[posX][posY].indelem=-1;
                    posX=posX+1;
                    datamonstre->carte->cases[posX][posY].elem='M';
                    datamonstre->carte->cases[posX][posY].indelem=datamonstre->indice_monstre;
                 }

                break;
                case 2:                    
                    if(posY+1<=40){
                    datamonstre->carte->cases[posX][posY].elem='s';
                    datamonstre->carte->cases[posX][posY].indelem=-1;
                    posY=posY+1;
                    datamonstre->carte->cases[posX][posY].elem='M';
                    datamonstre->carte->cases[posX][posY].indelem=datamonstre->indice_monstre;
                    }
                break;
                case 3: 
                   if(posY-1>=0){   
                    datamonstre->carte->cases[posX][posY].elem='s';
                    datamonstre->carte->cases[posX][posY].indelem=-1;
                     posY=posY-1;
                    datamonstre->carte->cases[posX][posY].elem='M';
                    datamonstre->carte->cases[posX][posY].indelem=datamonstre->indice_monstre;
                    
                   }
                break;
                */
            }
        
        
        
        sleep(1);

        }

        
    }
return NULL;
}

void lancer_thread_monstre(liste_carte_t *liste,int x,int y){
    data_to_monstre_t data_to_monstre[20];
    if (liste != NULL)
    {
        carte_chainee_t *current = liste->tete;
        while (current != NULL)
        {
            if (current->x==x && current->y==y)
            {
                  for(int i=0;i<current->carte.nbmob;i++){
                        data_to_monstre[i].indice_monstre=i;
                        data_to_monstre[i].monstre=current->carte.monstre[i];
                        data_to_monstre[i].carte=&current->carte;
                        data_to_monstre[i].liste_carte=liste;
                        pthread_create(&current->id_thread_monstre[i],NULL,thread_monstre,&data_to_monstre[i]);                   
                }
            }
            current = current->suivant;

        }
    }
}
void eteindre_thread_monstre(liste_carte_t *liste,int x,int y){
    if (liste != NULL)
    {
        carte_chainee_t *current = liste->tete;
        while (current != NULL)
        {
            if (current->x==x && current->y==y)
            {
                for(int i=0;i<current->carte.nbmob;i++){
                        pthread_cancel(current->id_thread_monstre[i]);
                }
            }
            current = current->suivant;
        }
    }
}




void *thread_client(void *arg){ 

info_client_t* info_client =(info_client_t*) arg; 
int demande_client_to_server;
int confirmation_eteinte=2;
int stop_thread=0;
carte_t carte_a_envoyer;
// Message d'indication du client
printf("Connexion du client numero %d\n",info_client->num_client); 
info_client->hero=create_hero("H");
pthread_mutex_lock(&mutex);
// 0 = not found
if(chercher_map_from_list(liste_carte_active,info_client->hero.carteX,info_client->hero.carteY,carte_a_envoyer)==0){
      // => Check dans la table stocké dans le fichier [Devoir la recharger à chaque fois]
            if(lseek(world_descriptor,0,SEEK_SET)==-1){
                     perror("Déplacement fichier ");                         /*  FILE  */
                     exit(EXIT_FAILURE);
             }
            l_carte=lire_tab_nodes_dans_fichier(world_descriptor);
            if(dans_tabnodes(l_carte,info_client->hero.carteX,info_client->hero.carteY)==0){ 
               // ICI NE RIEN FAIRE, COMME LA CARTE PAR DEFAULT EST FIXÉ, ALORS IL EST OBLIGATOIREMENT DANS LE TABLEAUA DE NODES
            }
            else{
              // Si présent dans la table :   On load à l'emplacement et on insère   
                off_t emplacement_carte_to_load=trouver_emplacement_par_tabnodes(l_carte,info_client->hero.carteX,info_client->hero.carteY);   
                carte_a_envoyer=charger_carte_monde_sav("monde.sav",world_descriptor,emplacement_carte_to_load);
                carte_a_envoyer.cases[info_client->hero.cooX][info_client->hero.cooY].elem='H';
                carte_a_envoyer.nb_joueur=1;
                liste_carte_active=inserer_liste(liste_carte_active,info_client->hero.carteX,info_client->hero.carteY,carte_a_envoyer);
                printf("LANCEMENT THREAD\n");
                lancer_thread_monstre(liste_carte_active,0,0);


            }
  }
  else{
          // Si trouvé dans la liste : carte = Liste_carte[X]
          carte_a_envoyer=map_from_list(liste_carte_active,info_client->hero.carteX,info_client->hero.carteY);
          carte_a_envoyer.cases[info_client->hero.cooX][info_client->hero.cooY].elem='H';
          carte_a_envoyer.nb_joueur++;
          mettre_a_jour_map_in_list(liste_carte_active,info_client->hero.carteX,info_client->hero.carteY,carte_a_envoyer,world_descriptor);

            
  }
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
      carte_a_envoyer=map_from_list(liste_carte_active,info_client->hero.carteX,info_client->hero.carteY);

      if(write(info_client->sockclient, &carte_a_envoyer, sizeof(carte_a_envoyer)) == -1) {
            perror("Erreur lors de l'envoi de la valeur ");
            exit(EXIT_FAILURE);
      }
      pthread_mutex_unlock(&mutex);
      break;

     case DEMANDE_HERO : 
      pthread_mutex_lock(&mutex);
      
      if(write(info_client->sockclient, &info_client->hero, sizeof(hero_t)) == -1) {
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
      carte_a_envoyer=map_from_list(liste_carte_active,info_client->hero.carteX,info_client->hero.carteY);
      if( carte_a_envoyer.nb_joueur-1<=0){
          carte_a_envoyer.nb_joueur=0;
      }
      else{
          carte_a_envoyer.nb_joueur--;
      }
     mettre_a_jour_map_in_list(liste_carte_active,info_client->hero.carteX, info_client->hero.carteY,carte_a_envoyer,world_descriptor);
  

     
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
      if(info_client->hero.cooY-1>=0){// Si tu peux bouger sans changer de map.
         if(carte_a_envoyer.cases[info_client->hero.cooX][info_client->hero.cooY-1].code_couleur!=2 && carte_a_envoyer.cases[info_client->hero.cooX][info_client->hero.cooY-1].elem!='X'){ // Si pas d'obstacle
                if(carte_a_envoyer.cases[info_client->hero.cooX][info_client->hero.cooY-1].elem=='$'){ // Si y'a un trésor on l'interprête
                        if(generer_nombre_aleatoire(2)==1){
                            printf("Heal de %d",info_client->num_client);
                            info_client->hero.health=info_client->hero.health_max;
                        }
                        else{
                            printf("RAMASSAGE PIECE GRAND TOUT\n");
                            info_client->hero=ramasser_piece_grand_tout(info_client->hero);
                        }    
                }
                if(carte_a_envoyer.cases[info_client->hero.cooX][info_client->hero.cooY-1].elem=='M'){
                    int resultat;
                    resultat=fight(info_client->hero,carte_a_envoyer.monstre[carte_a_envoyer.cases[info_client->hero.cooX][info_client->hero.cooY-1].indelem]);
                    if(resultat==1)//Le monstre a gagné
                {}
                    else if(resultat==2){
                        pthread_cancel(id_thread_monstre[carte_a_envoyer.cases[info_client->hero.cooX][info_client->hero.cooY-1].indelem]);
                    }

                }

                if(carte_a_envoyer.cases[info_client->hero.cooX][info_client->hero.cooY-1].elem=='A'){ // Si il y a un artefact on le ramasse au premier emplacement libre
                    int  id_cartefact_a_ramasser=carte_a_envoyer.cases[info_client->hero.cooX][info_client->hero.cooY-1].indelem;
                    int premiere_place_libre=premiere_place_libre_inventaire_artefact(info_client->hero);
                    info_client->hero=ramasser_artefact(info_client->hero,carte_a_envoyer.artefacts[id_cartefact_a_ramasser],premiere_place_libre);
                    carte_a_envoyer.artefacts[id_cartefact_a_ramasser]=create_artefact("-1",0,0,0,0,0);
                    carte_a_envoyer.cases[info_client->hero.cooX][info_client->hero.cooY-1].indelem=-1;
                }

                carte_a_envoyer.cases[info_client->hero.cooX][info_client->hero.cooY].elem=' ';
                info_client->hero.cooY=info_client->hero.cooY-1;
                carte_a_envoyer.cases[info_client->hero.cooX][info_client->hero.cooY].elem='H';
                mettre_a_jour_map_in_list(liste_carte_active,info_client->hero.carteX, info_client->hero.carteY,carte_a_envoyer,world_descriptor);
            }
      }
      else{ // Changement de map vers le haut
            carte_a_envoyer=map_from_list(liste_carte_active,info_client->hero.carteX,info_client->hero.carteY);
            carte_a_envoyer.cases[info_client->hero.cooX][info_client->hero.cooY].elem=' ';
            if(carte_a_envoyer.nb_joueur<=1){
                printf("Le dernier joueur sur la carte a quitté");
                carte_a_envoyer.nb_joueur=0;
                eteindre_thread_monstre(liste_carte_active,info_client->hero.carteX,info_client->hero.carteY);
            }
            else{
                carte_a_envoyer.nb_joueur--;
            }
            
            mettre_a_jour_map_in_list(liste_carte_active,info_client->hero.carteX,info_client->hero.carteY,carte_a_envoyer,world_descriptor);
            info_client->hero.cooY=19;    
            info_client->hero.carteY=info_client->hero.carteY+1;   
           
            // Si pas dans la liste => Cherche dans la table
            if(chercher_map_from_list(liste_carte_active,info_client->hero.carteX,info_client->hero.carteY,carte_a_envoyer)==0){  
                    if(lseek(world_descriptor,0,SEEK_SET)==-1){
                            perror("Déplacement fichier ");                        
                            exit(EXIT_FAILURE);
                    }
                    l_carte=lire_tab_nodes_dans_fichier(world_descriptor);
                    if(dans_tabnodes(l_carte,info_client->hero.carteX,info_client->hero.carteY)==0){ //0=absent, 1== présent
                           int numero_carte_generer=generer_nombre_aleatoire(nb_carte_dans_repertoire);
                           carte_a_envoyer=charger_carte(cartes[numero_carte_generer]); 
                           carte_a_envoyer.nb_joueur=1;                            
                           rajouter_carte_monde_sav(world_descriptor,l_carte,carte_a_envoyer,info_client->hero.carteX,info_client->hero.carteY);  // On la rajoute dans l'emplacement numéro 1, et on l'enregistre   
                           liste_carte_active=inserer_liste(liste_carte_active,info_client->hero.carteX,info_client->hero.carteY,carte_a_envoyer);
                           lancer_thread_monstre(liste_carte_active,info_client->hero.carteX,info_client->hero.carteY);

                     }
                     else{
                          off_t emplacement_carte_to_load=trouver_emplacement_par_tabnodes(l_carte,info_client->hero.carteX,info_client->hero.carteY);   
                          carte_a_envoyer=charger_carte_monde_sav("monde.sav",world_descriptor,emplacement_carte_to_load);
                          carte_a_envoyer.cases[info_client->hero.cooX][info_client->hero.cooY].elem='H';
                          carte_a_envoyer.nb_joueur=1; 
                          liste_carte_active=inserer_liste(liste_carte_active,info_client->hero.carteX,info_client->hero.carteY,carte_a_envoyer);
                          lancer_thread_monstre(liste_carte_active,info_client->hero.carteX,info_client->hero.carteY);

                     }
            }
            else{// Cependant, si elle est déjà activer par un joueur, alors on la charge grâce à la liste.
                  carte_a_envoyer=map_from_list(liste_carte_active,info_client->hero.carteX,info_client->hero.carteY);
                  carte_a_envoyer.cases[info_client->hero.cooX][info_client->hero.cooY].elem='H';
                  carte_a_envoyer.nb_joueur++;
                  mettre_a_jour_map_in_list(liste_carte_active,info_client->hero.carteX,info_client->hero.carteY,carte_a_envoyer,world_descriptor);
            }
                 
      }
      pthread_mutex_unlock(&mutex);
      break;

      case LEFT:
      pthread_mutex_lock(&mutex);
      if(info_client->hero.cooX-1>=0){ // Si tu peux bouger sans changer de map.
            if(carte_a_envoyer.cases[info_client->hero.cooX-1][info_client->hero.cooY].code_couleur!=2 && carte_a_envoyer.cases[info_client->hero.cooX-1][info_client->hero.cooY].elem!='X'){
                if(carte_a_envoyer.cases[info_client->hero.cooX-1][info_client->hero.cooY].elem=='$'){ // Si il y  a un trésor dans la case de départ
                        if(generer_nombre_aleatoire(2)==1){             // détermination de s'il s'agit de heal ou d'une piece de grand tout.
                            printf("Heal de %d",info_client->num_client);
                            info_client->hero.health=info_client->hero.health_max;
                        }
                        else{
                             info_client->hero=ramasser_piece_grand_tout(info_client->hero);
                        }    
                }
                if(carte_a_envoyer.cases[info_client->hero.cooX-1][info_client->hero.cooY].elem=='A'){// Si il y y a un artefact alors tu le ramasse dans la première pièce de ton inventaire libre.
                    int  id_cartefact_a_ramasser=carte_a_envoyer.cases[info_client->hero.cooX-1][info_client->hero.cooY].indelem;
                    int premiere_place_libre=premiere_place_libre_inventaire_artefact(info_client->hero);
                    info_client->hero=ramasser_artefact(info_client->hero,carte_a_envoyer.artefacts[id_cartefact_a_ramasser],premiere_place_libre);
                    carte_a_envoyer.artefacts[id_cartefact_a_ramasser]=create_artefact("-1",0,0,0,0,0);
                    carte_a_envoyer.cases[info_client->hero.cooX-1][info_client->hero.cooY].indelem=-1;
                }

                carte_a_envoyer.cases[info_client->hero.cooX][info_client->hero.cooY].elem=' '; // Mise à jour de la map dans la liste de map
                info_client->hero.cooX=info_client->hero.cooX-1;
                carte_a_envoyer.cases[info_client->hero.cooX][info_client->hero.cooY].elem='H';
                mettre_a_jour_map_in_list(liste_carte_active,info_client->hero.carteX,info_client->hero.carteY,carte_a_envoyer,world_descriptor);
            }
      }
      else{// Si tu dois générer une map
            carte_a_envoyer=map_from_list(liste_carte_active,info_client->hero.carteX,info_client->hero.carteY); // Pour reprendre la map à jour
            carte_a_envoyer.cases[info_client->hero.cooX][info_client->hero.cooY].elem=' ';                     // Pour supprimer le H à l'ancienne case
               if(carte_a_envoyer.nb_joueur<=1){
                printf("Le dernier joueur sur la carte a quitté");
                carte_a_envoyer.nb_joueur=0;
                eteindre_thread_monstre(liste_carte_active,info_client->hero.carteX,info_client->hero.carteY);

                }
                else{
                carte_a_envoyer.nb_joueur--;
                }
            
            mettre_a_jour_map_in_list(liste_carte_active,info_client->hero.carteX,info_client->hero.carteY,carte_a_envoyer,world_descriptor);
            info_client->hero.cooX=40;                                                                          // On repop le perso à l'extrémité
            info_client->hero.carteX=info_client->hero.carteX-1;                                                // On décrémente d'une carte.

            if(chercher_map_from_list(liste_carte_active,info_client->hero.carteX,info_client->hero.carteY,carte_a_envoyer)==0){  // Si pas dans la liste des cartes actives
                    if(lseek(world_descriptor,0,SEEK_SET)==-1){
                            perror("Déplacement fichier ");                         
                            exit(EXIT_FAILURE);
                    }
                    l_carte=lire_tab_nodes_dans_fichier(world_descriptor); // On lit le tableau de nodes du fichier monde.sav
                    if(dans_tabnodes(l_carte,info_client->hero.carteX,info_client->hero.carteY)==0){ // Si il est absent du tableau de nodes, on génére une map aléatoire.
                           int numero_carte_generer=generer_nombre_aleatoire(nb_carte_dans_repertoire);
                           carte_a_envoyer=charger_carte(cartes[numero_carte_generer]);
                           carte_a_envoyer.nb_joueur=1;                            
                           rajouter_carte_monde_sav(world_descriptor,l_carte,carte_a_envoyer,info_client->hero.carteX,info_client->hero.carteY);    
                           liste_carte_active=inserer_liste(liste_carte_active,info_client->hero.carteX,info_client->hero.carteY,carte_a_envoyer);
                           lancer_thread_monstre(liste_carte_active,info_client->hero.carteX,info_client->hero.carteY);

                          // Si pas présent dans la table : génère la map, insertion dans la table, écrire la table dans le file à l'emplacement 0 et insertion dans la liste chaînée     
                            
                            
                     }
                     else{ // Si présent, on cherche l'emplacement k grâce au tableau de nodes, et on charge la map à l'emplacement k, qu'on place dans la liste des cartes actives.
                          off_t emplacement_carte_to_load=trouver_emplacement_par_tabnodes(l_carte,info_client->hero.carteX,info_client->hero.carteY);   
                          carte_a_envoyer=charger_carte_monde_sav("monde.sav",world_descriptor,emplacement_carte_to_load);
                          carte_a_envoyer.nb_joueur=1;                            
                          carte_a_envoyer.cases[info_client->hero.cooX][info_client->hero.cooY].elem='H';
                          liste_carte_active=inserer_liste(liste_carte_active,info_client->hero.carteX,info_client->hero.carteY,carte_a_envoyer);
                          lancer_thread_monstre(liste_carte_active,info_client->hero.carteX,info_client->hero.carteY);

                     }
            }
            else{ // Cependant, si elle est déjà activer par un joueur, alors on la charge grâce à la liste.
                  carte_a_envoyer=map_from_list(liste_carte_active,info_client->hero.carteX,info_client->hero.carteY);
                  carte_a_envoyer.cases[info_client->hero.cooX][info_client->hero.cooY].elem='H';
                  carte_a_envoyer.nb_joueur++;                            
                  mettre_a_jour_map_in_list(liste_carte_active,info_client->hero.carteX,info_client->hero.carteY,carte_a_envoyer,world_descriptor);
            }

       if(info_client->hero.carteX+1==0 && info_client->hero.carteY==0){
            carte_t hold_card=map_from_list(liste_carte_active,0,0);
            hold_card.cases[1][info_client->hero.cooY].elem=' ';
            hold_card.nb_joueur--;
            mettre_a_jour_map_in_list(liste_carte_active,0,0,carte_a_envoyer,world_descriptor);
       }


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
                             info_client->hero=ramasser_piece_grand_tout(info_client->hero);
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
                mettre_a_jour_map_in_list(liste_carte_active,info_client->hero.carteX,info_client->hero.carteY,carte_a_envoyer,world_descriptor);
            }
      }
      else{
            carte_a_envoyer.cases[info_client->hero.cooX][info_client->hero.cooY].elem=' ';
               if(carte_a_envoyer.nb_joueur<=1){
                printf("Le dernier joueur sur la carte a quitté");
                carte_a_envoyer.nb_joueur=0;
                eteindre_thread_monstre(liste_carte_active,info_client->hero.carteX,info_client->hero.carteY);

                }
                else{
                carte_a_envoyer.nb_joueur--;
                }
            
            mettre_a_jour_map_in_list(liste_carte_active,info_client->hero.carteX,info_client->hero.carteY,carte_a_envoyer,world_descriptor);

            info_client->hero.cooX=0;    
            info_client->hero.carteX=info_client->hero.carteX+1;   

          
            // Si pas dans la liste => Cherche dans la table
            if(chercher_map_from_list(liste_carte_active,info_client->hero.carteX,info_client->hero.carteY,carte_a_envoyer)==0){  
                    if(lseek(world_descriptor,0,SEEK_SET)==-1){
                            perror("Déplacement fichier ");                         /*  FILE  */
                            exit(EXIT_FAILURE);
                    }
                    l_carte=lire_tab_nodes_dans_fichier(world_descriptor);
                    if(dans_tabnodes(l_carte,info_client->hero.carteX,info_client->hero.carteY)==0){ //0=absent, 1== présent
                           int numero_carte_generer=generer_nombre_aleatoire(nb_carte_dans_repertoire);
                           carte_a_envoyer=charger_carte(cartes[numero_carte_generer]); 
                           carte_a_envoyer.nb_joueur=1;
                           rajouter_carte_monde_sav(world_descriptor,l_carte,carte_a_envoyer,info_client->hero.carteX,info_client->hero.carteY);  // On la rajoute dans l'emplacement numéro 1, et on l'enregistre   
                           liste_carte_active=inserer_liste(liste_carte_active,info_client->hero.carteX,info_client->hero.carteY,carte_a_envoyer);
                           lancer_thread_monstre(liste_carte_active,info_client->hero.carteX,info_client->hero.carteY);

                          // Si pas présent dans la table : génère la map, insertion dans la table, écrire la table dans le file à l'emplacement 0 et insertion dans la liste chaînée     
                            
                            
                     }
                     else{
                          off_t emplacement_carte_to_load=trouver_emplacement_par_tabnodes(l_carte,info_client->hero.carteX,info_client->hero.carteY);   
                          carte_a_envoyer=charger_carte_monde_sav("monde.sav",world_descriptor,emplacement_carte_to_load);
                          carte_a_envoyer.cases[info_client->hero.cooX][info_client->hero.cooY].elem='H';
                          carte_a_envoyer.nb_joueur=1;                            
                          liste_carte_active=inserer_liste(liste_carte_active,info_client->hero.carteX,info_client->hero.carteY,carte_a_envoyer);
                          lancer_thread_monstre(liste_carte_active,info_client->hero.carteX,info_client->hero.carteY);

                     }
            }
            else{ // Cependant, si elle est déjà activer par un jour, alors on la charge grâce à la liste.
                  carte_a_envoyer=map_from_list(liste_carte_active,info_client->hero.carteX,info_client->hero.carteY);
                  carte_a_envoyer.cases[info_client->hero.cooX][info_client->hero.cooY].elem='H';
                  carte_a_envoyer.nb_joueur++;                            
                  mettre_a_jour_map_in_list(liste_carte_active,info_client->hero.carteX,info_client->hero.carteY,carte_a_envoyer,world_descriptor);
            }
      }
     
      pthread_mutex_unlock(&mutex);
      break;

      case BOTTOM:
      pthread_mutex_lock(&mutex);
      if(info_client->hero.cooY+1<=19){
           if(carte_a_envoyer.cases[info_client->hero.cooX][info_client->hero.cooY+1].code_couleur!=2 && carte_a_envoyer.cases[info_client->hero.cooX][info_client->hero.cooY+1].elem!='X'){
                if(carte_a_envoyer.cases[info_client->hero.cooX][info_client->hero.cooY+1].elem=='$'){
                        if(generer_nombre_aleatoire(2)==1){
                            printf("Heal de %d",info_client->num_client);
                            info_client->hero.health=info_client->hero.health_max;
                        }
                        else{
                             info_client->hero=ramasser_piece_grand_tout(info_client->hero);
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
                  mettre_a_jour_map_in_list(liste_carte_active,info_client->hero.carteX,info_client->hero.carteY,carte_a_envoyer,world_descriptor);
            }

      }
      else{
            carte_a_envoyer=map_from_list(liste_carte_active,info_client->hero.carteX,info_client->hero.carteY);
            carte_a_envoyer.cases[info_client->hero.cooX][info_client->hero.cooY].elem=' ';
              if(carte_a_envoyer.nb_joueur<=1){
                printf("Le dernier joueur sur la carte a quitté");
                carte_a_envoyer.nb_joueur=0;
                eteindre_thread_monstre(liste_carte_active,info_client->hero.carteX,info_client->hero.carteY);

                }
                else{
                carte_a_envoyer.nb_joueur--;
                }
            
                mettre_a_jour_map_in_list(liste_carte_active,info_client->hero.carteX,info_client->hero.carteY,carte_a_envoyer,world_descriptor);

            info_client->hero.cooY=0;    
            info_client->hero.carteY=info_client->hero.carteY-1;   

          
            if(chercher_map_from_list(liste_carte_active,info_client->hero.carteX,info_client->hero.carteY,carte_a_envoyer)==0){  // Si pas dans la liste active, on regarde dans le tableau de nodes
                    if(lseek(world_descriptor,0,SEEK_SET)==-1){
                            perror("Déplacement fichier ");                         /*  FILE  */
                            exit(EXIT_FAILURE);
                    }
                    l_carte=lire_tab_nodes_dans_fichier(world_descriptor);       
                    if(dans_tabnodes(l_carte,info_client->hero.carteX,info_client->hero.carteY)==0){ //Si il est absent du tableau de nodes
                           int numero_carte_generer=generer_nombre_aleatoire(nb_carte_dans_repertoire); // On génère un nombre aléatoire
                           carte_a_envoyer=charger_carte(cartes[numero_carte_generer]);  // Puis ce nombre nous donne le numéro de map
                           carte_a_envoyer.nb_joueur=1;
                           rajouter_carte_monde_sav(world_descriptor,l_carte,carte_a_envoyer,info_client->hero.carteX,info_client->hero.carteY);  // Qu'on rajoute au sein de notre monde.sav
                           liste_carte_active=inserer_liste(liste_carte_active,info_client->hero.carteX,info_client->hero.carteY,carte_a_envoyer); // Et de notre liste.
                           lancer_thread_monstre(liste_carte_active,info_client->hero.carteX,info_client->hero.carteY);

                            
                            
                     }
                     else{// Si il est dans le tableau de nodes alors on load la carte grâce à l'emplacement obtenu dans le tableau de nodes
                          off_t emplacement_carte_to_load=trouver_emplacement_par_tabnodes(l_carte,info_client->hero.carteX,info_client->hero.carteY);   
                          carte_a_envoyer=charger_carte_monde_sav("monde.sav",world_descriptor,emplacement_carte_to_load);
                          carte_a_envoyer.cases[info_client->hero.cooX][info_client->hero.cooY].elem='H';
                          carte_a_envoyer.nb_joueur=1;
                          liste_carte_active=inserer_liste(liste_carte_active,info_client->hero.carteX,info_client->hero.carteY,carte_a_envoyer);
                          lancer_thread_monstre(liste_carte_active,info_client->hero.carteX,info_client->hero.carteY);

                     }
            }
            else{// Cependant, si elle est déjà activer par un joueur, alors on la charge grâce à la liste.
                  carte_a_envoyer=map_from_list(liste_carte_active,info_client->hero.carteX,info_client->hero.carteY);
                  carte_a_envoyer.cases[info_client->hero.cooX][info_client->hero.cooY].elem='H';
                  carte_a_envoyer.nb_joueur++;
                  mettre_a_jour_map_in_list(liste_carte_active,info_client->hero.carteX,info_client->hero.carteY,carte_a_envoyer,world_descriptor);
            }
      }
      pthread_mutex_unlock(&mutex);
      break;

      case USE:
      printf("DEMANDE D'UTILISATION\n");
      if(info_client->hero.nb_piece_grand_tout==1){
          // RALENTISSEMENT     // Baisse la vitesse de déplacement /2
      } 
      if(info_client->hero.nb_piece_grand_tout==2){
          // GEL TOTAL DEPLACEMENT // Reduction de la vitesse de déplacement à 0
      } 
      if(info_client->hero.nb_piece_grand_tout==3){
          //GEL + MIHP          // Mi hp + vitesse à 0
      }
      break;

  }

}
/*
* Si on éteint le client, on efface l'élément H de la carte
*/
pthread_mutex_lock(&mutex);
carte_a_envoyer.cases[info_client->hero.cooX][info_client->hero.cooY].elem=' ';
printf("JOUEUR SUR LA MAP RESTANT :%d\n",carte_a_envoyer.nb_joueur);
 if(carte_a_envoyer.nb_joueur-1<=0){
      eteindre_thread_monstre(liste_carte_active,info_client->hero.carteX,info_client->hero.carteY);
 }
mettre_a_jour_map_in_list(liste_carte_active,info_client->hero.carteX,info_client->hero.carteY,carte_a_envoyer,world_descriptor);
                 
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
pthread_t thread_alea;
DIR* directory;
liste_carte_active=init_liste_carte();
carte_t carte_a_envoyer;

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
l_carte=init_tabnodes_t();
node_t node;
node.x=0;
node.y=0;
node.emplacement_carte=0;
   
for(int i=0;i<50;i++){
    inserer_tab_nodes(l_carte,node);
}
pthread_mutex_t mutex_cases[40][20];
for(int i=0;i<40;i++){
    for(int j=0;j<20;j++){
       pthread_mutex_init(&mutex_cases[i][j], NULL);
    }
}
        // On tente d'ouvrir le fichier pour tester s'il existe ou non
        if((world_descriptor=open("monde.sav",O_CREAT|O_EXCL|O_RDWR,S_IRUSR|S_IWUSR))==-1){
               if(errno==EEXIST){   
                   // Si il y a échec et que c'est parce qu'il existe, alors on l'ouvre, et on traîte le tableau de nodes en début.
                   if((world_descriptor=open("monde.sav",O_RDWR,S_IRUSR|S_IWUSR))==-1){
                        perror("ERREUR OUVERTURE");
                        exit(EXIT_FAILURE);
                   }
                   l_carte=lire_tab_nodes_dans_fichier(world_descriptor); // Traitement tableau nodes

               }
               else{
                    perror("ERREUR OUVERTURE");
                    exit(EXIT_FAILURE);
               }
        }
        else{
                   // Si le fichier n'existe pas, alors il est crée par le open, on ecrit dedans un tableau de nodes par défault afin de reconnaître les cartes.
                   printf("Le fichier existe pas");
                   ecrire_tab_nodes_dans_fichier(l_carte,world_descriptor,0);
                   carte_a_envoyer=charger_carte("map/debut.crt"); // On charge la carte de départ.
                   rajouter_carte_monde_sav(world_descriptor,l_carte,carte_a_envoyer,0,0);  // On la rajoute dans l'emplacement numéro 1, et on l'enregistre   

        }
    printf("%d",l_carte->length_total);

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
if(pthread_create(&thread_alea,NULL,thread_spawn_aleatoire,NULL)!=0){
        perror("Erreur de création du thread de trésor");
        exit(EXIT_FAILURE);
};


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
     pthread_join(thread_alea,NULL);

    for(int j=0;j<1024;j++){
        pthread_join(id_thread_monstre[j],NULL);
    }


}