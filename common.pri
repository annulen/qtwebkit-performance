# header files and dependencies
INCLUDEPATH += $$PWD/common
DEPENDPATH += $$PWD/common
QT += testlib \
    network \
    webkit \
    sql

# install target
isEmpty(INSTALL_DIR):INSTALL_DIR = $$[QT_INSTALL_BINS]
target.path += $$INSTALL_DIR/qtwebkit-benchmark
INSTALLS += target

INCLUDEPATH += $$PWD/common
debug_and_release: {
    CONFIG(debug, debug|release) QMAKE_LIBDIR += $$PWD/common/debug
    else: QMAKE_LIBDIR += $$PWD/common/release
} else:
    QMAKE_LIBDIR += $$PWD/common

!symbian: {
LIBS += -lcommon
}

symbian: {
    LIBS += -lcommon.lib -lhal -lesock -linsock

    TARGET.EPOCALLOWDLLDATA = 1
    TARGET.EPOCSTACKSIZE = 0x14000
    TARGET.EPOCHEAPSIZE = 0x20000 \
        0x10000000 \
        // \
        Min \
        128kB, \
        Max \
        256MB
    TARGET.CAPABILITY = NetworkServices LocalServices ReadUserData WriteUserData UserEnvironment
}

# Don't use Application bundles on Mac OS X
CONFIG -= app_bundle
