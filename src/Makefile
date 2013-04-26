CC 	    = gcc
OBJECTS = display.o main.o move.o dynamical_list.o parameter.o game.o 
LIBS	= -pthread -lncurses

asnake:	$(OBJECTS)
	$(CC) -o $@ $(OBJECTS) $(LIBS)

main.o:             main.c
	$(CC) -c $*.c
display.o:          display.c display.h
	$(CC) -c $*.c
move.o:             move.c move.h
	$(CC) -c $*.c
dynamical_list.o:   dynamical_list.c dynamical_list.h
	$(CC) -c $*.c
parameter.o:        parameter.c parameter.h
	$(CC) -c $*.c 
game.o:             game.c game.h
	$(CC) -c $*.c

clean:
	rm *.o asnake
