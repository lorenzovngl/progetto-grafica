
CPP  = g++
CC   = gcc
CFLAGS = -c -Wall
BIN  = open-sailing

# Library linking
OS := $(shell uname)
ifeq ($(OS),Darwin)
## caso Mac OS
$(info Mac OS detected)
FRMPATH=-F /Library/Frameworks
LIBS =  -framework OpenGL -framework SDL2 -framework SDL2_ttf -lm
$(info SDL2 libraries must be in: $(FRMPATH))
else
ifeq ($(OS),MINGW32_NT-6.1)
## caso Windows MinGW
$(info Windows MinGW detected)
FRMPATH = -IC:\MinGW\include
LIBS = -LC:\MinGW\lib -lmingw32 -lSDL2main -lSDL2 -lopengl32 -lglu32 -lSDL2_ttf -lGLEW -lglfw -lm
else
##caso Linux
$(info Linux detected)
#framework presenti solo nel caso MAC OS
FRMPATH =
LIBS = -lGL -lGLU -lglut -lSDL2 -lSDL2_image -lSDL2_ttf -lGLEW -lglfw -lm
endif
endif

OBJ = out/helper_functions.o out/ShadowMapper.o out/main.o out/BoundingBox.o out/Buoy.o out/Camera.o out/Controller.o out/Enviroment.o out/Fog.o out/Frontier.o out/Game.o out/HUD.o out/Leaderboard.o out/Mesh.o out/Options.o out/ShaderParams.o out/Ship.o out/ShipMesh.o out/Texture.o out/TextureManager.o out/Utils.o out/glm.o out/GLText.o 
LINKOBJ = out/helper_functions.o out/ShadowMapper.o out/main.o out/BoundingBox.o out/Buoy.o out/Camera.o out/Controller.o out/Enviroment.o out/Fog.o out/Frontier.o out/Game.o out/HUD.o out/Leaderboard.o out/Mesh.o out/Options.o out/ShaderParams.o out/Ship.o out/ShipMesh.o out/Texture.o out/TextureManager.o out/Utils.o out/glm.o out/GLText.o 

RM = rm -f

all: $(BIN)

clean:
	${RM} $(OBJ) $(BIN)

$(BIN): $(OBJ)
	$(CPP) $(LINKOBJ) -o $(BIN) $(FRMPATH) $(LIBS)

out/GCGraLib2.o: lib/GCGraLib2/GCGraLib2.c
	$(CC) $(CFLAGS) $(FRMPATH) lib/GCGraLib2/GCGraLib2.c -o lib/GCGraLib2/GCGraLib2.o

out/helper_functions.o: lib/ShadowMapper/helper_functions.h lib/ShadowMapper/helper_functions.c
	gcc $(CFLAGS) $(FRMPATH) lib/ShadowMapper/helper_functions.c -o out/helper_functions.o

out/ShadowMapper.o: lib/ShadowMapper/ShadowMapper.h lib/ShadowMapper/ShadowMapper.cpp
	$(CC) $(CFLAGS) $(FRMPATH) lib/ShadowMapper/ShadowMapper.cpp -o out/ShadowMapper.o

out/BoundingBox.o: src/headers/BoundingBox.h src/BoundingBox.cpp
	$(CPP) -c -Wno-deprecated $(FRMPATH) src/BoundingBox.cpp -o out/BoundingBox.o

out/Buoy.o: src/headers/Buoy.h src/Buoy.cpp
	$(CPP) -c -Wno-deprecated $(FRMPATH) src/Buoy.cpp -o out/Buoy.o

out/Camera.o: src/headers/Camera.h src/Camera.cpp
	$(CPP) -c -Wno-deprecated $(FRMPATH) src/Camera.cpp -o out/Camera.o

out/Controller.o: src/headers/Controller.h src/Controller.cpp
	$(CPP) -c -Wno-deprecated $(FRMPATH) src/Controller.cpp -o out/Controller.o

out/Enviroment.o: src/headers/Enviroment.h src/Enviroment.cpp
	$(CPP) -c -Wno-deprecated $(FRMPATH) src/Enviroment.cpp -o out/Enviroment.o

out/Fog.o: src/headers/Fog.h src/Fog.cpp
	$(CPP) -c -Wno-deprecated $(FRMPATH) src/Fog.cpp -o out/Fog.o

out/Frontier.o: src/headers/Frontier.h src/Frontier.cpp
	$(CPP) -c -Wno-deprecated $(FRMPATH) src/Frontier.cpp -o out/Frontier.o

out/Game.o: src/headers/Game.h src/Game.cpp
	$(CPP) -c -Wno-deprecated $(FRMPATH) src/Game.cpp -o out/Game.o

out/HUD.o: src/headers/HUD.h src/HUD.cpp
	$(CPP) -c -Wno-deprecated $(FRMPATH) src/HUD.cpp -o out/HUD.o

out/Leaderboard.o: src/headers/Leaderboard.h src/Leaderboard.cpp
	$(CPP) -c -Wno-deprecated $(FRMPATH) src/Leaderboard.cpp -o out/Leaderboard.o

out/Mesh.o: src/headers/Mesh.h src/Mesh.cpp
	$(CPP) -c -Wno-deprecated $(FRMPATH) src/Mesh.cpp -o out/Mesh.o

out/Options.o: src/headers/Options.h src/Options.cpp
	$(CPP) -c -Wno-deprecated $(FRMPATH) src/Options.cpp -o out/Options.o

out/ShaderParams.o: src/headers/ShaderParams.h src/ShaderParams.cpp
	$(CPP) -c -Wno-deprecated $(FRMPATH) src/ShaderParams.cpp -o out/ShaderParams.o

out/Ship.o: src/headers/Ship.h src/Ship.cpp
	$(CPP) -c -Wno-deprecated $(FRMPATH) src/Ship.cpp -o out/Ship.o

out/ShipMesh.o: src/headers/ShipMesh.h src/ShipMesh.cpp
	$(CPP) -c -Wno-deprecated $(FRMPATH) src/ShipMesh.cpp -o out/ShipMesh.o

out/Texture.o: src/headers/Texture.h src/Texture.cpp
	$(CPP) -c -Wno-deprecated $(FRMPATH) src/Texture.cpp -o out/Texture.o

out/TextureManager.o: src/headers/TextureManager.h src/TextureManager.cpp
	$(CPP) -c -Wno-deprecated $(FRMPATH) src/TextureManager.cpp -o out/TextureManager.o

out/Utils.o: src/headers/Utils.h src/Utils.cpp
	$(CPP) -c -Wno-deprecated $(FRMPATH) src/Utils.cpp -o out/Utils.o

out/glm.o: lib/glm.h lib/glm.cpp
	$(CPP) -c -Wno-deprecated $(FRMPATH) lib/glm.cpp -o out/glm.o

out/GLText.o: lib/GLText.h lib/GLText.cpp
	$(CPP) -c -Wno-deprecated $(FRMPATH) lib/GLText.cpp -o out/GLText.o

out/main.o: src/main.cpp
	$(CPP) -c -Wno-deprecated $(FRMPATH) src/main.cpp -o out/main.o

