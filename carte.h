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
    struct carte_chainee *suivant;
    struct carte_chainee *prec;
}carte_chainee_t;

typedef struct node{

    int x;
    int y;
    off_t emplacement_carte;

}node_t;

typedef struct tab_nodes{
    int nb_nodes;
    node_t nodes[5];
    off_t tab_suivant;
    struct tab_nodes* ptr_tab_node_suivante;

}tab_nodes_t;


typedef struct liste_carte{
     int length;
     node_t nodes[5];
     struct liste_carte *liste_suivante;
     off_t endroit_liste_suivante;
} liste_carte_t;



liste_carte_t *init_liste_carte();
void afficher_liste_carte(liste_carte_t *liste);
liste_carte_t *inserer_liste(liste_carte_t *liste, node_t node);
void afficher_carte(WINDOW* fen_affichage,carte_t carte);
void insertion(liste_carte_t *liste, int x, int y, char* nom_map);
