# ----------------------------- #
# --------- Makefile ---------- #
# ----------------------------- #

# Projet Sudoku IN201
# Copyright (c) 2013
# Simon Manchel et Woody Rousseau

# -- Liste des Fichiers ---------
FILE = main.c

# -- Chemins --------------------
SRC_PATH = src
OBJ_PATH = obj
EXE_PATH = exe
INC_PATH = include

# -- Macros ---------------------
CC = gcc

# -- Flags ----------------------
CFLAGS = -ansi -posix -pedantic -Wall -std=c99

# -- Exécutable -----------------
PRODUCT = sudoku

# -- Liste des src et obj -------
SRC = $(addprefix ${SRC_PATH}/, $(FILE))
OBJ = $(addprefix ${OBJ_PATH}/, $(addsuffix .o, $(basename $(FILE))))

# -- Règles principales ---------
$(OBJ_PATH)/%.o: $(SRC_PATH)/%.c
	@mkdir -p $(OBJ_PATH)
	$(CC) $(CFLAGS) -c $< -o $@

$(EXE_PATH)/$(PRODUCT): $(OBJ)
	@mkdir -p $(EXE_PATH)
	$(CC) -o $@ $^ $(INC) $(LIB)
	@echo "Compilation Effectuée. Entrer 'make run' pour lancer le programme."

# -- Autres Règles --------------
clean:
	rm -rf *~
	rm -rf .DS_Store
	@echo "Nettoyage des fichiers temporaires et des .DS_Store effectué." 
	@echo "Entrer 'make mrproper' pour nettoyer les fichiers obj."

mrproper: clean
	rm -rf $(OBJ)
	@echo "Nettoyage des fichiers temporaires et des .DS_Store effectué." 

tar:
	tar -cvf sudoku.tar Makefile src include
	gzip -9 sudoku.tar

run:
	@./$(EXE_PATH)/$(PRODUCT)

help:
	@echo "'make' : Compilation du Programme"
	@echo "'make clean' : Nettoyage des fichiers *~ et .DS_Store"
	@echo "'make mrproper' : 'make clean' et nettoyage du dossier obj"
	@echo "'make tar' : Création d'un tarball compressé via gzip"
	@echo "'make run' : Lancement du programme"
	@echo "'make help' : Voir cette aide"
