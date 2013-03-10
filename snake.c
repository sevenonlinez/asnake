/* Ascii Snake 
 * Version: 0.4
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
#include "header.h"


int main (int argc, char *argv[]) {
  len_x=get_cols();
  len_y=get_lines()-1;
  if(len_x<10 || len_y < 8) {
    printf("Zu kleine Terminalgrösse!!\n");
    return EXIT_FAILURE;
  }

  if(len_x>998 || len_y>998) {
    printf("To big terminal size!!\n");
    return EXIT_FAILURE;
  }
  
  if(argc==1) {
    set_geschwindigkeit(5);
  }
  else if(argc==2){
    set_geschwindigkeit(atoi(argv[1]));
  }
  else {
    set_geschwindigkeit(5);
  }

  pthread_t th1=1;
  pthread_t th3,th4;

  if (pthread_create (&th1,NULL,&read_stdin,NULL) != 0) {
    printf ("Konnte keinen Thread erzeugen\n");
    exit (EXIT_FAILURE);
  }    
  
  /*Setzen des neuen Modus*/
  if (new_tty (STDIN_FILENO) == -1) {
    printf ("Fehler bei der Funktion new_tty()\n");
    exit (EXIT_FAILURE);
  }
  
  snake_create_playground(len_x,len_y);
  create_snake(len_x,len_y);
  
  int sync=1; // sync for special_point 
//  int sync_blinken=0;

  do {
    if((score/level)%4==0 && sync==0) {
      special_point(th3,th4);
      sync=1;
    }
    if((score/level)%4==1) {
      sync=0;
    }

    /* if(special_point_active==1) { */
    /*   switch(sync_blinken) { */
    /*   case 1:  */
    /* 	sync_blinken=0; */
    /* 	break; */
    /*   case 0: */
    /* 	sync_blinken=1; */
    /* 	break; */
    /*   } */
    /* } */

    system("clear"); 
    snake_print_out(len_x,len_y);
    usleep(geschwindigkeit);
    
    move_snake(eingabe); 
    if(hindernis_check==0) {
      create_hindernis();
    } 
  } while (eingabe != 'q');
  
  game_over();
    
  return EXIT_SUCCESS;
}


