#!/bin/bash

files=("Buoy" "Camera" "Controller" "Enviroment" "main" "Mesh" "glm" "Ship" "Texture" "Utils")

truncate -s 0 Makefile
# Open file descriptor (fd) 3 for read/write on a text file.
exec 3<> Makefile

printf '
CPP  = g++
CC   = gcc
CFLAGS = -c -Wall
BIN  = ship

# Library linking
OS := $(shell uname)
ifeq ($(OS),Darwin)
## caso Mac OS
$(info Mac OS detected)
FRMPATH=-F /Library/Frameworks
LIBS =  -framework OpenGL -framework SDL2 -lm
$(info SDL2 libraries must be in: $(FRMPATH))
else
ifeq ($(OS),MINGW32_NT-6.1)
## caso Windows MinGW
$(info Windows MinGW detected)
FRMPATH = -IC:\MinGW\include
LIBS = -LC:\MinGW\lib -lmingw32 -lSDL2main -lSDL2 -lopengl32 -lglu32 -lm
else
##caso Linux
$(info Linux detected)
#framework presenti solo nel caso MAC OS
FRMPATH =
LIBS = -lGL -lGLU -lglut -lSDL2 -lSDL2_image -lm
endif
endif

OBJ = ' >&3

for file in "${files[@]}"
do
  printf "out/${file}.o " >&3
done

printf '\nLINKOBJ = ' >&3

for file in "${files[@]}"
do
  printf "out/${file}.o " >&3
done

printf '

RM = rm -f

all: $(BIN)

clean:
	${RM} $(OBJ) $(BIN)

$(BIN): $(OBJ)
	$(CPP) $(LINKOBJ) -o $(BIN) $(FRMPATH) $(LIBS)

lib/GCGraLib2/GCGraLib2.o: lib/GCGraLib2/GCGraLib2.c
	$(CC) $(CFLAGS) $(FRMPATH) lib/GCGraLib2/GCGraLib2.c -o lib/GCGraLib2/GCGraLib2.o

' >&3

for file in "${files[@]}"
do
  printf "out/${file}.o: src/${file}.cpp\n\t\$(CPP) -c -Wno-deprecated \$(FRMPATH) src/${file}.cpp -o out/${file}.o\n\n" >&3
done

# Close fd 3
exec 3>&-