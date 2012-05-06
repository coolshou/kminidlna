include_directories(${KDE4_INCLUDES} ${QT_INCLUDES} ${OPENSSL_INCLUDE_DIR} ${kminidlna_SOURCE_DIR} ${kminidlna_SOURCE_DIR}/src/server ${CMAKE_CURRENT_BINARY_DIR})
# add_subdirectory(interface)
set( SERVER_SRCS interface/restminidlnaput.cpp restconnection.cpp interface/restmediafolder.cpp interface/basicrestresource.cpp interface/restminidlna.cpp interface/restinterfaces.cpp interface/restresource.cpp
      restserver.cpp
      serverrequest.cpp
      certificategenerator.cpp
	)

kde4_add_library(server ${SERVER_SRCS})
target_link_libraries(server core view ${QT_QTXML_LIBRARY} ${QT_QTXMLPATTERNS_LIBRARY})

# IF(OPENSSL_FOUND)
#   include_directories(${OPENSSL_INCLUDE_DIR})
#   target_link_libraries(server ${OPENSSL_LIBRARIES})
# else(OPENSSL_FOUND)
#   message(FATAL_ERROR "No SSL")
# ENDIF(OPENSSL_FOUND)