#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "game.h"
#include "parameter.h"
#include "dynamical_list.h"
#include "move.h"

int len_x, len_y;

int special_point_active;
static int check;
int hindernis_check;
int sync_del_special_point=0;
// char eingabe;


struct koordinaten specialpoint;

struct snake *anfang;
struct snake *next;
struct snake *ende;

void x_move(int richtung, char **playfield) {    // 0: Minux  1: Plus
  struct snake *zeiger;
  zeiger=anfang; /* Zeiger auf 1. Element */
  while(zeiger->next != NULL) {
    zeiger=zeiger->next;
  }
  if(zeiger->xachse+richtung >= len_x) {
    game_over();
  }

  check=0; //check if hindernis 
  anhaengen(zeiger->xachse+richtung,zeiger->yachse);
  // Ueberprüfen ob hindernis getroffen wurde
  if(playfield[zeiger->xachse+richtung][zeiger->yachse]=='*') {
    check=1;
    hindernis_check=0;
    game.score=game.score+game.level;
  }
  if(playfield[zeiger->xachse+richtung][zeiger->yachse]=='$' \
	|| zeiger->xachse+richtung == specialpoint.x \
	&& zeiger->yachse== specialpoint.y ) {
    game.score=game.score+3*game.level;
    sync_del_special_point=1;
    special_point_active=0;
  }

  if(playfield[zeiger->xachse+richtung][zeiger->yachse]!='#'		\
     && playfield[zeiger->xachse+richtung][zeiger->yachse]!= '@')  {
    playfield[zeiger->xachse+richtung][zeiger->yachse]='@';
  }
  else {
    game_over();
  }
  if(check==0) {
    del_first(playfield);
  }
  
} 

void y_move(int richtung, char **playfield) {    // 0: Minux  1: Plus
  struct snake *zeiger;
  zeiger=anfang; /* Zeiger auf 1. Element */
  while(zeiger->next != NULL) {
    zeiger=zeiger->next;
  }
  check=0;
  anhaengen(zeiger->xachse,zeiger->yachse+richtung);
  if(playfield[zeiger->xachse][zeiger->yachse+richtung]=='*') {
    check=1;
    hindernis_check=0;
    game.score=game.score+game.level;
  }
  
  if(playfield[zeiger->xachse][zeiger->yachse+richtung]=='$' \
	|| zeiger->yachse+richtung == specialpoint.y \
	&& specialpoint.x == zeiger->xachse) {
    game.score=game.score+3*game.level;
    sync_del_special_point=1;
    special_point_active=0;
  }
    
  if(playfield[zeiger->xachse][zeiger->yachse+richtung]!='#'		\
     && playfield[zeiger->xachse][zeiger->yachse+richtung]!= '@') {
    playfield[zeiger->xachse][zeiger->yachse+richtung]='@';
  }
  else {
    game_over();
  }
  
  if(check==0) {
    del_first(playfield);
  }
} 

void move_snake(char **playfield) {
  if(eingabe==100) {
    struct snake *zeiger1, *zeiger2;
    zeiger1=anfang; /* Zeiger auf 1. Element */
    while(zeiger1->next != NULL) {
      zeiger1=zeiger1->next;
    }
    zeiger2=zeiger1->previous;
    if( zeiger1->xachse>=zeiger2->xachse) {
      x_move(1,playfield);
    }
    else if(zeiger1->xachse<zeiger2->xachse) {
      x_move(-1,playfield);
    }
  }

  else if(eingabe==97) {
    struct snake *zeiger1, *zeiger2;
    zeiger1=anfang; /* Zeiger auf 1. Element */
    while(zeiger1->next != NULL) {
      zeiger1=zeiger1->next;
    }
    zeiger2=zeiger1->previous;
    if( zeiger1->xachse<=zeiger2->xachse) {
      x_move(-1,playfield);
    }
    if(zeiger1->xachse>zeiger2->xachse) {
      x_move(1,playfield);
    }
  }

  else if(eingabe==119) {
    struct snake *zeiger1, *zeiger2;
    zeiger1=anfang; /* Zeiger auf 1. Element */
    while(zeiger1->next != NULL) {
      zeiger1=zeiger1->next;
    }
    zeiger2=zeiger1->previous;
    if( zeiger1->yachse<=zeiger2->yachse) {
      y_move(-1,playfield);
    }
    if( zeiger1->yachse>zeiger2->yachse) {
      y_move(1,playfield);
    }
  }


  else if(eingabe==115) {
    struct snake *zeiger1, *zeiger2;
    zeiger1=anfang; /* Zeiger auf 1. Element */
    while(zeiger1->next != NULL) {
      zeiger1=zeiger1->next;
    }
    zeiger2=zeiger1->previous;
    if( zeiger1->yachse>=zeiger2->yachse) {
      y_move(1,playfield);
    }
    if( zeiger1->yachse<zeiger2->yachse) {
      y_move(-1,playfield);
    }
  }

  else { 
    struct snake *zeiger1, *zeiger2;
    zeiger1=anfang; /* Zeiger auf 1. Element */
    while(zeiger1->next != NULL) {
      zeiger1=zeiger1->next;
    }
    zeiger2=zeiger1->previous;
    if( zeiger1->yachse>zeiger2->yachse) {
      y_move(1,playfield);
    }
    if( zeiger1->yachse<zeiger2->yachse) {
      y_move(-1, playfield);
    }
    if( zeiger1->xachse<zeiger2->xachse) {
      x_move(-1, playfield);
    }
    if(zeiger1->xachse>zeiger2->xachse) {
      x_move(1, playfield);
    }
  }
}
