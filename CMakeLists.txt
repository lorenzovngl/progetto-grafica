cmake_minimum_required(VERSION 3.14)
project(ProgettoGrafica)

set(CMAKE_CXX_STANDARD 14)

add_executable(ProgettoGrafica
        project/lib/ShadowMapper/helper_functions.h
        project/lib/ShadowMapper/helper_functions.c
        project/lib/ShadowMapper/ShadowMapper.cpp
        project/lib/ShadowMapper/ShadowMapper.h
        project/src/headers/BoundingBox.h
        project/src/headers/Buoy.h
        project/src/headers/Camera.h
        project/src/headers/Controller.h
        project/src/headers/Enviroment.h
        project/src/headers/Fog.h
        project/src/headers/Frontier.h
        project/src/headers/Game.h
        project/lib/glm.h
        project/lib/GLText.h
        project/src/headers/HUD.h
        project/src/headers/Leaderboard.h
        project/src/headers/Mesh.h
        project/src/headers/Options.h
        project/src/headers/point3.h
        project/src/headers/ShaderParams.h
        project/src/headers/Ship.h
        project/src/headers/ShipMesh.h
        project/src/headers/Texture.h
        project/src/headers/TextureManager.h
        project/src/headers/Utils.h
        project/src/BoundingBox.cpp
        project/src/Buoy.cpp
        project/src/Camera.cpp
        project/src/Controller.cpp
        project/src/Enviroment.cpp
        project/src/Fog.cpp
        project/src/Frontier.cpp
        project/src/Game.cpp
        project/lib/glm.cpp
        project/lib/GLText.cpp
        project/src/HUD.cpp
        project/src/Leaderboard.cpp
        project/src/main.cpp
        project/src/Mesh.cpp
        project/src/Options.cpp
        project/src/ShaderParams.cpp
        project/src/Ship.cpp
        project/src/ShipMesh.cpp
        project/src/Texture.cpp
        project/src/TextureManager.cpp
        project/src/Utils.cpp
        )