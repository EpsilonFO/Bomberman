#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <net/if.h>
#include "globals_serv.h"
#include "requetes_from_serv.h"
#include "../client/globals_client.h"


void add_to_4j(int clientfd){
    gameSetup *game = malloc(sizeof(gameSetup));
    game -> CODEREQ = htons(9);
    game -> EQ = -1;
    game -> PORTUDP = htons(PORT_UDP);
    game -> PORTMDIFF = htons(PORT_MDIFF_4J);
    if(inet_pton(AF_INET6, ADDR_MDIFF_4J, game -> ADRMDIFF)<=0){
        perror("inet_pton");
        close(clientfd);
        return;
    }

    // Configuration de l'interface pour le socket UDP
    int ifindex = if_nametoindex(MULTICAST);
    if (ifindex == 0) perror("if_nametoindex");
    multicastAddr_4j.sin6_scope_id = ifindex;

    if(nb_games_4p%4==0){
        nb_games_4p++;
        game -> ID = nb_players_4p;
        puts("Création d'une nouvelle partie de Chacun pour soi.");
        ssize_t bytes_sent = send(clientfd, game, sizeof(gameSetup), 0);
        if (bytes_sent < 0) {
            perror("send");
            free(game);
            close(clientfd);
            return;
        } else if (bytes_sent != sizeof(gameSetup)) {
            fprintf(stderr, "Sent incomplete gameSetup structure.\n");
            return;
        }
    }else{
        nb_games_4p++;
        game -> ID = nb_players_4p;
        ssize_t bytes_sent = send(clientfd, game, sizeof(gameSetup), 0);
        if (bytes_sent < 0) {
            perror("send");
            free(game);
            close(clientfd);
            return;
        } else if (bytes_sent != sizeof(gameSetup)) {
            fprintf(stderr, "Sent incomplete gameSetup structure.\n");
            return;
        }
    }
    nb_players_4p++;
}

void add_to_2v2(int clientfd){
    gameSetup *game = malloc(sizeof(gameSetup));
    game -> CODEREQ = htons(10);
    game -> EQ = nb_players_2v2%2;
    game -> PORTUDP = htons(PORT_UDP);
    game -> PORTMDIFF = htons(PORT_MDIFF_2V2);
    if(inet_pton(AF_INET6, ADDR_MDIFF_2V2, game -> ADRMDIFF)<=0){
        perror("inet_pton");
        close(clientfd);
        return;
    }
    
    // Configuration de l'interface pour le socket UDP
    int ifindex = if_nametoindex(MULTICAST);
    if (ifindex == 0) perror("if_nametoindex");
    multicastAddr_2v2.sin6_scope_id = ifindex;

    if(nb_games_2v2%4==0){
        nb_games_2v2++;
        game -> ID = nb_players_2v2;
        puts("Création d'une nouvelle partie de 2v2.");
        ssize_t bytes_sent = send(clientfd, game, sizeof(gameSetup), 0);
        if (bytes_sent < 0) {
            perror("send");
            free(game);
            close(clientfd);
            return;
        } else if (bytes_sent != sizeof(gameSetup)) {
            fprintf(stderr, "Sent incomplete gameSetup structure.\n");
            return;
        }
    }else{
        nb_games_2v2++;
        game -> ID = nb_players_2v2;
        ssize_t bytes_sent = send(clientfd, game, sizeof(gameSetup), 0);
        if (bytes_sent < 0) {
            perror("send");
            free(game);
            close(clientfd);
            return;
        } else if (bytes_sent != sizeof(gameSetup)) {
            fprintf(stderr, "Sent incomplete gameSetup structure.\n");
            return;
        }
    }
    nb_players_2v2++;
}