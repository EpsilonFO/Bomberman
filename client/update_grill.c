#include "ncurses.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include "../serveur/globals_serv.h"
#include "globals_client.h"

int game_initialized;
void update_grill(u_int8_t *buffer){
    mdiffgrille *mdg = (mdiffgrille*) buffer;
    
    if(!game_initialized){
        switch(mdg -> ID){
            case 0:
                p->x = 10;
                p->y = 0;
                break;
            case 1:
                p->x = mdg->LARGEUR - 1;
                p->y = 0;
                break;
            case 2:
                p->x = 0;
                p->y = mdg->HAUTEUR - 1;
                break;
            case 3:
                p->x = mdg->LARGEUR - 1;
                p->y = mdg->HAUTEUR - 1;
                break;
        }
        init_grill(b, l, p, mdg->HAUTEUR, mdg->LARGEUR);
        game_initialized = 1;
    }

    // if(ntohs(mdg -> NUM) == num_recv_server){
        for(int i = 0; i < mdg->HAUTEUR; i++){
            for(int j = 0; j < mdg->LARGEUR; j++){
                set_grid(b, j, i, mdg -> CASES[i * mdg->LARGEUR + j]);
            }
        }
        refresh_game(b, l);
        num_recv_server++;
    // }else {
    //     error *e = malloc(sizeof(error));
    //     e -> CODEREQ = htons(0);
    //     e -> CODEREQ = num_recv_server;
    //     send(sockc_tcp, e, sizeof(e), 0);
    // }
}