#ifndef REQUETES_FROM_SERV_H
#define REQUETES_FROM_SERV_H

#include <sys/socket.h>
#include <pthread.h>
#include "../client/ncurses.h"
#include "globals_serv.h"

extern int sock_tcp;
extern int sock_udp;

extern int nb_games_4p;
extern int nb_games_2v2;
extern int nb_players_4p;
extern int nb_players_2v2;
extern int ready_4p;
extern int ready_2v2;

extern void add_to_4j(int);
extern void add_to_2v2(int);

extern void player_ready_4j(int, uint8_t*, modifiedcase*, int*);
extern void player_ready_2v2(int, uint8_t*, modifiedcase*, int*);

extern void launch_game_4p(uint8_t*, modifiedcase*, int*);
extern void launch_game_2v2(uint8_t*, modifiedcase*, int*);

extern int action_game(u_int8_t*, uint8_t*, modifiedcase*, pos**, int);

extern void pose_bomb(uint8_t*, modifiedcase*, int, int, int*, pthread_mutex_t, int);

extern void send_message(uint8_t *, int);

#endif