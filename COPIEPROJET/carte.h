#include "case.h"
typedef struct carte_type{

    case_t cases[40][20];
    monstre_t monstre[20];
    artefact_t artefacts[20];
    int nbmob;
    int nbartefacts;

}carte_t;

void afficher_carte(WINDOW* fen_affichage,carte_t carte);