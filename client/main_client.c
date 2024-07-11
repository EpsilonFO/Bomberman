#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <poll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <ncurses.h>
#include "globals_client.h"
#include "../serveur/globals_serv.h"
#include "../serveur/requetes_from_serv.h"
#include "connexion_client.h"
#include "requetes_to_serv.h"

#define TIMEOUT 10000  // Durée du timeout en millisecondes

int sockc_udp, sockc_tcp, id, eq, in_game, num_recv_server;
board *b;
line *l, *ch;
pos *p;
struct sockaddr_in6 src_addr;

void end() {
    close(sockc_tcp);
    close(sockc_udp);
    endwin();
    if(b!=NULL) free_board(b);
    if(l!=NULL) free(l);
    if(p!=NULL) free(p);
    if(ch!=NULL) free(ch);
    exit(0);
}

void handle_tcp() {
    uint8_t buffer[1024];
    int r = recv(sockc_tcp, buffer, sizeof(buffer), 0);
    if (r < 0) {
        perror("Erreur lors de la réception du message TCP");
        end();
    }
    if (r == 0) {
        printf("Le serveur TCP a fermé la connexion.\n");
        end();
    }
    uint16_t code = ntohs(*(uint16_t*)buffer);
    int *info;
    switch (code) {
        case 9:
            info = abo_multidiff(buffer);
            id = info[0];
            eq = info[1];
            printf("Partie 4 joueurs, id : %d, équipe : %d\n", id, eq);
            break;
        case 10:
            info = abo_multidiff(buffer);
            id = info[0];
            eq = info[1];
            printf("Partie 2v2, id : %d, équipe : %d\n", id, eq);
            break;
        case 13: {
            msg_toserv *msg = (msg_toserv *)buffer;
            msg->DATA[msg->LEN] = '\0'; // Assurez-vous que la chaîne est terminée par un null

            // Calculer la taille nécessaire pour cpy
            size_t cpy_size = strlen(ch->data) + 2; // +1 pour l'espace et +1 pour le null terminator
            char *cpy = malloc(cpy_size);
            if (cpy == NULL) {
                perror("Erreur d'allocation mémoire pour cpy");
                exit(EXIT_FAILURE);
            }

            sprintf(cpy, "%s " , ch->data);

            // Calculer la taille nécessaire pour ch->data après la concaténation
            size_t new_data_size = strlen(cpy) + strlen(msg->DATA) + 20; // +20 pour "Joueur X : " et le '\n'
            if (new_data_size >= TEXT_SIZE) {
                fprintf(stderr, "Erreur: le message est trop long pour ch->data\n");
                free(cpy);
                exit(EXIT_FAILURE);
            }

            sprintf(ch->data, "Joueur %d : %s\n%s", msg->ID + 1, msg->DATA, cpy);
            ch->cursor = strlen(ch->data); // Initialiser correctement le curseur à la fin de la chaîne

            free(cpy); 
            break;
        }
        case 15:
            end_game_4j(buffer);
            break;
        case 16:
            end_game_2v2(buffer);
            break;
        case 20: {
            killed(buffer);
            break;
        }
        default:
            break;
    }
}
void handle_udp() {
    uint8_t buffer[1024];
    socklen_t addr_len = sizeof(src_addr);
    int r = recvfrom(sockc_udp, buffer, sizeof(buffer), 0, (struct sockaddr *)&src_addr, &addr_len);
    if (r < 0) {
        perror("Erreur lors de la réception du message UDP");
        end();
    }
    if (r == 0) {
        printf("Le serveur TCP a fermé la connexion.\n");
        end();
    }
    uint16_t code = ntohs(*(uint16_t*)buffer);
    switch (code) {
        case 11:
            update_grill(buffer);
            in_game = 1;
            break;
        case 12:
            case_modifier(buffer);
            break;
        default:
            break;
    }
}

int main() {
    game_initialized = 0;
    if (connect_to_serv() == -1) exit(EXIT_FAILURE);

    struct pollfd fds[2];
    int ret;

    // Initialisation des structures pollfd
    fds[0].fd = sockc_tcp;
    fds[0].events = POLLIN;
    fds[1].fd = sockc_udp;
    fds[1].events = POLLIN;

    b = malloc(sizeof(board));
    l = malloc(sizeof(line));
    p = malloc(sizeof(pos));
    ch = malloc(sizeof(line));
    memset(ch->data, 0, sizeof(ch->data));
    ch->cursor = 0;

    uint8_t a;

    while (1) {
        ret = poll(fds, 2, TIMEOUT);
        a = control(l);
        if (ret < 0) {
            perror("poll");
            return -1;
        }
        if (!ret && in_game) {
            puts("Le serveur a fermé la connexion.");
            end();
        }

        if (fds[0].revents & POLLIN) {
            handle_tcp();
        }

        if(a != 255 && in_game){
            move_4j(a);
        }
        if (fds[1].revents & POLLIN) {
            handle_udp();
        }
    }
    return 0;
}
