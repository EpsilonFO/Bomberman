#ifndef GLOBALS_CLIENT_H
#define GLOBALS_CLIENT_H

#include <stdint.h>
#include "ncurses.h"

#define SERVER_ADDR "fe80::8b0:61bf:b730:ad2b"
#define SO_REUSE SO_REUSEADDR

extern int sockc_udp;
extern int sockc_tcp;
extern struct sockaddr_in6 src_addr;

extern board *b;
extern line *l;
extern line *ch;
extern pos *p;

extern int in_game;
extern int game_initialized;

extern int id;
extern int eq;
extern int num_msg;
extern int num_recv_server;
extern int port_udp;

extern void end();
extern void envoi_message(char *);

typedef struct {
    uint16_t CODEREQ;
    uint8_t ID;
    uint8_t EQ;
} typeGame;

typedef struct {
    uint16_t CODEREQ;
    uint8_t ID;
    uint8_t EQ;
} ready;

typedef struct {
    uint16_t CODEREQ;
    uint8_t ID;
    uint8_t EQ;
    uint16_t NUM;
    uint8_t ACTION;
} action;

typedef struct {
    uint16_t CODEREQ;
    uint8_t ID;
    uint8_t EQ;
    uint8_t LEN;
    char DATA[1024];
} msg_toserv;

#endif
