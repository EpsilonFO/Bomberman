#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>
#include <poll.h>
#include <netinet/in.h>
#include "requetes_from_serv.h"
#include "globals_serv.h"
#include "../client/globals_client.h"

#define TIMEOUT 10000  // Timeout en millisecondes pour poll

int close_thread = 0;

void handle_tcp(int clientfd, uint8_t *cases, modifiedcase *casesmodified, int *modified) {
    uint8_t buffer[1024];
    memset(buffer, 0, sizeof(buffer));
    int rt = recv(clientfd, buffer, sizeof(buffer), 0);
    if (rt <= 0) {
        perror("Erreur lors de la réception des données TCP");
        close(clientfd);
        close_thread = 1;
        nb_players_4p--;
        return;
    }
    uint16_t code = ntohs(*(uint16_t*)buffer);
    printf("codereq_tcp : %d\n", code);

    switch(code){
        case 1:
            add_to_4j(clientfd);
            break;
        case 2:
            add_to_2v2(clientfd);
            break;
        case 3:
            player_ready_4j(clientfd, cases, casesmodified, modified);
            break;
        case 4:
            player_ready_2v2(clientfd, cases, casesmodified, modified);
            break;
        case 7:
            send_message(buffer, clientfd);
            break;
        case 8:
            // send_msg_mate();
            break;
        case 21:
            nb_players_4p--;
            nb_players_2v2--;
            break;

    }
}

void handle_udp(int sock_udp, uint8_t *cases, modifiedcase *casesmodified, pos **posJoueurs, int *modified, int clientfd) {
    uint8_t buffer[1024];
    struct sockaddr_in6 src_addr;
    socklen_t addr_len = sizeof(src_addr);
    memset(buffer, 0, sizeof(buffer));
    int rt = recvfrom(sock_udp, buffer, sizeof(buffer), 0, (struct sockaddr *)&src_addr, &addr_len);
    if (rt < 0) {
        perror("Erreur lors de la réception des données UDP");
        return;
    }
    if(rt == 0){
        puts("Un client s'est déconnecté.");
        nb_players_4p--; // A changer car pas forcément une partie 4 joueurs
        return;
    }
    uint16_t code = ntohs(*(uint16_t*)buffer);
    printf("codereq_udp : %d\n", code);
    switch(code){
        case 5:
            *modified = action_game(buffer, cases, casesmodified, posJoueurs, clientfd);
            break;
        case 6:
            // action_2v2(buffer);
            break;
        default:
            break;
    }
}

void game(int clientfd, uint8_t* cases) {
    struct pollfd fds[2];
    int ret;

    modifiedcase* casesmodified = malloc(sizeof(modifiedcase)*WIDTH*HEIGHT);
    pos *posJoueurs[4];
    int modified = 0;
    //Position des joueurs
    posJoueurs[0] = malloc(sizeof(pos)); posJoueurs[0] -> x = 0; posJoueurs[0] -> y = 0; 
    posJoueurs[1] = malloc(sizeof(pos)); posJoueurs[1] -> x = HEIGHT - 1; posJoueurs[1] -> y = 0;
    posJoueurs[2] = malloc(sizeof(pos)); posJoueurs[2] -> x = 0; posJoueurs[2] -> y = WIDTH - 1;
    posJoueurs[3] = malloc(sizeof(pos)); posJoueurs[3] -> x = HEIGHT - 1; posJoueurs[3] -> y = WIDTH - 1;

    fds[0].fd = clientfd;
    fds[0].events = POLLIN;
    fds[1].fd = sock_udp;
    fds[1].events = POLLIN;

    while (1) {
        ret = poll(fds, 2, TIMEOUT);

        if (ret < 0) {
            perror("poll");
            break;
        }
        if (ret == 0) {
            continue;
        }

        if (fds[0].revents & POLLIN) {
            handle_tcp(clientfd, cases, casesmodified, &modified);
        }
        if (fds[1].revents & POLLIN) {
            handle_udp(sock_udp, cases, casesmodified, posJoueurs, &modified, clientfd);
        }
    }
    // pthread_exit(NULL);
}
