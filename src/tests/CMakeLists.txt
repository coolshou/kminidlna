
set(Tests configurationfiletest.cpp)
set(Tests_HEADER configurationfiletest.h)

qt4_wrap_cpp(Tests_HEADER_MOC ${Tests_HEADER})

add_executable(tests ${Tests} ${Tests_HEADER_MOC})

target_link_libraries(tests core core view server ${KDE4_KDEUI_LIBS} ${KDE4_KIO_LIBS} ${QT_QTCORE_LIBRARY} ${QT_QTTEST_LIBRARY})

add_test(NAME tests COMMAND tests)
