#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  // für STDIN_FILENO
#include <pthread.h>

#include "parameter.h"
#include "game.h"
#include "move.h"
#include "display.h"
#include "dynamical_list.h"

struct game_t game;

struct parameter {
	char **array;
};

struct koordinaten specialpoint;

extern struct snake *anfang;
extern struct snake *next;
extern struct snake *ende;

int special_point_active;
// int sync_del_special_point;

char eingabe;

void *read_stdin() {
  while(0 == 0) {
    if (read (STDIN_FILENO, &eingabe, 1) < 1) {
      printf ("Fehler bei read\n");
      restore_tty (STDIN_FILENO);
      exit (EXIT_FAILURE);
    }
    usleep(game.geschwindigkeit);
  }
}


void snake_create_playground(char **playfield) {
  int counter; 
  int n,m;
    for(n=0; n < len_x-1; n++) {
        for( m=0; m < len_y-1; m++) {
            playfield[n][m]=' ';
        }
    } 
  
  for (counter=0;counter<len_x;counter++) {
    playfield[counter][0]='#';
    playfield[counter][len_y-1]='#';
  }
  
  for (counter=0;counter<len_y;counter++) {
    playfield[0][counter]='#';
    playfield[len_x-1][counter]='#';
  } 
        
}

void snake_print_out(char **playfield) {
  int j,i;
  for (i=0;i<len_y;i++) {
    for( j=0;j<len_x;j++) {
      printf("%c ", playfield[j][i]);
    }
    printf("\n");
  }
}

void create_snake(char **playfield) {
    int mittex = len_x / 2;
    int mittey = len_y / 2;

  anhaengen(mittex-2,mittey);
  anhaengen(mittex-1,mittey);
  anhaengen(mittex,mittey);
  anhaengen(mittex+1,mittey);
  
  struct snake *zeiger;
  
  zeiger=anfang;
  
  while( zeiger != NULL) {
    int x=zeiger->xachse;
    int y=zeiger->yachse;
    playfield[x][y]='@';
    zeiger=zeiger->next;
  } 
}

int zufallsauswahl(int minimum, int maximum) {
    int bereich = maximum - minimum + 1;
    int zahl = minimum + ( rand() % bereich );
    return zahl;
}

void special_point (pthread_t th1, pthread_t th2, char **playfield) {

    struct parameter *f;
    f = (struct parameter *)malloc(sizeof(struct parameter));
    if( f==NULL) {
        printf("Konnte keinen Speicher reservieren...!!!\n");
    }
    f->array=playfield;

    do  {
        specialpoint.x= zufallsauswahl(0,len_x-1);

        specialpoint.y= zufallsauswahl(0,len_y-1);

    } while ( playfield[specialpoint.x][specialpoint.y]=='@' \
	|| playfield[specialpoint.x][specialpoint.y]== '*');
  
  playfield[specialpoint.x][specialpoint.y]='$';
  special_point_active=1;
  if( pthread_create(&th2,NULL, &special_point_blink,f) != 0) {
      printf ("Konnte keinen Thread erzeugen\n");
    exit (EXIT_FAILURE);
  }   

  if (pthread_create (&th1,NULL,&del_special_point,f) != 0) {
    printf ("Konnte keinen Thread erzeugen\n");
    exit (EXIT_FAILURE);
  }  
}  

void *special_point_blink(void *param) {
    struct parameter *f = (struct parameter *)param;
 //   int sync_blinken=0;

  while(special_point_active==1 && sync_del_special_point==0 ) {
    if(f->array[specialpoint.x][specialpoint.y]=='$') {
      f->array[specialpoint.x][specialpoint.y]=' ';
    }
    else if(f->array[specialpoint.x][specialpoint.y]) {
      f->array[specialpoint.x][specialpoint.y]='$';
    }
    usleep(2*game.geschwindigkeit);
  }
    pthread_exit(NULL);
}

void *del_special_point(void *param) {    
  struct parameter *f = (struct parameter *)param;
  sync_del_special_point=0;

  struct snake *zeiger;
  zeiger=anfang; /* Zeiger auf 1. Element */
  while(zeiger->next != NULL)
    zeiger=zeiger->next;  
  
  int len=0;

  if(zeiger->xachse>specialpoint.x) {
    len+=(zeiger->xachse-specialpoint.x);
  }
  else if(zeiger->xachse<=specialpoint.x) {
    len+=(specialpoint.x-zeiger->xachse);
  }

  if(zeiger->yachse>specialpoint.y) {
    len+=(zeiger->yachse-specialpoint.y);
  }
  else if(zeiger->yachse<=specialpoint.y) {
    len+=(specialpoint.y-zeiger->yachse);
  }
  len+=10; 
  len*=game.geschwindigkeit;

  usleep(len);

  if (sync_del_special_point==0) {
    f->array[specialpoint.x][specialpoint.y]=' ';
    sync_del_special_point=1;
  }
  pthread_exit(NULL);
}
 

void game_over() {
  system("clear");
  printf("SCORE:\t\t%i\n\n", game.score);
  printf(" _______  _______  __   __  _______ \n");
  printf("|       ||   _   ||  |_|  ||       |\n");
  printf("|    ___||  |_|  ||       ||    ___|\n");
  printf("|   | __ |       ||       ||   |___ \n");
  printf("|   ||  ||       ||       ||    ___|\n");
  printf("|   |_| ||   _   || ||_|| ||   |___ \n");
  printf("|_______||__| |__||_|   |_||_______|\n");
  printf(" _______  __   __  _______  _______ \n");
  printf("|       ||  | |  ||       ||    _ | \n");
  printf("|   _   ||  |_|  ||    ___||   | || \n");
  printf("|  | |  ||       ||   |___ |   |_|| \n");
  printf("|  |_|  ||       ||    ___||    __ |\n");
  printf("|       | |     | |   |___ |   |  ||\n");
  printf("|_______|  |___|  |_______||___|  ||\n\n");
  printf("\n\n");
  restore_tty (STDIN_FILENO);
  exit(0);
}

void create_hindernis(char **playfield) {
  int x,y;
  do {
	  if(game.level>3) {
		  x=zufallsauswahl(2,len_x-3);
		  y=zufallsauswahl(2,len_y-3);
	  }
	  else {
		  x=zufallsauswahl(1,len_x-2);
		  y=zufallsauswahl(1,len_y-2);
	  }
  } while(playfield[x][y]=='@' || playfield[x][y]=='$' || x%2!=0 );
	 
  playfield[x][y]='*'; 
  hindernis_check=1;
}


