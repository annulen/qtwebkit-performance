TEMPLATE = app

INCLUDEPATH += ../common
DEPENDPATH += ../common

HEADERS += ../common/cookiejar.h
SOURCES += main.cpp ../common/cookiejar.cpp

QT += network sql webkit
