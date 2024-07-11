#ifndef GLOBALS_SERV_H
#define GLOBALS_SERV_H

#include <ncurses.h>

#define PORT_TCP 4000
#define PORT_UDP 5000
#define PORT_MDIFF_4J 10000
#define PORT_MDIFF_2V2 10001
#define ADDR_MDIFF_4J "ff12::1:2:3"
#define ADDR_MDIFF_2V2 "ff12::1:2:4"

#define MULTICAST "eth0"

#define NB_JOUEURS_4J 2
#define NB_JOUEURS_2V2 4

#define HEIGHT 20
#define WIDTH 20

extern int sock_tcp;
extern int sock_udp;

extern int **sockets;

extern struct sockaddr_in6 servaddr_tcp;
extern struct sockaddr_in6 servaddr_udp;
extern struct sockaddr_in6 multicastAddr_4j;
extern struct sockaddr_in6 multicastAddr_2v2;

extern int nb_games_4p;
extern int nb_games_2v2;

extern int nb_players_4p;
extern int nb_players_2v2;

extern int ready_4p;
extern int ready_2v2;

extern int close_thread;

extern void game(int, uint8_t*);

typedef struct {
    uint16_t CODEREQ;
    uint8_t ID;
    uint8_t EQ;
    uint16_t PORTUDP;
    uint16_t PORTMDIFF;
    uint8_t ADRMDIFF[16];
} gameSetup;

typedef struct {
    uint16_t CODEREQ;
    uint8_t ID;
    uint8_t EQ;
    uint16_t NUM;
    uint8_t HAUTEUR;
    uint8_t LARGEUR;
    uint8_t CASES[];
} mdiffgrille;

typedef struct {
    uint8_t line;
    uint8_t column;
    uint8_t content;
} modifiedcase;

typedef struct {
    uint16_t CODEREQ;
    uint8_t ID;
    uint8_t EQ;
    uint16_t NUM;
    uint8_t NB;
    modifiedcase CASES[];
} mdiffmodif;

typedef struct {
    uint16_t CODEREQ;
    uint8_t ID;
    uint8_t EQ;
    uint16_t LEN;
    char DATA[1024];
} msg_toclient;

typedef struct {
    uint16_t CODEREQ;
    uint8_t ID;
    uint8_t EQ;
} endgame;


typedef struct {
    int clientfd;
    uint8_t* cases;
} player;

#endif
