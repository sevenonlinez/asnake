#ifndef _DISPLAY_H_
#define _DISPLAY_H_

extern int len_x;
extern int len_y;
extern struct termios BACKUP_TTY;

int get_cols();
int get_lines();
void set_display_size();
int check_display_size();
int new_tty (int fd);
int restore_tty (int fd);

#endif // _DISPLAY_H_
