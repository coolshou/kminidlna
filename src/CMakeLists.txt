# cmake_minimum_required(VERSION 2.6)
include_directories(${KDE4_INCLUDES} ${QT_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR})
set(VERSION "0.2 beta")

configure_file(kminidlna_version.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/kminidlna_version.h )

# add_subdirectory(../po)
add_subdirectory(view)
add_subdirectory(server)
add_subdirectory(core)


set(kminidlna_SRCS main.cpp)
set(kminidlna_RCCS resource.qrc)

kde4_add_app_icon(kminidlna_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/resource/kminidlna.png")
qt4_add_resources(kminidlna_RCCS_SRCS ${kminidlna_RCCS})
kde4_add_executable(kminidlna ${kminidlna_SRCS} ${kminidlna_RCCS_SRCS})
target_link_libraries(kminidlna core view server ${KDE4_KDEUI_LIBS} ${KDE4_KIO_LIBS} ${QT_QTCORE_LIBRARY} ${QT_QTNETWORK_LIBRARY})

install(TARGETS kminidlna DESTINATION  ${BIN_INSTALL_DIR})
install(PROGRAMS kminidlna.desktop DESTINATION ${XDG_APPS_INSTALL_DIR})
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/resource/kminidlna.png DESTINATION ${ICON_INSTALL_DIR})
