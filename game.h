#ifndef _GAME_H
#define _GAME_H

#include <pthread.h>

struct game_t {
    int level;
    int geschwindigkeit;
    int score;
};

struct koordinaten {
  int x;
  int y;
};

extern struct koordinaten specialpoint;

extern struct game_t game;
extern char eingabe;
extern int sync_del_special_point;

void *read_stdin();
void snake_create_playground(char **playfield);
void snake_print_out(char **playfield);
void special_point( /*pthread_t th1, pthread_t th2,*/ char **playfield);
void *special_point_blink(void *param);
void *del_special_point(void *param);
void game_over();
void create_hindernis(char **playfield);
int zufallsauswahl(int minimum, int maximum);

#endif // _GAME_H
