#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../serveur/globals_serv.h"
#include "globals_client.h"
#include "requetes_to_serv.h"

void killed(uint8_t *buffer){
    endgame *eg = (endgame*)buffer;
    if(eg->ID == id){
        // endwin();
        close(sockc_udp);
        free_board(b); free(l); free(p);
        printf("Vous avez été tué par une bombe...\n");
        in_game = 0;
    }
}