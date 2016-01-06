CONFIG += console
CONFIG += c++14
QT += core gui
QT += widgets
QT += opengl

TARGET = Texture2D
TEMPLATE = app

SOURCES += main.cpp
SOURCES += MainWindow.cpp

include( $$PWD/../GLEW.pri )

HEADERS  += MainWindow.hpp

DEFINES += LOCAL_LANGUAGE=$$join( LOCAL_LANGUAGE_,,\\\",\\\")
DEFINES += GLSL_PWD=$$join( PWD,,\\\",\\\")

DISTFILES += glsl/v.vert \
    glsl/f.frag


