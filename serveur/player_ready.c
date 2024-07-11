#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/select.h>
#include "globals_serv.h"
#include "requetes_from_serv.h"


void player_ready_4j(int clientfd, uint8_t *cases, modifiedcase *casesmodified, int *modified){
    ready_4p++;
    if(ready_4p==NB_JOUEURS_4J){
        puts("Tous les clients 4j sont prêts, lancement de la partie");
        launch_game_4p(cases, casesmodified, modified);
    }else{
        printf("%d clients prêts, attente des autres\n", ready_4p);
    }
}

void player_ready_2v2(int clientfd, uint8_t *cases, modifiedcase *casesmodified, int *modified){
    ready_2v2++;
    if(ready_2v2==NB_JOUEURS_2V2){
        puts("Tous les clients 2v2 sont prêts, lancement de la partie");
        launch_game_2v2(cases, casesmodified, modified);
    }else{
            printf("%d clients prêts, attente des autres\n", ready_2v2);
    }
}
