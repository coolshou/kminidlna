# include_directories(${KDE4_INCLUDES} ${QT_INCLUDES} ${OPENSSL_INCLUDE_DIR} ${kminidlna_SOURCE_DIR} ${kminidlna_SOURCE_DIR}/src/server ${CMAKE_CURRENT_BINARY_DIR})
#${OPENSSL_INCLUDE_DIR})

set( CORE_SRCS pidthread.cpp
	configurationfile.cpp
	minidlna_process.cpp
	
	model/mediafolder.cpp
	)


kde4_add_library(core ${CORE_SRCS})

# target_link_libraries(core ${OPENSSL_LIBRARIES} ${OPENSSL_CRYPTO_LIBRARIES} server)
target_link_libraries(core server)
# add_subdirectory(model)