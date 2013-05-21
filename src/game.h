#ifndef _GAME_H
#define _GAME_H

#include <pthread.h>
#include "dynamical_list.h"

struct game_t {
    int level;
    int geschwindigkeit;
    int score;
};

struct param {
    struct snake *snake1;
    struct snake *snake2;
};

extern struct point specialpoint;

extern struct game_t game;
extern char eingabe;
extern int sync_del_special_point;
extern char eingabe;


void *read_stdin(void *param);
void map_create_playground(char **playfield);
void snake_print_out(char **playfield);
void special_point(char **playfield, struct snake *snake1);
void *special_point_blink(void *param);
//void *del_special_point(void *param, struct snake snake1);
void game_over();
void create_hindernis(char **playfield);
int zufallsauswahl(int minimum, int maximum);
void *del_special_point(void *param);

#endif // _GAME_H
