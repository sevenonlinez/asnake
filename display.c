#include <stdio.h>
#include <stdlib.h>
#include <curses.h>  // gcc -lncurses
#include <term.h>
#include <termios.h> 
#include "display.h"

int len_x;
int len_y;

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


void set_display_size() {
    len_x=get_cols();
    len_y=get_lines()-1;

    int xcor=len_x;
    
    if( xcor % 2 == 0) {
        xcor = len_x/2;
    }
    else {
        xcor = (len_x+1)/2;
        xcor--;    
    }
    len_x = xcor;
}

int check_display_size() {
  if(len_x<5 || len_y < 8) {
    printf("Zu kleine Terminalgrösse!!\n");
    return EXIT_FAILURE;
  }

  if(len_x>499 || len_y>998) {
    printf("To big terminal size!!\n");
    return EXIT_FAILURE;
  }
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
