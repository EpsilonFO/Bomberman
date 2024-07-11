#include "ncurses.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "../serveur/globals_serv.h"
#include "globals_client.h"
#include "ncurses.h"

void case_modifier(uint8_t* buffer){
    mdiffmodif *mdm = (mdiffmodif*) buffer;
    // if(ntohs(mdm -> NUM) == num_recv_server){    
        for(int i = 0; i < mdm->NB; ++i){
            set_grid(b, mdm->CASES[i].line, mdm->CASES[i].column, mdm->CASES[i].content);
        }
        refresh_game(b, l);
        num_recv_server++;
    // }else{
    //     error *e = malloc(sizeof(error));
    //     e -> CODEREQ = htons(0);
    //     e -> CODEREQ = num_recv_server;
    //     send(sockc_tcp, e, sizeof(e), 0);
    // }
}