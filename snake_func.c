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

struct koordinaten arg; 


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

void snake_create_playground(char **playfield) {
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

void snake_print_out(char **playfield) {
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

void create_snake(char **playfield) {
  anhaengen(len_x/2-4,len_y/2-1);
  anhaengen(len_x/2-2,len_y/2-1);
  anhaengen(len_x/2,len_y/2-1);
  anhaengen(len_x/2+2,len_y/2-1);
  
  struct snake *zeiger;
  
  zeiger=anfang;
  
  while( zeiger != NULL) {
    int x=zeiger->xachse;
    int y=zeiger->yachse;
    playfield[x][y]='0';
    zeiger=zeiger->next;
  } 
}

void del_first(char **playfield) {  //Um das 1. Element der Liste zu löschen
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


