
# PROJET BOMBERMAN

Voici un explication du fonctionnement de notre projet Bomberman.

## Structure du Projet

Le projet est divisé en deux parties principales :
- **Client** : Contient tout le code nécessaire pour l'interface client du jeu.
- **Serveur** : Gère la logique du serveur pour le jeu.


## Compilation

Le projet utilise un `Makefile` dans le répertoire racine qui invoque les `Makefile` dans les sous-dossiers `client` et `serveur`.

1. Ouvrez un terminal dans le répertoire racine du projet.
2. Exécutez la commande suivante pour compiler le projet :
   ```bash
   make
   ```

## Lancement du Serveur

Une fois la compilation terminée, vous pouvez lancer le serveur avec la commande suivante :

```bash
make server_run
```
et un client avec la commande suivante :
```bash
make client_run
```

Assurez-vous que le serveur est en cours d'exécution avant de lancer les instances du client.

Les propriétés du jeu (nombre de joueurs dans une partie, dimensions de la grille) sont disponibles dans le fichier `serveur/globals_serv.h`.

## Jouer

Lorsque le client se connecte, il doit entrer le mode de jeu qu'il souhaite rejoindre :
- 1 - Mode Chacun pour soi
- 2 - Mode 2v2
- p ou P - Quitter la partie

Une fois le choix fait, le client doit se déclarer prêt. Il a 60 secondes pour envoyer n'importe quel message avant de se faire sortir de la partie par le serveur.


## Touches
- **Se déplacer :** Flèches haut, bas, gauche, droite
- **Poser une bombe :** F1
- **Ecrire un message dans le tchat :** Ecrire son message
- **Envoyer son message :** F4


## Arrêt

Pour arrêter le serveur ou un client, utilisez `Ctrl+C` dans les terminaux correspondants.

## Plus d'Informations

Voici les structures des 2 sous-dossiers du projet :

![](architecture_client.png)

![](architecture_serveur.png)


## Auteurs
- Yanis ALLAL
- Antony LEHMANN
- Félix OLLIVIER