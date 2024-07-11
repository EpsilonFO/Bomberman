#ifndef REQUETES_TO_SERV_H
#define REQUETES_TO_SERV_H

#include <stdint.h>

extern void choose_mode();
extern int *abo_multidiff(uint8_t*);
extern void announce_ready_4j(int, int);
extern void announce_ready_2v2(int, int);
extern void update_grill(uint8_t*);
extern void move_4j(uint8_t);
extern void case_modifier(uint8_t*);
extern void killed(uint8_t*);
extern void end_game_4j(uint8_t*);
extern void end_game_2v2(uint8_t*);

#endif