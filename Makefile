# Variables générales
CC = gcc
CFLAGS = -Wall -g -pthread
LDFLAGS = -lncurses -pthread

# Cibles phony pour la clarté des intentions
.PHONY: all clean client_run server_run mcli mserv

# Compilation de tout le projet
all: mcli mserv

# Compilation du client
mcli:
	$(MAKE) -C client all

# Compilation du serveur
mserv:
	$(MAKE) -C serveur all

# Nettoyage des fichiers compilés dans les deux sous-répertoires
clean:
	$(MAKE) -C client clean
	$(MAKE) -C serveur clean

# Exécuter le client et le serveur
client_run:
	$(MAKE) -C client run

server_run:
	$(MAKE) -C serveur run
