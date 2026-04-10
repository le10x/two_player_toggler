cmake_minimum_required(VERSION 3.25)

# Esto fuerza la descarga del SDK si GitHub no lo inyecta correctamente
if (NOT Geode_FOUND)
    include(FetchContent)
    FetchContent_Declare(
        Geode
        GIT_REPOSITORY https://github.com
        GIT_TAG v5.5.3
    )
    FetchContent_MakeAvailable(Geode)
endif()

project(TestMod)

find_package(Geode REQUIRED)

add_geode_mod(${PROJECT_NAME} SOURCES src/main.cpp)
