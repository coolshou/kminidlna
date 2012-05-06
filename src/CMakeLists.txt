cmake_minimum_required(VERSION 2.6)

project(kminidlna)

find_package(OpenSSL REQUIRED)
find_package(KDE4 REQUIRED)
find_package(Qt4 REQUIRED)


# add_subdirectory(po)
# add_subdirectory(src)
set(VERSION "0.2 beta")

# set(CMAKE_VERBOSE_MAKEFILE ON)
# set(CMAKE_INCLUDE_CURRENT_DIR ON)
# set(CMAKE_INCLUDE_DIRECTORIES_PROJECT_BEFORE ON)

add_definitions(${QT_DEFINITIONS} ${KDE4_DEFINITIONS})

include_directories(${KDE4_INCLUDES} ${QT_INCLUDES} ${OPENSSL_INCLUDE_DIR} ${kminidlna_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})


configure_file(kminidlna_version.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/kminidlna_version.h)

enable_testing() #enable testing



set(kminidlna_SRCS main.cpp)
set(kminidlna_RCCS resource.qrc)

kde4_add_app_icon(kminidlna_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/resource/kminidlna.png")
qt4_add_resources(kminidlna_RCCS_SRCS ${kminidlna_RCCS})
kde4_add_executable(kminidlna ${kminidlna_SRCS} ${kminidlna_RCCS_SRCS})
# MESSAGE(STATUS "OPENSSL_SSL_LIBRARIES :" "${OPENSSL_SSL_LIBRARIES}") 
# MESSAGE(STATUS "OPENSSL_CRYPTO_LIBRARIES :" "${OPENSSL_CRYPTO_LIBRARIES}")
# MESSAGE(STATUS "OPENSSL_LIBRARIES :" "${OPENSSL_LIBRARIES}")

target_link_libraries(kminidlna server core view) 
target_link_libraries(kminidlna ${OPENSSL_LIBRARIES} ${KDE4_KDEUI_LIBS} ${KDE4_KIO_LIBS} ${QT_QTCORE_LIBRARY} ${QT_QTNETWORK_LIBRARY} ${QT_QTXML_LIBRARY} ${QT_QTXMLPATTERNS_LIBRARY})

install(TARGETS kminidlna DESTINATION  ${BIN_INSTALL_DIR})
install(PROGRAMS kminidlna.desktop DESTINATION ${XDG_APPS_INSTALL_DIR})
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/resource/kminidlna.png DESTINATION ${ICON_INSTALL_DIR})
add_subdirectory(server)
add_subdirectory(view)
add_subdirectory(core)
add_subdirectory(tests)