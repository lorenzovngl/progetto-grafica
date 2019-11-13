cmake_minimum_required(VERSION 3.14)
project(ProgettoGrafica)

set(CMAKE_CXX_STANDARD 14)

add_executable(ProgettoGrafica
        src/headers/BoundingBox.h
        src/headers/Buoy.h
        src/headers/Camera.h
        src/headers/Controller.h
        src/headers/Enviroment.h
        src/headers/Game.h
        src/headers/glm.h
        src/headers/GLText.h
        src/headers/HUD.h
        src/headers/Mesh.h
        src/headers/point3.h
        src/headers/Ship.h
        src/headers/ShipMesh.h
        src/headers/Texture.h
        src/headers/Utils.h
        src/BoundingBox.cpp
        src/Buoy.cpp
        src/Camera.cpp
        src/Controller.cpp
        src/Enviroment.cpp
        src/Game.cpp
        src/glm.cpp
        src/GLText.cpp
        src/HUD.cpp
        src/main.cpp
        src/Mesh.cpp
        src/Ship.cpp
        src/ShipMesh.cpp
        src/Texture.cpp
        src/Utils.cpp
        )