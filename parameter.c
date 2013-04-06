#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

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
  printf("-s =SPEED\t Set the speed of the snake, default is 5\n");
  printf("-h\t\t Show this help screen\n");
  exit(0);
}

int check_args(int argc, char *argv[]) {
  int dflag = 0;  /* Debug ON/OFF */
  int hflag = 0;  /* show help screen */ 
  char *svalue = NULL;  /* set the speed of the snake */
  int index;
  int c;
  
  opterr = 0;

  while ((c = getopt (argc, argv, "dhs:")) != -1) 
    switch (c) {
    case 'd':
      dflag = 1;
      break;
    case 'h':
      hflag = 1;
      show_help_screen();
      break;
    case 's':
      svalue = optarg;
      set_geschwindigkeit(atoi(svalue));
      break;
    case '?':
      if (optopt == 's') {
        fprintf(stderr, "Option -%c requires an argument.\n", optopt);
      //  return 1;
      }
      else if (isprint (optopt)) {
        fprintf(stderr, "Unknown option '-%c'.\n", optopt);
      //  return 1;
      }
      else 
        fprintf(stderr, "Unknown option character '\\x%x'.\n", optopt);
      return -1;
    default:
      abort ();
    }

  // printf ("aflag = %d, bflag = %d, cvalue = %s\n", aflag, bflag, cvalue);

  for (index = optind; index < argc; index++) {
    printf("Non-option argument %s\n", argv[index]);
    return -1;
    }
}
