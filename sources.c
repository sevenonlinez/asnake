#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>  // für usleep() 
#include <termios.h>  
#include <curses.h>  // gcc -lncurses
#include <pthread.h>  // gcc  -pthread
#include <term.h>
#include "snake.h"

// Variabeln

int sync_del_special_point; /* bool 0: '$' wurde nich erwischt
			       1: '$' wurde erwischt, lösche pos($) nicht */

struct snake *anfang=NULL;
struct snake *next = NULL;
struct snake *ende=NULL;

struct parameter {
	char **array;
};




/*Altes Terminal wiederherstellen */
struct termios BACKUP_TTY;

/* Eingabekanal wird so umgeändert damit die Tasten einzeln */
/* abgefragt werden können                                  */
int new_tty (int fd) {
  struct termios buffer;
  
  /* Wir erfragen nach den Attributen des Terminals und */
  /* übergeben diese dann an buffer. BACKUP_TTY dient   */
  /* bei Programmende zur Wiederherstellung der alten   */
  /* Attribute und bleibt somit unberührt.              */
  if ((tcgetattr (fd, &BACKUP_TTY)) == -1)
    return -1;
  buffer = BACKUP_TTY;
  
  /* Lokale Flags werden gelöscht :                           */
  /* ECHO - Zeichenausgabe auf Bildschirm                     */
  /* ICANON - Zeilenorientierter Eingabemodus                 */
  /* ISIG - Terminal Steuerzeichen (kein STRG+C mehr möglich) */
  buffer.c_lflag = buffer.c_lflag & ~(ECHO | ICANON | ISIG);
  
  /* VMIN=Anzahl der Bytes die gelesen werden müssen, */
  /* bevor read() zurückkehrt                         */
  /* In unserem Beispiel 1Byte für 1 Zeichen          */
  buffer.c_cc[VMIN] = 1;
  
  /*Wir setzen jetzt die von uns gewünschten Attribute*/
  if ((tcsetattr (fd, TCSAFLUSH, &buffer)) == -1)
    return -1;
  return 0;
}


/*Ursprüngliches Terminal wiederherstellen*/
int restore_tty (int fd) {
  if ((tcsetattr (fd, TCSAFLUSH, &BACKUP_TTY)) == -1)
    return -1;
  return 0;
}



void *read_stdin() {
  while(1==1) {
    if (read (STDIN_FILENO, &eingabe, 1) < 1) {
      printf ("Fehler bei read\n");
      restore_tty (STDIN_FILENO);
      exit (EXIT_FAILURE);
    }
    usleep(geschwindigkeit);
  }
}

void game_over() {
  system("clear");
  printf("SCORE:\t\t%i\n\n", score);
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

int zufallsauswahl(int minimum, int maximum) { 
  int bereich = maximum - minimum + 1; 
  int zahl = minimum + ( rand() % bereich ); 
  return zahl;
}

void create_hindernis(char **playfield) {
  int x,y;
  do {
	  if(level>5) {
		  x=zufallsauswahl(2,len_x-3);
		  y=zufallsauswahl(2,len_y-3);
	  }
	  else {
		  x=zufallsauswahl(1,len_x-2);
		  y=zufallsauswahl(1,len_y-2);
	  }
  } while(playfield[x][y]=='0' || playfield[x][y]=='$' );
	 
  playfield[x][y]='*'; 
  hindernis_check=1;
}

int get_cols() {
  int ret;
  
  /* terminfor initialisieren */
  ret = setupterm (NULL, fileno (stdin), NULL);
  if (ret != 0) {
    perror ("setupterm()");
    exit (EXIT_FAILURE);
  }
  
  ret=tigetnum("cols");
  if (ret == -1) {
    printf ("get_cols: wird nicht unterstützt\n");
    return -1;
  }
  else
    return ret;
}

int get_lines() {
  int ret;
  
  /* terminfor initialisieren */
  ret = setupterm (NULL, fileno (stdin), NULL);
  if (ret != 0) {
    perror ("setupterm()");
    exit (EXIT_FAILURE);
  }
  
  ret=tigetnum("lines");
  if (ret == -1) {
    printf ("get_lines: wird nicht unterstützt\n");
    return -1;
  }
  else
    return ret;
}

void set_geschwindigkeit(int eingabe) {
  switch(eingabe) {
  case 1:
    geschwindigkeit=300000;
    level=1;
    break;
  case 2:
    geschwindigkeit=200000;
    level=2;
    break;
  case 3:
    geschwindigkeit=100000;
    level=3;
    break;
  case 4: 
    geschwindigkeit=80000;
    level=4;
    break;
  case 5:
    geschwindigkeit=70000;
    level=5;
    break;
  case 6:
    geschwindigkeit=60000;
    level=6;
    break;
  case 7:
    geschwindigkeit=50000;
    level=7;
    break;
  case 8:
    geschwindigkeit=40000;
    level=8;
    break;
  case 9:
    geschwindigkeit=30000;
    level=9;
    break;
  default:
    geschwindigkeit=80000;
    level=4;
    break;
  }
}
// ---------------------------------------------------------
/*
int special_point_blink(// struct koordinaten *point ) {
  int sync_blinken=0;

  while(special_point_active==1 && sync_del_special_point==0) {
    if(sync_blinken==1) {
      playfield[arg.x][arg.y]=' ';
    }
    else if(sync_blinken==0) {
      playfield[arg.x][arg.y]='$';
    }
    usleep(geschwindigkeit-1);
  }
  return 0;
}
*/
// -------------------------------------------------------------

void *del_special_point(void *param) {
  struct parameter *f = (struct parameter *)param;
  sync_del_special_point=0;

  struct snake *zeiger;
  zeiger=anfang; /* Zeiger auf 1. Element */
  while(zeiger->next != NULL)
    zeiger=zeiger->next;  
  
  int len=0;

  if(zeiger->xachse>arg.x) {
    len+=(zeiger->xachse-arg.x);
  }
  else if(zeiger->xachse<=arg.x) {
    len+=(arg.x-zeiger->xachse);
  }

  if(zeiger->yachse>arg.y) {
    len+=(zeiger->yachse-arg.y);
  }
  else if(zeiger->yachse<=arg.y) {
    len+=(arg.y-zeiger->yachse);
  }
  len+=10; 
  len*=geschwindigkeit;

  usleep(len);

  if(sync_del_special_point==0) {
    f->array[arg.x][arg.y]=' ';
  }
	// special_point_active=0;
  pthread_exit(NULL);
}
 

void special_point( pthread_t th1, pthread_t th2, char **playfield) {
  /* int x,y; */
  // special_point_active=1;

  struct parameter *f;
  f = (struct parameter *)malloc(sizeof(struct parameter));
  if( f==NULL) {
    printf("Konnte keinen Speicher reservieren...!!!\n");
  }
  f->array=playfield;

  do {
	  if(level>5) {
		  arg.x=zufallsauswahl(2,len_x-3);
		  arg.y=zufallsauswahl(2,len_y-3);
	  }
	  else {
		  arg.x=zufallsauswahl(1,len_x-2);
		  arg.y=zufallsauswahl(1,len_y-2);
	  }
  } while(playfield[arg.x][arg.y]=='0' || playfield[arg.x][arg.y]=='*');
  
  playfield[arg.x][arg.y]='$';

  if (pthread_create (&th1,NULL,&del_special_point,f) != 0) {
    printf ("Konnte keinen Thread erzeugen\n");
    exit (EXIT_FAILURE);
  }    
//  if (pthread_create (&th2,NULL,&special_point_blink,&arg) != 0) {
//    printf ("Konnte keinen Thread erzeugen\n");
//    exit (EXIT_FAILURE);
//  }
  
}
