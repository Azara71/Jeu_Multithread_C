#include "case.h"
typedef struct carte_type{

    case_t cases[40][20];
    monstre_t monstre[20];
    artefact_t artefacts[20];
    int nbmob;
    int nbartefacts;

}carte_t;

typedef struct carte_chainee{
    int x;
    int y;
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
     int length;
     node_t nodes[5];
     struct tabnodes *tab_suivant;
     off_t endroit_liste_suivante;
} tabnodes_t;


void afficher_carte(WINDOW* fen_affichage,carte_t carte);
/*
* LISTE CARTE
*/
liste_carte_t *init_liste_carte();
void afficher_liste_carte(liste_carte_t *liste);
liste_carte_t *inserer_liste(liste_carte_t *liste, int x,int y);
liste_carte_t *remove_map_from_list(liste_carte_t *liste,int x,int y);
void insertion(liste_carte_t *liste, int x, int y);
/*
* TABNODES DE CARTE
*/
tabnodes_t *init_tabnodes_t();
tabnodes_t *inserer_tab_nodes(tabnodes_t *tabnode, node_t node);
void afficher_tabnodes_t(tabnodes_t *tabnode);
