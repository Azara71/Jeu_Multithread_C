#ifndef _ENREGISTREMENT_
#define _ENREGISTREMENT_

void enregistrer_case(int fd,case_t* casetowrite);
case_t charger_case(int fd);
void enregistrer_carte(carte_t* carte,char* name_file);
off_t enregistrer_new_sav_carte(carte_t* carte,char* name_file,int fd);
carte_t charger_carte_monde_sav(char* name_file,int fd,off_t k);
carte_t charger_carte(char* name_file);
carte_t charger_carte_default();
#endif