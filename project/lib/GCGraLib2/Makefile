# -- makefile
#    if you  encounter the message: "Missing separator..."
#    make sure that all indented lines are indented by using a TAB
#    rather than blanks

CC = gcc
CFLAGS = -c -Wall
CDEB= -DDEBUG

# Library linking 
FRMPATH=-F /Library/Frameworks
#LDLIB = -L/usr/X11R6/lib -lX11 -lSDL2 -lSDL2_ttf -lSDL2_image -lm
LDLIB = -framework SDL2 -framework SDL2_ttf -framework SDL2_image -lm

# ---- Files --- 
GCGraLib2.o: GCGraLib2.c GCGraLib2.h
	$(CC) $(CFLAGS) $(FRMPATH) GCGraLib2.c -o $@

.c.o:
	$(CC) $(CFLAGS) *.c

clean:
	rm -f *.o *.*~ *~ $(GCGDIR)*.o $(BIN)
