cmake_minimum_required(VERSION 3.25)

# No añades FetchContent aquí, la acción de GitHub ya lo maneja.
project(TestMod)

find_package(Geode REQUIRED)

# Tu archivo fuente debe estar en la carpeta src/
add_geode_mod(${PROJECT_NAME} SOURCES src/main.cpp)
