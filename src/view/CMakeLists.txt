# include_directories(${KDE4_INCLUDES} ${QT_INCLUDES} ${OPENSSL_INCLUDE_DIR} ${kminidlna_SOURCE_DIR} ${kminidlna_SOURCE_DIR}/server ${CMAKE_CURRENT_BINARY_DIR})

set(VIEW_SRCS generatecertificatedialog.cpp settingsmediafolder.cpp settingsconffile.cpp foldereditdialog.cpp mediafolderswidget.cpp settingsserver.cpp
	  abstractsettings.cpp
	  kminidlna.cpp
	  mainwidget.cpp
	  settingsdialog.cpp
	  settingsgeneral.cpp
	  settingsminidlna.cpp
	  
	 
      )
kde4_add_library(view ${VIEW_SRCS})

target_link_libraries(view server core ${OPENSSL_LIBRARIES})
