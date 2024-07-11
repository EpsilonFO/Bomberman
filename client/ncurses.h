#ifndef NCURSES_H
#define NCURSES_H

#include <stdbool.h>
#include <stdint.h>

typedef enum ACTION { NONE, UP, DOWN, LEFT, RIGHT, QUIT } ACTION;

#define TEXT_SIZE 1024

typedef struct board {
    char* grid;
    int w;
    int h;
} board;

typedef struct line {
    char data[TEXT_SIZE];
    int cursor;
} line;

typedef struct pos {
    int x;
    int y;
} pos;

extern board *b;
extern line *l;
extern pos *p;

extern void setup_board(board*, int, int);
extern void free_board(board*);
extern int get_grid(board*, int, int);
extern void set_grid(board*, int, int, int);
extern void refresh_game(board*, line*);
extern uint8_t control(line*);
extern bool perform_action(board*, pos*, uint8_t);
extern void init_grill(board*, line*, pos*, int, int);

#endif