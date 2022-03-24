#include <stdio.h>
#include "monstre.h"
#include "artefact.h"

typedef union entite_type{

monstre_t monstre;
artefact_t artefact;
char tresor_ou_obstacle;


}entite_t;



