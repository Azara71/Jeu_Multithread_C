#
# CONFIGURATION GENERALE (à configurer)
#

EXEC = client serveur editeur test
#On met les .o dans Objets, make depend à chaque fois
OBJETS = artefact.o monstre.o enregistrement.o fonction_ncurses.o carte.o hero.o
NOM_PROJET = 

#
# SUFFIXES (ne pas modifier)
#

.SUFFIXES: .c .o

#
# OBJETS (ne pas modifier)
#

EXEC_O = $(EXEC:=.o)
OBJETS_O = $(OBJETS) $(EXEC_O)

#
# ARGUMENTS ET COMPILATEUR (à configurer)
#

CC = gcc
CCFLAGS_STD = -Wall -O3 -Werror 
CCFLAGS_DEBUG = -D _DEBUG_
CCFLAGS = $(CCFLAGS_STD)
CCLIBS = -lncurses -lpthread

#
# REGLES (ne pas modifier)
# DANS LA REGLE JAI DU RAJOUTER UN LIBS
#

all: msg $(OBJETS) $(EXEC_O)
	@echo "Creation des executables..."
	@for i in $(EXEC); do \
	$(CC) -o $$i $$i.o  $(OBJETS) $(CCLIBS); \
	done
	@echo "Termine."

msg:
	@echo "Creation des objets..."

debug: CCFLAGS = $(CCFLAGS_STD) $(CCFLAGS_DEBUG)
debug: all

#
# REGLES PAR DEFAUT (ne pas modifier)
#

.c.o: .h
	@cd $(dir $<) && ${CC} ${CCFLAGS} -c $(notdir $<) -o $(notdir $@)

#
# REGLES GENERALES (ne pas modifier)
#

# Vous pouvez ajouter des commandes spécifiques à votre projet
clean:
	@echo "Suppresion des objets, des fichiers temporaires..."
	@rm -f $(OBJETS) $(EXEC_O)
	@rm -f *~ *#
	@rm -f $(EXEC)
	@rm -f dependances
	@echo "Termine."

depend:
	@echo "Creation des dependances..."
	@sed -e "/^# DEPENDANCES/,$$ d" makefile > dependances
	@echo "# DEPENDANCES" >> dependances
	@for i in $(OBJETS_O); do \
	$(CC) -MM -MT $$i $(CCFLAGS) `echo $$i | sed "s/\(.*\)\\.o$$/\1.c/"` >> dependances; \
	done
	@cat dependances > makefile
	@rm dependances
	@echo "Termine."

#
# CREATION ARCHIVE (ne pas modifier)
#

ARCHIVE_FILES = *

archive: clean
	@echo "Creation de l'archive $(NOM_PROJET)$(shell date '+%y%m%d.tar.gz')..."
	@REP=`basename "$$PWD"`; cd .. && tar zcf $(NOM_PROJET)$(shell date '+%y%m%d.tar.gz') $(addprefix "$$REP"/,$(ARCHIVE_FILES))
	@echo "Termine."

# DEPENDANCES
artefact.o: artefact.c artefact.h
monstre.o: monstre.c monstre.h
enregistrement.o: enregistrement.c fonction_ncurses.h monstre.h \
 artefact.h carte.h case.h entite.h
fonction_ncurses.o: fonction_ncurses.c fonction_ncurses.h
carte.o: carte.c carte.h case.h monstre.h entite.h artefact.h
hero.o: hero.c hero.h artefact.h
editeur.o: editeur.c fonction_ncurses.h monstre.h artefact.h carte.h \
 case.h entite.h enregistrement.h
serveur.o: serveur.c fonction_ncurses.h monstre.h artefact.h carte.h \
 case.h entite.h enregistrement.h hero.h
client.o: client.c fonction_ncurses.h monstre.h artefact.h carte.h case.h \
 entite.h enregistrement.h
