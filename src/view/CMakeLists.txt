include_directories(${KDE4_INCLUDES} ${QT_INCLUDES}  )

set( VIEW_SRCS settingsmediafolder.cpp settingsconffile.cpp foldereditdialog.cpp mediafolderswidget.cpp settingsserver.cpp
	  abstractsettings.cpp
	  kminidlna.cpp
	  mainwidget.cpp
	  settingsdialog.cpp
	  settingsgeneral.cpp
	  settingsminidlna.cpp
	  
	 
      )
kde4_add_library(view STATIC ${VIEW_SRCS} )

target_link_libraries(view core )
