#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "ncurses.h"
#include <arpa/inet.h>  
#include <sys/types.h>  
#include <sys/socket.h> 
#include "globals_client.h"
#define W 22
#define H 10


void setup_board(board* board, int h, int w) {
    int lines, columns;
    getmaxyx(stdscr, lines, columns);
    board->h = h;
    board->w = w;
    board->grid = calloc((board->w) * (board->h), sizeof(char));
}

void free_board(board* board) {
    free(board->grid);
    free(board);
}

int get_grid(board* b, int x, int y) {
    return b->grid[y * b->w + x];
}

void set_grid(board* b, int x, int y, int v) {
    b->grid[y * b->w + x] = v;
}

void refresh_game(board* b, line* l) {
    // Initialize color pairs
    init_pair(0, COLOR_WHITE, COLOR_WHITE);
    init_pair(1, COLOR_YELLOW, COLOR_BLACK);
    init_pair(2, COLOR_RED, COLOR_BLACK);
    init_pair(3, COLOR_CYAN, COLOR_BLACK);
    init_pair(4, COLOR_GREEN, COLOR_BLACK);
    init_pair(5, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(6, COLOR_BLUE, COLOR_BLACK);

    int x, y;
    for (y = 0; y < b->h; y++) {
        for (x = 0; x < b->w; x++) {
            char c = ' ';
            int color_pair = -1;
            switch (get_grid(b, x, y)) {
                case 0:
                    c = ' ';
                    break;
                case 1:
                    c = 'X';
                    color_pair = 0;
                    break;
                case 2:
                    c = 'x';
                    color_pair = 1;
                    break;
                case 3:
                    c = 'B';
                    color_pair = 2;
                    break;
                case 4:
                    c = 'O';
                    break;
                case 5:
                    c = '1';
                    color_pair = 3;
                    break;
                case 6:
                    c = '2';
                    color_pair = 4;
                    break;
                case 7:
                    c = '3';
                    color_pair = 5;
                    break;
                case 8:
                    c = '4';
                    color_pair = 6;
                    break;
                default:
                    c = '?';
                    break;
            }

            if (color_pair >= 0) {
                attron(COLOR_PAIR(color_pair));
                mvaddch(y + 1, x + 1, c);
                attroff(COLOR_PAIR(color_pair));
            } else {
                mvaddch(y + 1, x + 1, c);
            }
        }
    }

    // Draw borders
    for (x = 0; x < b->w + 2; x++) {
        mvaddch(0, x, '-');
        mvaddch(b->h + 1, x, '-');
    }
    for (y = 0; y < b->h + 2; y++) {
        mvaddch(y, 0, '|');
        mvaddch(y, b->w + 1, '|');
    }

    // Draw the text box
    int text_box_start_y = b->h + 2;
    int text_box_start_x = 0;

    // Draw borders around the text box
    for (x = 0; x < 44; x++) {
        mvaddch(text_box_start_y, text_box_start_x + x, '-');
        mvaddch(text_box_start_y + 4 - 1, text_box_start_x + x, '-');
    }
    for (y = 0; y < 4; y++) {
        mvaddch(text_box_start_y + y, text_box_start_x, '|');
        mvaddch(text_box_start_y + y, text_box_start_x + 44 - 1, '|');
    }

    // Update chat text within the text box
    attron(COLOR_PAIR(1)); // Enable custom color 1
    attron(A_BOLD); // Enable bold

    // Display text line by line
    int line_length = 44 - 2; // Length of a line of text
    for (y = 1; y < 4 - 1; y++) {
        for (x = 1; x < 44 - 1; x++) {
            int text_index = (y - 1) * line_length + (x - 1);
            if (text_index >= TEXT_SIZE || text_index >= l->cursor)
                mvaddch(text_box_start_y + y, text_box_start_x + x, ' ');
            else
                mvaddch(text_box_start_y + y, text_box_start_x + x, l->data[text_index]);
        }
    }
    



    attroff(A_BOLD); // Disable bold
    attroff(COLOR_PAIR(1)); // Disable custom color 1

 
    // Draw the text box for ch to the right of the text box for l
    int ch_box_start_y = 0;
    int ch_box_start_x = text_box_start_x + W ; // 2 columns gap between boxes

    // Draw borders around the text box for ch
    for (x = 0; x < W; x++) {
        mvaddch(ch_box_start_y, ch_box_start_x + x, '-');
        mvaddch(ch_box_start_y + W - 1, ch_box_start_x + x, '-');
    }
    for (y = 0; y < W; y++) {
        mvaddch(ch_box_start_y + y, ch_box_start_x, '|');
        mvaddch(ch_box_start_y + y, ch_box_start_x + W - 1, '|');
    }

    // Update chat text within the text box for ch
    attron(COLOR_PAIR(2)); // Enable custom color 2
    attron(A_BOLD); // Enable bold

    // Display text line by line for ch
    line_length = W - 2; // Length of a line of text
    int current_x = 1, current_y = 1;

    for (int i = 0; i < strlen(ch -> data); i++) {
        if (ch -> data[i] == '\n') {
            current_y++;
            current_x = 1;
            if (current_y >= W - 1) {
                break; // Stop if we've reached the bottom of the box
            }
        } else {
            mvaddch(ch_box_start_y + current_y, ch_box_start_x + current_x, ch -> data[i]);
            current_x++;
            if (current_x >= W - 1) {
                current_x = 1;
                current_y++;
                if (current_y >= W - 1) {
                    break; // Stop if we've reached the bottom of the box
                }
            }
        }
        if (ch -> data[i] == '\0') {
            break; // Stop if end of text is reached
        }
    }

    attroff(A_BOLD); // Disable bold
    attroff(COLOR_PAIR(2)); // Disable custom color 2

    refresh(); // Apply the changes to the terminal
}


uint8_t control(line* l) {
    int c;
    int prev_c = ERR;
    // We consume all similar consecutive key presses
    while ((c = getch()) != ERR) { // getch returns the first key press in the queue
        if (prev_c != ERR && prev_c != c) {
            ungetch(c); // put 'c' back in the queue
            break;
        }
        prev_c = c;
    }
    uint8_t a = -1;
    switch (prev_c) {
        case ERR: break;
        case KEY_LEFT:
            a = 3; break;
        case KEY_RIGHT:
            a = 1; break;
        case KEY_UP:
            a = 0; break;
        case KEY_DOWN:
            a = 2; break;
        case KEY_F(1):
            a = 4; break;
        case KEY_F(2):
            a = 5; break;
        case KEY_F(3):
            a = 6; break;
        case KEY_BACKSPACE:
            if (l->cursor > 0) l->cursor--;
            break;
        case KEY_F(4):
            if (l->data == NULL) {
                fprintf(stderr, "Data is NULL\n");
                return a;
            }

            size_t data_len = strlen(l->data) + 1;  // +1 for the null terminator

            msg_toserv *msg = malloc(sizeof(msg_toserv));
            if (msg == NULL) {
                perror("Allocation failed");
                return a;
            }

            msg->CODEREQ = htons(7);
            msg->ID = id;
            msg->EQ = -1;
            msg->LEN = htonl(data_len);
            strncpy(msg->DATA, l->data, l->cursor);  // Ensure null-termination
            msg->DATA[l->cursor] = '\0';

            ssize_t sent_bytes = send(sockc_tcp, msg, sizeof(msg_toserv), 0);
            if (sent_bytes < 0) {
                perror("Erreur lors de l'envoi du message");
            }

            l->cursor = 0;
            free(msg);
            break;
        default:
            if (prev_c >= ' ' && prev_c <= '~' && l->cursor < TEXT_SIZE)
                l->data[(l->cursor)++] = prev_c;
            break;
    }
    return a;
}

bool perform_action(board* b, pos* p, uint8_t a) {
    int xd = 0, yd = 0;
    switch (a) {
        case 3:
            xd = -1; yd = 0; break;
        case 1:
            xd = 1; yd = 0; break;
        case 0:
            xd = 0; yd = -1; break;
        case 2:
            xd = 0; yd = 1; break;
        case 'p':
            return true;
        default: break;
    }
    p->x = (p->x + xd + b->w) % b->w;
    p->y = (p->y + yd + b->h) % b->h;
    set_grid(b, p->x, p->y, 1);
    return false;
}

void init_grill(board* b, line* l, pos* p, int h, int w) {
    // NOTE: All ncurses operations (getch, mvaddch, refresh, etc.) must be done on the same thread.
    initscr(); /* Start curses mode */
    raw(); /* Disable line buffering */
    intrflush(stdscr, FALSE); /* No need to flush when intr key is pressed */
    keypad(stdscr, TRUE); /* Required in order to get events from keyboard */
    nodelay(stdscr, TRUE); /* Make getch non-blocking */
    noecho(); /* Don't echo() while we do getch (we will manually print characters when relevant) */
    curs_set(0); // Set the cursor to invisible
    start_color(); // Enable colors
    init_pair(1, COLOR_YELLOW, COLOR_BLACK); // Define a new color style (text is yellow, background is black)
    setup_board(b, h, w);
}