#include <stdlib.h>
#include "requetes_from_serv.h"
#include "../client/globals_client.h"
#include "../client/ncurses.h"
#include "globals_serv.h"
#include <pthread.h>
#include <stdio.h>

pthread_mutex_t verrou;

int action_game(u_int8_t *buffer, uint8_t *cases, modifiedcase *casesmodified, pos **posJoueurs, int clientfd) {
    pthread_mutex_t verrou = PTHREAD_MUTEX_INITIALIZER;
    pthread_mutex_t lockmodif = PTHREAD_MUTEX_INITIALIZER;
    action *act = (action*) buffer;
    int xd = 0;
    int yd = 0;
    int modified = 0;
    int tempx = posJoueurs[act->ID]->x;
    int tempy = posJoueurs[act->ID]->y;

    switch (act->ACTION) {
        case 3:  // Left
            xd = -1; yd = 0; break;
        case 1:  // Right
            xd = 1; yd = 0; break;
        case 0:  // Up
            xd = 0; yd = -1; break;
        case 2:  // Down
            xd = 0; yd = 1; break;
        case 4:
            modified += 1;
            int index = tempy * WIDTH + tempx;
            cases[index] = 3;
            modifiedcase cm = {tempx, tempy, 3};
            casesmodified[modified] = cm;
            pose_bomb(cases, casesmodified, tempx, tempy, &modified, lockmodif, clientfd);
            return modified;
        default: break;
    }

    pthread_mutex_lock(&verrou);
    posJoueurs[act->ID]->x += xd;
    posJoueurs[act->ID]->y += yd;

    // Assurer que les nouvelles coordonnées sont dans les limites
    posJoueurs[act->ID]->x = (posJoueurs[act->ID]->x + HEIGHT) % HEIGHT;
    posJoueurs[act->ID]->y = (posJoueurs[act->ID]->y + WIDTH) % WIDTH;

    int new_index = posJoueurs[act->ID]->y * WIDTH + posJoueurs[act->ID]->x;
    int old_index = tempy * WIDTH + tempx;
    if (cases[new_index] == 0) {
        cases[new_index] = act->ID + 5;
        if (cases[old_index] != 3) {
            cases[old_index] = 0;
        }
        modifiedcase cm = {posJoueurs[act->ID]->x, posJoueurs[act->ID]->y, act->ID + 5};
        casesmodified[modified] = cm;
        modifiedcase cm2 = {tempx, tempy, 0};
        casesmodified[modified + 1] = cm2;
        modified += 2;
    } else {
        posJoueurs[act->ID]->x = tempx;
        posJoueurs[act->ID]->y = tempy;
    }
    pthread_mutex_unlock(&verrou);
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            printf("%d ", cases[i * WIDTH + j]);
        }
        printf("\n");
    }
    puts("Positions des joueurs : ");
    for (int i = 0; i < 4; i++) {
        printf("Joueur %d -> x: %d, y: %d\n", i, posJoueurs[i]->x, posJoueurs[i]->y);
    }
    pthread_mutex_destroy(&verrou);

    return modified;
}
