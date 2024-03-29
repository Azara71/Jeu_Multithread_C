#include "case.h"
typedef struct carte_type{

    case_t cases[40][20];
    monstre_t monstre[20];
    artefact_t artefacts[20];
    int nbmob;
    int nbartefacts;
    int nb_joueur;


}carte_t;

typedef struct carte_chainee{
    int x;
    int y;
    pthread_t id_thread_monstre[20];
    carte_t carte;
    struct carte_chainee *suivant;
    struct carte_chainee *prec;
}carte_chainee_t;

typedef struct liste_carte{
     size_t length;
     carte_chainee_t *queue;
     carte_chainee_t *tete;
} liste_carte_t;

typedef struct node{

    int x;
    int y;
    off_t emplacement_carte;

}node_t;

typedef struct tabnodes{
     int length_total;
     int length;
     node_t nodes[5];
     struct tabnodes *tab_suivant;
     off_t mon_endroit;
     int suivant;
} tabnodes_t;

int generer_nombre_aleatoire(int nb_max);
void afficher_carte(WINDOW* fen_affichage,carte_t carte);
/*
* LISTE CARTE
*/
void *thread_monstre(void *arg);
void lancer_thread_monstre(liste_carte_t *liste_carte_active,int x,int y);
void eteindre_thread_monstre(liste_carte_t *liste,int x,int y);
liste_carte_t *init_liste_carte();
void afficher_liste_carte(liste_carte_t *liste);
liste_carte_t *inserer_liste(liste_carte_t *liste, int x,int y,carte_t carteainserer);
liste_carte_t *remove_map_from_list(liste_carte_t *liste,int x,int y);
void insertion(liste_carte_t *liste, int x, int y);
int chercher_map_from_list(liste_carte_t *liste, int x, int y,carte_t carte);
carte_t map_from_list(liste_carte_t *liste, int x, int y);
liste_carte_t* mettre_a_jour_map_in_list(liste_carte_t *liste, int x, int y,carte_t carte,int world_descriptor);
void mettre_tresor_alea(liste_carte_t *liste);

/*
* TABNODES DE CARTE
*/
tabnodes_t *init_tabnodes_t();
tabnodes_t *inserer_tab_nodes(tabnodes_t *tabnode, node_t node);
void afficher_tabnodes_t(tabnodes_t *tabnode);
off_t trouver_emplacement_par_tabnodes(tabnodes_t *tabnode,int x,int y);
tabnodes_t* lire_tab_nodes_dans_fichier(int fd);
void ecrire_tab_nodes_dans_fichier(tabnodes_t *tabnode,int fd,int k);
tabnodes_t *remplacer(tabnodes_t *tabnode, node_t node,int num_node);
void rajouter_carte_monde_sav(int world_descriptor,tabnodes_t* l_carte,carte_t carte_a_envoyer,int x,int y);
int dans_tabnodes(tabnodes_t *tabnode,int x,int y);
