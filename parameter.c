#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

void show_help_screen() {
  printf("asnake\nReleased under the GNU GPL.\n");
  printf("-s --speed=SPEED\tSet the speed of the snake, default is 5\n");
  exit(0);
}

void check_args(int argc, char *argv[]) {
    if(argc==1) {
        set_geschwindigkeit(5);
    }
    else {
	    show_help_screen();
      }
 
/*     
      else {
	printf("%c: Ungültige Option %c\n", argv[0], argv[1]);
	show_help_screen();
      }
    }
    
    else if(argc==3) {
      if(argv[1]=="-s") {
	    set_geschwindigkeit(atoi(argv[2]));
      }
      
      else {
	show_help_screen();
      }   
      
    } */
}
