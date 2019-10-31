cmake_minimum_required(VERSION 3.14)
project(ProgettoGrafica)

set(CMAKE_CXX_STANDARD 14)

add_executable(ProgettoGrafica
        src/headers/Buoy.h
        src/headers/Camera.h
        src/headers/Controller.h
        src/headers/Enviroment.h
        src/headers/glm.h
        src/headers/Mesh.h
        src/headers/point3.h
        src/headers/Ship.h
        src/headers/ShipMesh.h
        src/headers/Texture.h
        src/headers/Utils.h
        src/Buoy.cpp
        src/Camera.cpp
        src/Controller.cpp
        src/Enviroment.cpp
        src/glm.cpp
        src/main.cpp
        src/Mesh.cpp
        src/Ship.cpp
        src/ShipMesh.cpp
        src/Texture.cpp
        src/Utils.cpp
        )