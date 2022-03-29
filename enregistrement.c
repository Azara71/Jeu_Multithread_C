#include <locale.h>     // Pour setlocal
#include <stdlib.h>     // Pour EXIT_SUCCESS
#include <ncurses.h>    // Pour les fonctions/constantes ncurses
#include "fonction_ncurses.h"  // Les fonctions personnalisées
#include "monstre.h"
#include "artefact.h"
#include "carte.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h> 
#include <string.h>


#define BLUE 2
#define YELLOW 3
#define GREEN 4
#define RED 5

void enregistrer_monstre(int fd,monstre_t monstre){
    int taille_att=0;
    int retour;
    taille_att=strlen(monstre.name)+1;

// ECRITURE TAILLE DU NOM
    if((retour=write(fd,&taille_att,sizeof(int)))==-1){
    perror("Erreur");
    exit(EXIT_FAILURE);
}

// ECRITURE NOM
     if(write(fd,monstre.name,sizeof(char)*taille_att)==-1){
          perror("Erreur d'écriture");                            
          exit(EXIT_FAILURE);
    }
// ECRITURE ARMOR
      if(write(fd,&monstre.health,sizeof(unsigned short int))==-1){
          perror("Erreur d'écriture");                            
          exit(EXIT_FAILURE);
     }

     if(write(fd,&monstre.armor,sizeof(unsigned short int))==-1){
          perror("Erreur d'écriture");                            
          exit(EXIT_FAILURE);
     }
// ECRITURE STRENGTH
    if(write(fd,&monstre.strength,sizeof(unsigned short int))==-1){
          perror("Erreur d'écriture");                            
          exit(EXIT_FAILURE);
    }
// ECRITURE SPEED
    if(write(fd,&monstre.speed,sizeof(unsigned short int))==-1){
          perror("Erreur d'écriture");                            
          exit(EXIT_FAILURE);
    }
// ECRITURE ATTACKSPEED
    if(write(fd,&monstre.attackspeed,sizeof(unsigned short int))==-1){
          perror("Erreur d'écriture");                            
          exit(EXIT_FAILURE);
    }
}

monstre_t charger_monstre(int fd){
monstre_t monstretoreturn;
int taille;
unsigned int health;
unsigned int armor;
unsigned int strength;
unsigned int speed;
unsigned int attackspeed;
char buffer[1024];
    if(read(fd,&taille,sizeof(int))==-1){
        perror("Erreur lecture taille");
        exit(EXIT_FAILURE);
    }

    if((read(fd,buffer,taille))==-1){
        perror("Erreur lecture");
        exit(EXIT_FAILURE);
    }
     if((read(fd,&health,sizeof(unsigned short int)))==-1){
        perror("Erreur lecture");
        exit(EXIT_FAILURE);
    }
     if((read(fd,&armor,sizeof(unsigned short int)))==-1){
        perror("Erreur lecture");
        exit(EXIT_FAILURE);
    }
     if((read(fd,&strength,sizeof(unsigned short int)))==-1){
        perror("Erreur lecture");
        exit(EXIT_FAILURE);
    }
     if((read(fd,&speed,sizeof(unsigned short int)))==-1){
        perror("Erreur lecture");
        exit(EXIT_FAILURE);
    }
    if((read(fd,&attackspeed,sizeof(unsigned short int)))==-1){
        perror("Erreur lecture");
        exit(EXIT_FAILURE);
    }
monstretoreturn=create_monster(buffer,health,armor,strength,speed,attackspeed);
return monstretoreturn;

}

void enregistrer_artefact(int fd,artefact_t artefact){
    int taille_att=0;
    int retour;
    taille_att=strlen(artefact.name)+1;

// ECRITURE TAILLE DU NOM
    if((retour=write(fd,&taille_att,sizeof(int)))==-1){
    perror("Erreur");
    exit(EXIT_FAILURE);
}

// ECRITURE NOM
     if(write(fd,artefact.name,sizeof(char)*taille_att)==-1){
          perror("Erreur d'écriture");                            
          exit(EXIT_FAILURE);
    }
// ECRITURE ARMOR
      if(write(fd,&artefact.health,sizeof(unsigned short int))==-1){
          perror("Erreur d'écriture");                            
          exit(EXIT_FAILURE);
     }

     if(write(fd,&artefact.armor,sizeof(unsigned short int))==-1){
          perror("Erreur d'écriture");                            
          exit(EXIT_FAILURE);
     }
// ECRITURE STRENGTH
    if(write(fd,&artefact.strength,sizeof(unsigned short int))==-1){
          perror("Erreur d'écriture");                            
          exit(EXIT_FAILURE);
    }
// ECRITURE SPEED
    if(write(fd,&artefact.speed,sizeof(unsigned short int))==-1){
          perror("Erreur d'écriture");                            
          exit(EXIT_FAILURE);
    }
// ECRITURE ATTACKSPEED
    if(write(fd,&artefact.attackspeed,sizeof(unsigned short int))==-1){
          perror("Erreur d'écriture");                            
          exit(EXIT_FAILURE);
    }
}

artefact_t charger_artefact(int fd){
artefact_t artefact;
int taille;
unsigned int health;
unsigned int armor;
unsigned int strength;
unsigned int speed;
unsigned int attackspeed;
char buffer[1024];
    if(read(fd,&taille,sizeof(int))==-1){
        perror("Erreur lecture taille");
        exit(EXIT_FAILURE);
    }

    if((read(fd,buffer,taille))==-1){
        perror("Erreur lecture");
        exit(EXIT_FAILURE);
    }
     if((read(fd,&health,sizeof(unsigned short int)))==-1){
        perror("Erreur lecture");
        exit(EXIT_FAILURE);
    }
     if((read(fd,&armor,sizeof(unsigned short int)))==-1){
        perror("Erreur lecture");
        exit(EXIT_FAILURE);
    }
     if((read(fd,&strength,sizeof(unsigned short int)))==-1){
        perror("Erreur lecture");
        exit(EXIT_FAILURE);
    }
     if((read(fd,&speed,sizeof(unsigned short int)))==-1){
        perror("Erreur lecture");
        exit(EXIT_FAILURE);
    }
    if((read(fd,&attackspeed,sizeof(unsigned short int)))==-1){
        perror("Erreur lecture");
        exit(EXIT_FAILURE);
    }
artefact=create_artefact(buffer,health,armor,strength,speed,attackspeed);
return artefact;

}
 
void enregistrer_carte(carte_t* carte,char* name_file){
int fd;

    if(name_file==NULL){
        perror("Erreur fichier non renseignée");
        exit(EXIT_FAILURE);
    }
    char* nom_file_extension=malloc(strlen(name_file)*sizeof(char));
    strcpy(nom_file_extension,name_file);
    printf("Enregistrement dans %s",nom_file_extension);

    if((fd=open(nom_file_extension,O_CREAT|O_TRUNC|O_RDWR,S_IRUSR|S_IWUSR))==-1){ /*  OPENING   */
          perror("Erreur ouverture fichier 1");                         /*  FILE  */
          exit(EXIT_FAILURE);
    }
   if(write(fd,carte->cases,800*sizeof(case_t))==-1){
       perror("Erreur ecriture fichier 1");                         /*  FILE  */
          exit(EXIT_FAILURE);
   };
    if(write(fd,&carte->nbmob,sizeof(int))==-1){
       perror("Erreur ecriture fichier 1");                         /*  FILE  */
          exit(EXIT_FAILURE);
    };
     if(write(fd,&carte->nbartefacts,sizeof(int))==-1){
       perror("Erreur ecriture fichier 1");                         /*  FILE  */
          exit(EXIT_FAILURE);
    };
   for(int i=0;i<20;i++){
       enregistrer_monstre(fd,carte->monstre[i]);
   }
    for(int i=0;i<20;i++){
       enregistrer_artefact(fd,carte->artefacts[i]);
   }

 
}

off_t enregistrer_new_sav_carte(carte_t* carte,int fd){
off_t k;
  if(lseek(fd,0,SEEK_END)==-1){
          perror("Déplacement fichier ");                         /*  FILE  */
          exit(EXIT_FAILURE);
  }    


   if(write(fd,carte->cases,800*sizeof(case_t))==-1){
       perror("Erreur ecriture fichier 1");                         /*  FILE  */
          exit(EXIT_FAILURE);
   };
    if(write(fd,&carte->nbmob,sizeof(int))==-1){
       perror("Erreur ecriture fichier 1");                         /*  FILE  */
          exit(EXIT_FAILURE);
    };
     if(write(fd,&carte->nbartefacts,sizeof(int))==-1){
       perror("Erreur ecriture fichier 1");                         /*  FILE  */
          exit(EXIT_FAILURE);
    };
   for(int i=0;i<20;i++){
       enregistrer_monstre(fd,carte->monstre[i]);
   }
    for(int i=0;i<20;i++){
       enregistrer_artefact(fd,carte->artefacts[i]);
   }
   if((k=lseek(fd,0,SEEK_END))==-1){
          perror("Déplacement fichier ");                         /*  FILE  */
          exit(EXIT_FAILURE);
  }
  
  return k;
}

void enregistrer_carte_emplacement_dans_fichier(carte_t* carte,int fd)
{
    
    
   if(write(fd,carte->cases,800*sizeof(case_t))==-1){
       perror("Erreur ecriture fichier 1");                         /*  FILE  */
          exit(EXIT_FAILURE);
   };
    if(write(fd,&carte->nbmob,sizeof(int))==-1){
       perror("Erreur ecriture fichier 1");                         /*  FILE  */
          exit(EXIT_FAILURE);
    };
     if(write(fd,&carte->nbartefacts,sizeof(int))==-1){
       perror("Erreur ecriture fichier 1");                         /*  FILE  */
          exit(EXIT_FAILURE);
    };
   for(int i=0;i<20;i++){
       enregistrer_monstre(fd,carte->monstre[i]);
   }
    for(int i=0;i<20;i++){
       enregistrer_artefact(fd,carte->artefacts[i]);
   }

}



carte_t charger_carte(char* name_file){

int fd;
carte_t macarte_load;

     if(name_file==NULL){
        perror("Erreur fichier non renseignée");
        exit(EXIT_FAILURE);
    }

    if((fd=open(name_file,O_RDWR,S_IRUSR|S_IWUSR))==-1){
         perror("Erreur ouverture fichier 1");
         exit(EXIT_FAILURE);
    }
    printf("Chargement de :%s\n",name_file);
   if(read(fd,macarte_load.cases,800*sizeof(case_t))==-1){
       perror("Erreur ecriture fichier 1");                         /*  FILE  */
          exit(EXIT_FAILURE);
   };
   
    if(read(fd,&macarte_load.nbmob,sizeof(int))==-1){
       perror("Erreur ecriture fichier 1");                         /*  FILE  */
          exit(EXIT_FAILURE);
    };
     if(read(fd,&macarte_load.nbartefacts,sizeof(int))==-1){
       perror("Erreur ecriture fichier 1");                         /*  FILE  */
          exit(EXIT_FAILURE);
    };

    for(int i=0;i<20;i++){
        macarte_load.monstre[i]=charger_monstre(fd);
    }
     for(int i=0;i<20;i++){
       macarte_load.artefacts[i]=charger_artefact(fd);
   }

return macarte_load;
}

carte_t charger_carte_monde_sav(char* name_file,int fd,off_t k){



carte_t macarte_load;



    if(lseek(fd,k,SEEK_SET)==-1){
          perror("Déplacement fichier ");                         /*  FILE  */
          exit(EXIT_FAILURE);
     }


   if(read(fd,macarte_load.cases,800*sizeof(case_t))==-1){
       perror("Erreur ecriture fichier 1");                         /*  FILE  */
          exit(EXIT_FAILURE);
   };
   
    if(read(fd,&macarte_load.nbmob,sizeof(int))==-1){
       perror("Erreur ecriture fichier 1");                         /*  FILE  */
          exit(EXIT_FAILURE);
    };
     if(read(fd,&macarte_load.nbartefacts,sizeof(int))==-1){
       perror("Erreur ecriture fichier 1");                         /*  FILE  */
          exit(EXIT_FAILURE);
    };

    for(int i=0;i<20;i++){
        macarte_load.monstre[i]=charger_monstre(fd);
    }
     for(int i=0;i<20;i++){
       macarte_load.artefacts[i]=charger_artefact(fd);
   }

return macarte_load;
}





carte_t charger_carte_default(){
  // FONCTION DE CREATION D'UNE CARTE PAR DEFAULT
  carte_t cartetoreturn;

  for(int i=0;i<40;i++){
    for(int j=0;j<20;j++){
      cartetoreturn.cases[i][j].code_couleur=GREEN;
      cartetoreturn.cases[i][j].elem='s';
      cartetoreturn.cases[i][j].indelem=-1;
    }
  }

  for(int i=0;i<20;i++){
      cartetoreturn.monstre[i]=create_monster("-1",0,0,0,0,0);
  }
   for(int i=0;i<20;i++){
      cartetoreturn.artefacts[i]=create_artefact("-1",0,0,0,0,0);
  }
  return cartetoreturn;
}
