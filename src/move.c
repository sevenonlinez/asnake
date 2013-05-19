#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "game.h"
#include "parameter.h"
#include "dynamical_list.h"
#include "move.h"

int len_x, len_y;

int special_point_active;
static int counter_special_point=0;
int hindernis_check;
int sync_del_special_point;


struct koordinaten specialpoint; 

struct snake *anfang;
struct snake *next;
struct snake *ende;


void move_snake(char **playfield) {
    struct snake *zeiger;
    zeiger=anfang; /* Zeiger auf 1. Element */
    while(zeiger->next != NULL) {
        zeiger=zeiger->next;
    }
    int check=0;
    int vector[2];
    vector[0] = incr[0] - zeiger->vector[0];
    vector[1] = incr[1] - zeiger->vector[1];

    anhaengen(vector);
    if(playfield[vector[0]][vector[1]]=='*') {
        check=1;

        if (counter_special_point >= 5) {
            special_point(playfield);
            counter_special_point=0;
        }
        else {
            counter_special_point++;
        }

        counter_special_point++;
        create_hindernis(playfield);
        game.score=game.score+game.level;
    }

    if(playfield[vector[0]][vector[1]]=='$'\
            || zeiger->vector[1]+incr[1] == specialpoint.y \
            && specialpoint.x == zeiger->vector[0]+incr[0]  ) {
        game.score=game.score+2*game.level+1;
        sync_del_special_point=1;
        special_point_active=0;
    }

    if(playfield[vector[0]][vector[1]]!='#' && playfield[vector[0]][vector[1]]!= '@') {
        playfield[vector[0]][vector[1]]='@';
    }
    else {
        game_over();
    }

    if(check==0) {
        del_first(playfield);
    }
}
