project(KminiDLNA)
cmake_minimum_required(VERSION 2.6)
find_package(KDE4 REQUIRED)
find_package(Qt4 REQUIRED)

add_definitions(${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
include_directories(${KDE4_INCLUDES} ${QT_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR})

set(KminiDLNA_SRCS mainwidget.cpp KminiDLNA.cpp main.cpp)
set(KminiDLNA_RCCS resource.qrc)
# qt4_automoc(${KminiDLNA_SRCS})
qt4_add_resources(KminiDLNA_RCCS_SRCS ${KminiDLNA_RCCS})
kde4_add_executable(kminidlna ${KminiDLNA_SRCS} ${KminiDLNA_RCCS_SRCS})
target_link_libraries(kminidlna ${KDE4_KDEUI_LIBS})
