#include <stdlib.h>
#include "globals_serv.h"
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>

typedef struct {
    uint8_t *c;
    modifiedcase *casemodified;
    int x;
    int y;
    int *modified;
    int clientfd;
    pthread_mutex_t lock;
} ar;

void announce_kill(uint8_t *cases, int x, int y, int clientfd){
    endgame *eg = malloc(sizeof(endgame));
    eg -> CODEREQ = htons(20);
    eg -> ID = cases[y*WIDTH + x] - 5;
    printf("on va tuer %d, cases vaut %d\n", eg -> ID, cases[(y)*WIDTH + x]);
    if(send(clientfd, eg, sizeof(endgame), 0)<=0){
        perror("Erreur lors de l'envoi de l'annonce de mort");
    }
    ready_4p--;
    if(ready_4p == 1){
        endgame *eg = malloc(sizeof(endgame));
        eg -> CODEREQ = htons(15);
        eg -> ID = cases[y*WIDTH + x] - 5;
        for (int i = 0; i < nb_players_4p; ++i) {
            ssize_t sent_bytes = send(*sockets[i], eg, sizeof(endgame), 0);
            if (sent_bytes < 0) {
                perror("Erreur lors de l'envoi du message");
            }
        }
    }else printf("Il reste %d joueurs\n", ready_4p);
}

void *bomb_thread(void *args){
    ar *a = (ar*) args;
    uint8_t *cases = a -> c;
    modifiedcase *casemodified = a -> casemodified;
    int x = a -> x;
    int y = a -> y;
    int *modified = a -> modified;
    int clientfd = a -> clientfd;
    // pthread_mutex_t lock = a -> lock;
    sleep(3);
    puts("EXPLOSION DE LA BOMBE");
    modifiedcase cm = {x, y, 4}; // case principale
    // pthread_mutex_lock(&lock);
    casemodified[*modified]= cm;
    cases[y*WIDTH+(x)] = 4;
    (*modified)++;
    if(cases[(y-1)*WIDTH + x]!=1){
        if(cases[(y-1)*WIDTH + x]!=2){
            modifiedcase cm2 = {x, y-2, 4}; // case 2 fois au-dessus
            casemodified[*modified]= cm2;
            if(cases[(y-2)*WIDTH + x]>4 && cases[(y-2)*WIDTH + x]<9) announce_kill(cases, x, y-2, clientfd);
            cases[(y-2)*WIDTH + x] = 4;
            (*modified)++;
        }
        modifiedcase cm3 = {x, y-1, 4}; // case juste au-dessus
        casemodified[*modified]= cm3;
        if(cases[(y-1)*WIDTH + x]>4 && cases[(y-1)*WIDTH + x]<9) announce_kill(cases, x, y-1, clientfd);
        cases[(y-1)*WIDTH + x] = 4;
        (*modified)++;
    }
    if(cases[(y+1)*WIDTH+x] != 1 ){
        if(cases[(y+1)*WIDTH + x]!=2){
            modifiedcase cm4 = {x, y+2, 4}; // case 2 fois en dessous
            casemodified[*modified]= cm4;
            if(cases[(y+2)*WIDTH + x]>4 && cases[(y+2)*WIDTH + x]<9) announce_kill(cases, x, y+2, clientfd);
            cases[(y+2)*WIDTH+x] = 4;
            (*modified)++;
        }
        modifiedcase cm5 = {x, y+1, 4}; // case juste en dessous
        casemodified[*modified]= cm5;
        if(cases[(y+1)*WIDTH + x]>4 && cases[(y+1)*WIDTH + x]<9) announce_kill(cases, x, y+1, clientfd);
        cases[(y+1)*WIDTH + x] = 4;
        (*modified)++;
        
    }
    if(cases[y*WIDTH+ x + 1] != 1 ){
        if(cases[y*WIDTH + x + 1]!=2){
            modifiedcase cm6 = {x + 2, y, 4}; // case 2 fois à droite
            casemodified[*modified]= cm6;
            if(cases[y*WIDTH+ x + 2]>4 && cases[y*WIDTH+ x + 2]<9) announce_kill(cases, x+2, y, clientfd);
            cases[y*WIDTH+ x + 2] = 4;
            (*modified)++;
        }
        modifiedcase cm7 = {x + 1, y, 4}; // case juste à droite
        casemodified[*modified]= cm7;
        if(cases[y*WIDTH+ x + 1]>4 && cases[y*WIDTH+ x + 1]<9) announce_kill(cases, x+1, y, clientfd);
        cases[y*WIDTH+ x + 1] = 4;
        (*modified)++;
        
    }if(cases[y*WIDTH+ x - 1] != 1 ){
        if(cases[y*WIDTH + x - 1]!=2){
            modifiedcase cm8 = {x - 2, y, 4}; // case 2 fois à gauche
            casemodified[*modified]= cm8;
            if(cases[y*WIDTH+ x - 2]>4 && cases[y*WIDTH+ x - 2]<9) announce_kill(cases, x-2, y, clientfd);
            cases[y*WIDTH+ x - 1] = 4;
            (*modified)++;
        }
        modifiedcase cm9 = {x - 1, y, 4};
        casemodified[*modified]= cm9; // case juste à gauche
        if(cases[y*WIDTH+ x - 1]>4 && cases[y*WIDTH+ x - 1]<9) announce_kill(cases, x-1, y, clientfd);
        cases[y*WIDTH+ x - 2] = 4;
        (*modified)++;
        
    }
    if(cases[(y-1)*WIDTH+(x-1)]!=1){
        modifiedcase cm10 = {x-1, y-1, 4};
            casemodified[*modified]= cm10; // case coin haut gauche
            if(cases[(y-1)*WIDTH+(x-1)]>4 && cases[(y-1)*WIDTH+(x-1)]<9) announce_kill(cases, x-1, y-1, clientfd);
            cases[(y-1)*WIDTH+(x-1)] = 4;
            (*modified)++;
    }if(cases[(y+1)*WIDTH+(x-1)]!=1){
        modifiedcase cm11 = {x-1, y+1, 4};
            casemodified[*modified]= cm11; // case coin bas gauche 
            if(cases[(y+1)*WIDTH+(x-1)]>4 && cases[(y+1)*WIDTH+(x-1)]<9) announce_kill(cases, x-1, y+1, clientfd);
            cases[(y+1)*WIDTH+(x-1)] = 4;
            (*modified)++;
    }if(cases[(y-1)*WIDTH+(x+1)]!=1){
        modifiedcase cm12 = {x+1, y-1, 4}; // case coin haut droite
            casemodified[*modified]= cm12;
            if(cases[(y-1)*WIDTH+(x+1)]>4 && cases[(y-1)*WIDTH+(x+1)]<9) announce_kill(cases, x+1, y-1, clientfd);
            cases[(y-1)*WIDTH+(x+1)] = 4;
            (*modified)++;
    }if(cases[(y+1)*WIDTH+(x+1)]!=1){
        modifiedcase cm13 = {x+1, y+1, 4}; //case coin bas droite
            casemodified[*modified]= cm13;
            if(cases[(y+1)*WIDTH+(x+1)]>4 && cases[(y+1)*WIDTH+(x+1)]<9) announce_kill(cases, x+1, y+1, clientfd);
            cases[(y+1)*WIDTH+(x+1)] = 4;
            (*modified)++;
    }
    // pthread_mutex_unlock(&lock);
    sleep(1);
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {          
            if(cases[i * WIDTH + j] == 4) cases[i * WIDTH + j] = 0;
        }
    }
    // pthread_mutex_destroy(&lock);
    pthread_exit(NULL);
}

void pose_bomb(uint8_t *cases, modifiedcase *casesmodified, int x, int y, int *modified, pthread_mutex_t lock, int clientfd){
    ar *a = malloc(sizeof(ar));
    a -> c = cases;
    a -> casemodified =  casesmodified;
    a -> x = x;
    a -> y = y;
    a -> modified = modified;
    a -> clientfd = clientfd;
    // a -> lock = lock;
    pthread_t thread;
    if (pthread_create(&thread, NULL, bomb_thread, a) != 0) {
        perror("Erreur lors de la création du thread");
    }
}