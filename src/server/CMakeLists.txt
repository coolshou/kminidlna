include_directories(${KDE4_INCLUDES} ${QT_INCLUDES}  )

set( SERVER_SRCS restconnection.cpp interface/restmediafolder.cpp interface/basicrestresource.cpp interface/restminidlna.cpp interface/restinterfaces.cpp interface/restresource.cpp
      restserver.cpp
      serverrequest.cpp
	)

kde4_add_library(server STATIC ${SERVER_SRCS} )
target_link_libraries(server ${QT_QTXML_LIBRARY} ${QT_QTXMLPATTERNS_LIBRARY})
add_subdirectory(interface)