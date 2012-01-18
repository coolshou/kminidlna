include_directories(${KDE4_INCLUDES} ${QT_INCLUDES}  )

set( VIEW_SRCS settingsserver.cpp
	  abstractsettings.cpp
	  kminidlna.cpp
	  mainwidget.cpp
	  settingsdialog.cpp
	  settingsgeneral.cpp
	  settingsminidlna.cpp
	  mediadirmodel.cpp 
	  mediadirwidget.cpp
      )
kde4_add_library(view STATIC ${VIEW_SRCS} )

target_link_libraries(view core )