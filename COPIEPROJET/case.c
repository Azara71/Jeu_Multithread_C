#include "case.h"



case_t create_case(int code,char elem){

case_t casetoreturn;
casetoreturn.code_couleur=code;
casetoreturn.elem=elem;
casetoreturn.indelem=-1;
return casetoreturn;
}
