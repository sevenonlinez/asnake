#include <stdio.h>
#include <stdlib.h>
#include "snake.h"

int special_point_active=0;
struct koordinaten specialpoint;

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
    score=score+level;
  }
  if(playfield[zeiger->xachse+richtung][zeiger->yachse]=='$' || zeiger->xachse+richtung == specialpoint.x \
        && zeiger->yachse== specialpoint.y ) {
    score=score+3*level;
    sync_del_special_point=1;
    special_point_active=0;
  }

  if(playfield[zeiger->xachse+richtung][zeiger->yachse]!='#'		\
     && playfield[zeiger->xachse+richtung][zeiger->yachse]!= '0'  \
     && playfield[zeiger->xachse+richtung-1][zeiger->yachse]!='#') {
    playfield[zeiger->xachse+richtung][zeiger->yachse]='0';
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
    score=score+level;
  }
  
  if(playfield[zeiger->xachse][zeiger->yachse+richtung]=='$' || zeiger->yachse+richtung == specialpoint.y \
        && specialpoint.x == zeiger->xachse) {
    score=score+3*level;
    sync_del_special_point=1;
    special_point_active=0;
  }
    
  if(playfield[zeiger->xachse][zeiger->yachse+richtung]!='#'		\
     && playfield[zeiger->xachse][zeiger->yachse+richtung]!= '0') {
    playfield[zeiger->xachse][zeiger->yachse+richtung]='0';
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
      x_move(2,playfield);
    }
    else if(zeiger1->xachse<zeiger2->xachse) {
      x_move(-2,playfield);
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
      x_move(-2,playfield);
    }
    if(zeiger1->xachse>zeiger2->xachse) {
      x_move(2,playfield);
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
      x_move(-2, playfield);
    }
    if(zeiger1->xachse>zeiger2->xachse) {
      x_move(2, playfield);
    }
  }
}
