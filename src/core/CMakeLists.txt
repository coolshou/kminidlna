include_directories(${KDE4_INCLUDES} ${QT_INCLUDES}  )

set( CORE_SRCS pidthread.cpp
	configurationfile.cpp
	minidlna_process.cpp
	
	model/mediafolder.cpp
	)


kde4_add_library(core STATIC ${CORE_SRCS} )

target_link_libraries(core server)
# add_subdirectory(model)