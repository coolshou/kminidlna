project(kminidlna)
cmake_minimum_required(VERSION 2.6)
find_package(KDE4 REQUIRED)
find_package(Qt4 REQUIRED)

add_subdirectory(po)
add_subdirectory(src)

# set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_INCLUDE_DIRECTORIES_PROJECT_BEFORE ON)

add_definitions(${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
include_directories(${KDE4_INCLUDES} ${QT_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR})

set(VERSION "0.19")