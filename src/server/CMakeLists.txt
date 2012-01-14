include_directories(${KDE4_INCLUDES} ${QT_INCLUDES}  )

set( SERVER_SRCS
      restserver.cpp
      serverrequest.cpp
	)

kde4_add_library(server STATIC ${SERVER_SRCS} )