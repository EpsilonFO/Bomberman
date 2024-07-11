# Variables pour le compilateur
CC = gcc
CFLAGS = -Wall -g -pthread  # Add -pthread for compilation
LDFLAGS = -lncurses -pthread  # Add -pthread for linking

# Nom de l'exécutable
TARGET = bomberman_client

# Fichiers source
SOURCES = main_client.c connexion_client.c choose_mode.c abo_multidiff.c update_grill.c move.c casemodifier.c ready.c killed.c end_game.c ncurses.c
OBJECTS = $(SOURCES:.c=.o)

# Règle par défaut
all: $(TARGET)

# Lien des fichiers objets en exécutable
$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

# Compilation des fichiers source en fichiers objets
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Nettoyage des fichiers générés
clean:
	rm -f $(TARGET) $(OBJECTS)

# Pour 'make run'
run: $(TARGET)
	./$(TARGET)

# Pour vérifier la dépendance des en-têtes
depend: .depend

.depend: $(SOURCES)
	rm -f ./.depend
	$(CC) $(CFLAGS) -MM $^ >> ./.depend;

include .depend
