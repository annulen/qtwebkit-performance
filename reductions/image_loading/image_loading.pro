TEMPLATE = app
TARGET = image_loading
SOURCES = main.cpp \
        ../shared/ImageDecoderQt.cpp ../shared/RGBA32BufferQt.cpp ../shared/IntSizeQt.cpp ../shared/IntRectQt.cpp ../shared/IntPointQt.cpp
RESOURCES = images.qrc

INCLUDEPATH += ../shared ../shared/WebCore/

QT += testlib
