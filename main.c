/* Ascii Snake 
 * Version: 2.0
 * Released under the GNU GPL
 * sevenonlinez
 */ 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>  // für usleep() 
#include <termios.h>  
#include <curses.h>  // gcc -lncurses
#include <pthread.h>  // gcc  -pthread
#include <term.h>

#include "game.h"
#include "display.h"
#include "parameter.h"
#include "move.h"
#include "dynamical_list.h"


int main(int argc, char *argv[]) {
    set_display_size();
    check_display_size();
    // set the default speed
    set_geschwindigkeit(5);
    
    int args_check = check_args(argc, argv);
    if (args_check == -1) {
        return -1;
    }

    // init playfield value

    char **playfield;  // pointer to pointer
    playfield=malloc(len_x * sizeof( char*)); 

    if(NULL== playfield) {
      printf("Kein virtueller RAM mehr vorhanden...!!");
      return EXIT_FAILURE;
    }
    int i;
    for( i=0; i< len_x; i++) {
        playfield[i] = malloc(len_y * sizeof(char));
            if( NULL == playfield[i]) {
            printf("Kein Speicher mehr fuer Zeile %d\n", i);
        }
    }

    // set new terminal mode 
    if (new_tty (STDIN_FILENO) == -1) {
        printf ("Fehler bei der Funktion new_tty()\n");
        exit (EXIT_FAILURE);
    }

    pthread_t th1=1, th2=2, th3=3;;

    if (pthread_create (&th1,NULL,&read_stdin,NULL) != 0) {
        printf ("Konnte keinen Thread erzeugen\n");
        exit (EXIT_FAILURE);
    }
    snake_create_playground(playfield); 
    create_snake(playfield);
    snake_print_out(playfield);

    int sync=1;   // sync for special_point 
    game.score=0;

  do {
    if((game.score/game.level)%5==0 && sync==0) {
      special_point(th2,th3,playfield);
      sync=1;
    }
    if((game.score/game.level)%5==1) {
      sync=0;
    }

    if (eingabe == 'p' || eingabe == 'P') {
        do {
            usleep(100);
        } while (eingabe == 'p' || eingabe == 'P');
    }

    system("clear"); 
    snake_print_out(playfield);
    usleep(game.geschwindigkeit);
    
    move_snake(playfield); 
    if(hindernis_check==0) {
      create_hindernis(playfield);
    } 
  } while (eingabe != 'q');
  
  game_over();
    
  return EXIT_SUCCESS;
}

