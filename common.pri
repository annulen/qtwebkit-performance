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
SOURCES += $$PWD/common/common_init.cpp \
    $$PWD/common/benchmark.cc \
    $$PWD/common/databasenetworkaccessmanager.cpp \
    $$PWD/common/webpage.cpp
HEADERS += $$PWD/common/benchmark.h \
    $$PWD/common/common_init.h \
    $$PWD/common/cookiejar.h \
    $$PWD/common/databasenetworkaccessmanager.h \
    $$PWD/common/databasenetworkreply.h \
    $$PWD/common/databasetests.h \
    $$PWD/common/urlfilereader.h \
    $$PWD/common/webpage.h

symbian: {
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
    
    LIBS += -lhal
}
