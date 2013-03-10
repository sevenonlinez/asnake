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
char playfield[1000][1000];
int hindernis_check=0; /* bool 0: keine Hindernis generieren \
			* 1: Hindernis generieren */
int check;  /* bool 0: kein * getroffen, 1: * getroffen -> führe nicht \
	     * del(first) aus. */
char eingabe; // richtungseingabe
int geschwindigkeit=70000; 
int len_x=80;
int len_y=24;
int score;
int level;

int sync_del_special_point; /* bool 0: '$' wurde nich erwischt
			       1: '$' wurde erwischt, lösche pos($) nicht */

/* int sync_blinken; /\* synchronisiere special point blinken. *\/  */

// int special_point_active;  /* bool: 0: nicht active
//			      1: active  */

struct koordinaten {
  int x;
  int y;
};

struct koordinaten arg;


struct snake {
  int xachse;
  int yachse;
  struct snake *next;
  struct snake *previous;
};

struct snake *anfang=NULL;
struct snake *next = NULL;
struct snake *ende=NULL;

void anhaengen(int xachse, int yachse) {
  
  struct snake *zeiger, *zeiger1;
  
  /* Wurde schon Speicher für den ende-Zeiger bereitgestellt */
  if(ende == NULL) {
    if((ende = malloc(sizeof(struct snake))) == NULL) {
      printf("Konnte keinen Speicherplatz für ende reservieren\n");
      exit(0);
    }
  }
  
  
  
  if(anfang == NULL) {
    /* Reservieren von Speicherplatz für Struktur 
     * für das erste Element der Liste*/
    if((anfang = malloc(sizeof(struct snake))) == NULL) {
      fprintf(stderr, "Kein Speicherplatz vorhanden für anfang\n");
      return;
    }
    anfang->xachse=xachse;
    anfang->yachse=yachse;
    anfang->next=NULL;
    ende=anfang;
    ende->previous=NULL;
  }
  else {
    zeiger=anfang; /* Zeiger auf 1. Element */
    while(zeiger->next != NULL)
      zeiger=zeiger->next;
    
    
    /* Speicherplatz für das letzte
     * Element der Liste reservieren und anhängen.   */
    if((zeiger->next =malloc(sizeof(struct snake))) == NULL) {
      fprintf(stderr,"Kein Speicherplatz für das "
	      "letzte Element\n");
      return;
    } 
    
    zeiger1=zeiger;
    
    zeiger=zeiger->next; /* zeiger auf neuen Speicherplatz */
    
    zeiger->xachse=xachse;
    zeiger->yachse=yachse;
    
    zeiger->next=NULL;
    ende=zeiger;
    zeiger->previous=zeiger1;
    zeiger1->next=zeiger;
  }
}

void snake_create_playground() {
  int j=len_x;
  int i=len_y;
  int counter;
  
  int n,m;
  for( n=0;n<i;n++) {
    for( m=0;m<j;m++) {
      playfield[m][n]=' ';
    }
  }
  
  for (counter=0;counter<j;counter++) {
    playfield[counter][0]='#';
    playfield[counter][i-1]='#';
  }
  
  for (counter=0;counter<i;counter++) {
    playfield[0][counter]='#';
    playfield[j-1][counter]='#';
  }
  
}

void snake_print_out() {
  int l=len_x;
  int w=len_y;
  int j,i;
  for (i=0;i<w;i++) {
    for( j=0;j<l;j++) {
      printf("%c", playfield[j][i]);
    }
    
    printf("\n");
  }
}

void create_snake() {
  anhaengen(len_x/2-2,len_y/2-1);
  anhaengen(len_x/2-1,len_y/2-1);
  anhaengen(len_x/2,len_y/2-1);
  anhaengen(len_x/2+1,len_y/2-1);
  
  struct snake *zeiger;
  
  zeiger=anfang;
  
  while( zeiger != NULL) {
    int x=zeiger->xachse;
    int y=zeiger->yachse;
    playfield[x][y]='0';
    zeiger=zeiger->next;
  } 
}

void del_first() {  //Um das 1. Element der Liste zu löschen
  struct snake *zeiger;
  zeiger=anfang->next;
  playfield[anfang->xachse][anfang->yachse]=' ';
  if(zeiger == NULL) {
    free(anfang);
    anfang=NULL;
    ende=NULL;
    return;
  }
  zeiger->previous=NULL;
  free(anfang);
  anfang=zeiger;
  
}

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


void x_move(int richtung) {    // 0: Minux  1: Plus
  struct snake *zeiger;
  zeiger=anfang; /* Zeiger auf 1. Element */
  while(zeiger->next != NULL) {
    zeiger=zeiger->next;
  }
  check=0; //check if hindernis 
  anhaengen(zeiger->xachse+richtung,zeiger->yachse);
  // Ueberprüfen ob hindernis getroffen wurde
  if(playfield[zeiger->xachse+richtung][zeiger->yachse]=='*') {
    check=1;
    hindernis_check=0;
    score=score+level;
  }
  if(playfield[zeiger->xachse+richtung][zeiger->yachse]=='$') {
    score=score+3*level;
    sync_del_special_point=1;
//    special_point_active=0;
  }

  if(playfield[zeiger->xachse+richtung][zeiger->yachse]!='#'		\
     && playfield[zeiger->xachse+richtung][zeiger->yachse]!= '0') {
    playfield[zeiger->xachse+richtung][zeiger->yachse]='0';
  }
  else {
    game_over();
  }
  if(check==0) {
    del_first();
  }
  
} 

void y_move(int richtung) {    // 0: Minux  1: Plus
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
  
  if(playfield[zeiger->xachse][zeiger->yachse+richtung]=='$') {
    score=score+3*level;
    sync_del_special_point=1;
//    special_point_active=0;
  }
    
  if(playfield[zeiger->xachse][zeiger->yachse+richtung]!='#'		\
     && playfield[zeiger->xachse][zeiger->yachse+richtung]!= '0') {
    playfield[zeiger->xachse][zeiger->yachse+richtung]='0';
  }
  else {
    game_over();
  }
  
  if(check==0) {
    del_first();
  }
} 

void move_snake() {
  if(eingabe==100) {
    struct snake *zeiger1, *zeiger2;
    zeiger1=anfang; /* Zeiger auf 1. Element */
    while(zeiger1->next != NULL) {
      zeiger1=zeiger1->next;
    }
    zeiger2=zeiger1->previous;
    if( zeiger1->xachse>=zeiger2->xachse) {
      x_move(1);
    }
    else if(zeiger1->xachse<zeiger2->xachse) {
      x_move(-1);
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
      x_move(-1);
    }
    if(zeiger1->xachse>zeiger2->xachse) {
      x_move(1);
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
      y_move(-1);
    }
    if( zeiger1->yachse>zeiger2->yachse) {
      y_move(1);
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
      y_move(1);
    }
    if( zeiger1->yachse<zeiger2->yachse) {
      y_move(-1);
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
      y_move(1);
    }
    if( zeiger1->yachse<zeiger2->yachse) {
      y_move(-1);
    }
    if( zeiger1->xachse<zeiger2->xachse) {
      x_move(-1);
    }
    if(zeiger1->xachse>zeiger2->xachse) {
      x_move(1);
    }
  }
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

void create_hindernis() {
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

void *del_special_point(/*struct koordinaten *point*/) {
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
    playfield[arg.x][arg.y]=' ';
  }
	// special_point_active=0;
  pthread_exit(NULL);
}
 

void special_point( pthread_t th1, pthread_t th2) {
  /* int x,y; */
  // special_point_active=1;

  /* struct koordinaten arg; */
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

  if (pthread_create (&th1,NULL,&del_special_point,NULL) != 0) {
    printf ("Konnte keinen Thread erzeugen\n");
    exit (EXIT_FAILURE);
  }    
//  if (pthread_create (&th2,NULL,&special_point_blink,&arg) != 0) {
//    printf ("Konnte keinen Thread erzeugen\n");
//    exit (EXIT_FAILURE);
//  }
  
}
