#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include "globals_serv.h"
#include "../client/ncurses.h"

#define FREQ 100

typedef struct {
    uint8_t *current;
    modifiedcase *modified;
    int *modif;
    int game;
} cm;

void send_grill_freq(mdiffmodif *mdm, int eq, struct sockaddr_in6 mdiff, int modif){
    if (sendto(sock_udp, mdm, sizeof(mdiffmodif) + modif * sizeof(modifiedcase), 0, (struct sockaddr *)&mdiff, sizeof(mdiff)) < 0) {
        perror("Erreur lors de l'envoi des données multicast");
        free(mdm);
    }
}

int send_grille(mdiffgrille *mdg, int eq){
        size_t message_size = sizeof(mdiffgrille) + mdg -> HAUTEUR * mdg -> LARGEUR * sizeof(uint8_t);
        if(eq){
            if (sendto(sock_udp, mdg, message_size, 0, (struct sockaddr *)&multicastAddr_2v2, sizeof(multicastAddr_2v2)) < 0) {
                perror("Erreur lors de l'envoi des données multicast");
                free(mdg);
                return 1;
            }
        }else{
            if (sendto(sock_udp, mdg, message_size, 0, (struct sockaddr *)&multicastAddr_4j, sizeof(multicastAddr_4j)) < 0) {
                perror("Erreur lors de l'envoi des données multicast");
                free(mdg);
                return 1;
            }
        }
        free(mdg);
        return 0;
}
void init_grille(mdiffgrille *mdg){
    srand(time(NULL));
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            int randomValue = rand() % 100;
            if (randomValue < 90) {
                mdg -> CASES[i * HEIGHT + j] = (uint8_t)0;
            } else if (randomValue < 95) {
                mdg -> CASES[i * HEIGHT + j] = (uint8_t)2;
            } else {
                mdg -> CASES[i * HEIGHT + j] = (uint8_t)1;
            }
        }
    }
    mdg->CASES[0] = (uint8_t)5;
    mdg->CASES[(HEIGHT - 1)] = (uint8_t)6;
    mdg->CASES[(WIDTH - 1) * HEIGHT] = (uint8_t)7;
    mdg->CASES[(WIDTH - 1) * HEIGHT + (HEIGHT - 1)] = (uint8_t)8;

}

void copy_grille(uint8_t *cases, uint8_t *mdg){
    for (int i = 0; i < WIDTH; i++) {
        for (int j = 0; j < HEIGHT; j++) {
            mdg[i * HEIGHT + j] = cases[i * HEIGHT + j]; 
        }
    }
}

void *multicast_sender(void *args) {
    cm *arguments = (cm *)args;
    uint8_t *cases = arguments -> current;
    modifiedcase *casesmodified = arguments -> modified;
    int num_msg_mdiff = 1;
    int init = 1;
    while (1) {
        // Allocation correcte de la structure mdiffgrille avec espace pour CASES
        mdiffgrille *mdg = malloc(sizeof(mdiffgrille) + HEIGHT * WIDTH * sizeof(uint8_t));
        if (mdg == NULL) {
            perror("Erreur lors de l'allocation mémoire");
            break;
        }
        memset(mdg, 0, sizeof(mdiffgrille) + HEIGHT * WIDTH * sizeof(uint8_t));  // Initialiser la mémoire avec des zéros
        mdg->CODEREQ = htons(11);
        mdg->ID = 0;
        mdg->EQ = 0;
        mdg->NUM = htons(1);
        mdg->HAUTEUR = HEIGHT;
        mdg->LARGEUR = WIDTH;
        if(init){
            init_grille(mdg);
            copy_grille(mdg -> CASES, cases);
            if(send_grille(mdg, arguments -> game)) break;
            init = 0;
            num_msg_mdiff++;
            continue;
        }else{
            for(int i=0;i<50;++i){
                usleep(20*1000);
                mdiffmodif *mdm = malloc(sizeof(mdiffmodif) + *(arguments -> modif) * sizeof(modifiedcase));
                memset(mdm, 0, sizeof(mdiffmodif) + *(arguments -> modif) * sizeof(modifiedcase));  // Initialiser la mémoire avec des zéros
                mdm -> CODEREQ = htons(12);
                mdm -> ID = 0;
                mdm -> EQ = 0;
                mdm -> NUM = htons(num_msg_mdiff);
                mdm -> NB = *(arguments -> modif);
                for(int j=0;j<*(arguments -> modif);++j){
                    mdm -> CASES[j].line = casesmodified[j].line;
                    mdm -> CASES[j].column = casesmodified[j].column;
                    mdm -> CASES[j].content = casesmodified[j].content;
                }
                if(arguments -> game) send_grill_freq(mdm, arguments -> game, multicastAddr_2v2, *(arguments -> modif));
                else send_grill_freq(mdm, arguments -> game, multicastAddr_4j, *(arguments -> modif));
                *(arguments -> modif) = 0;
                num_msg_mdiff++;
            }
            puts("fin de usleep");
            copy_grille(cases, mdg -> CASES);
            if(send_grille(mdg, arguments -> game))break;
        }
        num_msg_mdiff++;  // Envoi toutes les secondes
    }
    pthread_exit(NULL);
}


void launch_game_4p(uint8_t *casescurrent, modifiedcase *casesmodified, int *modified){
    cm *arguments = malloc(sizeof(cm));
    arguments -> current = casescurrent;
    arguments -> modified = casesmodified;
    arguments -> modif = modified;
    arguments -> game = 0;
    pthread_t multicast_thread;
    pthread_create(&multicast_thread, NULL, multicast_sender, arguments);
}


void launch_game_2v2(uint8_t *casescurrent, modifiedcase *casesmodified, int *modified){
    cm *arguments = malloc(sizeof(cm));
    arguments -> current = casescurrent;
    arguments -> modified = casesmodified;
    arguments -> modif = modified;
    arguments -> game = 1;
    pthread_t multicast_thread;
    pthread_create(&multicast_thread, NULL, multicast_sender, arguments);
}