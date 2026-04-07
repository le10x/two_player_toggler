cmake_minimum_required(VERSION 3.21)

project(MyTestMod)

# Buscamos el SDK de Geode
find_package(Geode REQUIRED)

# Añadimos el mod con sus archivos fuente
add_geode_mod(
    ${PROJECT_NAME}
    SOURCES src/main.cpp
)
