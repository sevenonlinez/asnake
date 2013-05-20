#include <stdio.h>
#include <stdlib.h>
#include "dynamical_list.h"
#include "game.h"

static int counter_special_point;

void snake_create_snake (char **playfield, struct point start_point, struct snake snake1) {

    if((snake1.tail = malloc(sizeof(struct snake_link))) == NULL) {
        fprintf(stderr, "Kein Speicherplatz vorhanden für tail\n");
        return;
    }
    snake1.tail->pos.x=start_point.x;
    snake1.tail->pos.y=start_point.y;
    snake1.tail->previous=NULL;

    playfield[snake1.tail->pos.x][snake1.tail->pos.y] = '@';

}

void snake_append_head_link(char **playfield, struct point p, struct snake snake1) {

    struct snake_link *zeiger, *zeiger1;

    zeiger=snake1.tail; /* Zeiger auf 1. Element */
    while(zeiger->previous != NULL)
        zeiger=zeiger->previous;


    /* Speicherplatz für das letzte
     * Element der Liste reservieren und anhängen.   */
    if((zeiger->previous =malloc(sizeof(struct snake_link))) == NULL) {
        fprintf(stderr,"Kein Speicherplatz für das "
                "letzte Element\n");
        return;
    }

    zeiger1=zeiger;

    zeiger=zeiger->previous; /* zeiger auf neuen Speicherplatz */

    zeiger->pos.x=p.x;
    zeiger->pos.y=p.y;

    zeiger->previous=NULL;
    snake1.tail=zeiger;
    zeiger->previous=zeiger1;
    zeiger1->previous=zeiger;
    while( zeiger != NULL) {
        int x=zeiger->pos.x;
        int y=zeiger->pos.y;
        playfield[x][y]='@';
        zeiger=zeiger->previous;
    }
}

void snake_del_tail_link(char **playfield, struct snake snake1) {
    struct snake_link *zeiger;
    zeiger=snake1.tail->previous;
    playfield[snake1.tail->pos.y][snake1.tail->pos.y]=' ';
    if(zeiger == NULL) {
        free(snake1.tail);
        snake1.tail=NULL;
        return;
    }
    zeiger->previous=NULL;
    free(snake1.tail);
    snake1.tail=zeiger;
}



void snake_move_snake(char **playfield, struct snake snake1) {
    struct snake_link *zeiger;

    zeiger=snake1.tail; /* Zeiger auf 1. Element */
    while(zeiger->previous != NULL)
        zeiger=zeiger->previous;

    int check=0;
    struct point vector;
    vector.x = snake1.movement.x - zeiger->pos.x;
    vector.y = snake1.movement.y - zeiger->pos.y;
    snake_append_head_link(playfield,vector, snake1);
    if(playfield[vector.x][vector.y]=='*') {
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

    if(playfield[vector.x][vector.y]=='$'\
            || zeiger->pos.y+snake1.movement.y == specialpoint.y \
            && specialpoint.x == zeiger->pos.x+snake1.movement.x ) {
        game.score=game.score+2*game.level+1;
        sync_del_special_point=1;
        special_point_active=0;
    }

    if(playfield[vector.x][vector.y]!='#' && playfield[vector.x][vector.y]!= '@') {
        playfield[vector.x][vector.y]='@';
    }
    else {
        game_over();
    }

    if(check==0) {
        snake_del_tail_link(playfield, snake1);
    }
}

