#include "case.h"
typedef struct carte_type{

    case_t cases[40][20];
    monstre_t monstre[20];
    artefact_t artefacts[20];
    int nbmob;
    int nbartefacts;

}carte_t;
/*
typedef struct element_carte{
    int x;
    int y;
    char* nom_carte;
    struct element_carte* suivant;
}element_carte_t;

typedef struct liste_carte{
    element_carte_t *premier;
}liste_carte_t;
*/
typedef struct carte_chainee{
    int x;
    int y;
    char* nom_carte;
    struct carte_chainee *suivant;
    struct carte_chainee *prec;
}carte_chainee_t;

typedef struct liste_carte{
     size_t length;
     carte_chainee_t *queue;
     carte_chainee_t *tete;
} liste_carte_t;

liste_carte_t *init_liste_carte();
void afficher_liste_carte(liste_carte_t *liste);
liste_carte_t *inserer_liste(liste_carte_t *liste, int x,int y, char* nom_map);
liste_carte_t *remove_map_from_list(liste_carte_t *liste,char* nom_map);
void afficher_carte(WINDOW* fen_affichage,carte_t carte);
void insertion(liste_carte_t *liste, int x, int y, char* nom_map);