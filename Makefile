
CPP  = g++
CC   = gcc
CFLAGS = -c -Wall
BIN  = ship-game

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

OBJ = out/Buoy.o out/Camera.o out/Controller.o out/Enviroment.o out/main.o out/Mesh.o out/glm.o out/Ship.o out/ShipMesh.o out/Texture.o out/Utils.o 
LINKOBJ = out/Buoy.o out/Camera.o out/Controller.o out/Enviroment.o out/main.o out/Mesh.o out/glm.o out/Ship.o out/ShipMesh.o out/Texture.o out/Utils.o 

RM = rm -f

all: $(BIN)

clean:
	${RM} $(OBJ) $(BIN)

$(BIN): $(OBJ)
	$(CPP) $(LINKOBJ) -o $(BIN) $(FRMPATH) $(LIBS)

lib/GCGraLib2/GCGraLib2.o: lib/GCGraLib2/GCGraLib2.c
	$(CC) $(CFLAGS) $(FRMPATH) lib/GCGraLib2/GCGraLib2.c -o lib/GCGraLib2/GCGraLib2.o

out/Buoy.o: src/Buoy.cpp
	$(CPP) -c -Wno-deprecated $(FRMPATH) src/Buoy.cpp -o out/Buoy.o

out/Camera.o: src/Camera.cpp
	$(CPP) -c -Wno-deprecated $(FRMPATH) src/Camera.cpp -o out/Camera.o

out/Controller.o: src/Controller.cpp
	$(CPP) -c -Wno-deprecated $(FRMPATH) src/Controller.cpp -o out/Controller.o

out/Enviroment.o: src/Enviroment.cpp
	$(CPP) -c -Wno-deprecated $(FRMPATH) src/Enviroment.cpp -o out/Enviroment.o

out/main.o: src/main.cpp
	$(CPP) -c -Wno-deprecated $(FRMPATH) src/main.cpp -o out/main.o

out/Mesh.o: src/Mesh.cpp
	$(CPP) -c -Wno-deprecated $(FRMPATH) src/Mesh.cpp -o out/Mesh.o

out/glm.o: src/glm.cpp
	$(CPP) -c -Wno-deprecated $(FRMPATH) src/glm.cpp -o out/glm.o

out/Ship.o: src/Ship.cpp
	$(CPP) -c -Wno-deprecated $(FRMPATH) src/Ship.cpp -o out/Ship.o

out/ShipMesh.o: src/ShipMesh.cpp
	$(CPP) -c -Wno-deprecated $(FRMPATH) src/ShipMesh.cpp -o out/ShipMesh.o

out/Texture.o: src/Texture.cpp
	$(CPP) -c -Wno-deprecated $(FRMPATH) src/Texture.cpp -o out/Texture.o

out/Utils.o: src/Utils.cpp
	$(CPP) -c -Wno-deprecated $(FRMPATH) src/Utils.cpp -o out/Utils.o

