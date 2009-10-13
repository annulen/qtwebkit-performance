TEMPLATE = app
TARGET = image_cycling
SOURCES = main.cpp ImageDecoderQt.cpp RGBA32BufferQt.cpp IntSizeQt.cpp IntRectQt.cpp IntPointQt.cpp
RESOURCES = images.qrc

INCLUDEPATH += WebCore/

QT += testlib
