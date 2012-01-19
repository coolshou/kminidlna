include_directories(${KDE4_INCLUDES} ${QT_INCLUDES}  )

set( CORE_SRCS pidthread.cpp
	minidlna_configuration.cpp
	minidlna_process.cpp
	)


kde4_add_library(core STATIC ${CORE_SRCS} )

target_link_libraries(core server)