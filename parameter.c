#include <stdio.h>
#include "parameter.h"
#include "game.h"

struct game_t game;

void set_geschwindigkeit(int eingabe) {
  switch(eingabe) {
  case 1:
    game.geschwindigkeit=300000;
    game.level=1;
    break;
  case 2:
    game.geschwindigkeit=200000;
    game.level=2;
    break;
  case 3:
    game.geschwindigkeit=100000;
    game.level=3;
    break;
  case 4: 
    game.geschwindigkeit=80000;
    game.level=4;
    break;
  case 5:
    game.geschwindigkeit=70000;
    game.level=5;
    break;
  case 6:
    game.geschwindigkeit=60000;
    game.level=6;
    break;
  case 7:
    game.geschwindigkeit=50000;
    game.level=7;
    break;
  case 8:
    game.geschwindigkeit=40000;
    game.level=8;
    break;
  case 9:
    game.geschwindigkeit=30000;
    game.level=9;
    break;
  default:
    game.geschwindigkeit=80000;
    game.level=4;
    break;
  }
}


void check_args(int argc, char *argv[]) {
    if(argc==1) {
        set_geschwindigkeit(5);
    }
    else if(argc==2) {
        set_geschwindigkeit(atoi(argv[1]));
    }
    else {
        set_geschwindigkeit(5);
    }
}
