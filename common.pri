# header files and dependencies
INCLUDEPATH += ../common
DEPENDPATH += ../common
QT += testlib network webkit

# install target
target.path += $$[QT_INSTALL_BINS]/benchmark
INSTALLS += target
