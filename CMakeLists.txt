project(KminiDLNA)
cmake_minimum_required(VERSION 2.6)
find_package(KDE4 REQUIRED)
find_package(Qt4 REQUIRED)

add_definitions(${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
include_directories(${KDE4_INCLUDES} ${QT_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR})

set(KminiDLNA_SRCS settingsminidlna.cpp settingsgeneral.cpp settingsdialog.cpp minidlnaprocess.cpp mainwidget.cpp KminiDLNA.cpp main.cpp)
set(KminiDLNA_RCCS resource.qrc)
  # some .ui files
#   set( KminiDLNA_UIS
# 	kcfg_general.ui
# 	
#   )
# QT4_WRAP_UI( KminiDLNA_UI_HDRS ${KminiDLNA_UIS} )

# qt4_automoc(${KminiDLNA_SRCS})
kde4_add_app_icon(KminiDLNA_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/resource/icon.png")
qt4_add_resources(KminiDLNA_RCCS_SRCS ${KminiDLNA_RCCS})
# kde4_add_kcfg_files(KminiDLNA_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/kminidlnasettings.kcfgc")
kde4_add_executable(kminidlna ${KminiDLNA_SRCS} ${KminiDLNA_RCCS_SRCS})
target_link_libraries(kminidlna ${KDE4_KDEUI_LIBS})
