# header files and dependencies
INCLUDEPATH += ../../common
DEPENDPATH += ../../common
QT += testlib network webkit

# install target
isEmpty(INSTALL_DIR): INSTALL_DIR = $$[QT_INSTALL_BINS]
target.path += $$INSTALL_DIR/qtwebkit-benchmark
INSTALLS += target

SOURCES += ../../common/benchmark.cc
