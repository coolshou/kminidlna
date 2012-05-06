
#Macro for unit testing
MACRO(UNIT_TESTS)
    FOREACH(_className ${ARGN})
        set(_testName ${_className}test)
        kde4_add_unit_test(${_testName} TESTNAME kminidlna-${_testName} ${_testName}.cpp)
        target_link_libraries(${_testName} ${OPENSSL_LIBRARIES} server core view  ${KDE4_KDEUI_LIBS} ${KDE4_KIO_LIBS} ${QT_QTCORE_LIBRARY} ${QT_QTTEST_LIBRARY} ${QT_QTXML_LIBRARY} ${QT_QTXMLPATTERNS_LIBRARY})
    ENDFOREACH(_className)
ENDMACRO(UNIT_TESTS)

#add name of cpp file without ending test.cpp
unit_tests(configurationfile restconnection certificategenerator)



