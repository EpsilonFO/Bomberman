#include <stdio.h>
#include "requetes_to_serv.h"
#include "../serveur/globals_serv.h"
#include "ncurses.h"
#include "globals_client.h"

void end_game_4j(uint8_t *buffer){
    endgame *eg = (endgame*)buffer;
    printf("%d a gagné la partie !\n", eg->ID);
    end();
}

void end_game_2v2(uint8_t *buffer){
    endgame *eg = (endgame*)buffer;
    printf("L'équipe %d a gagné la partie !\n", eg->EQ);
    end();
}