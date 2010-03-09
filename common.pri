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
LIBPATH += $$PWD/common

!symbian: {
LIBS += -lcommon
}

symbian: {
    LIBS += -lcommon.lib -lhal

    TARGET.EPOCALLOWDLLDATA = 1
    TARGET.EPOCSTACKSIZE = 0x14000
    TARGET.EPOCHEAPSIZE = 0x20000 \
        0x2000000 \
        // \
        Min \
        128kB, \
        Max \
        32MB
    TARGET.CAPABILITY = All -TCB -DRM -AllFiles 
}

# Don't use Application bundles on Mac OS X
CONFIG -= app_bundle
