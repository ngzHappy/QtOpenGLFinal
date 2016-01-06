
CONFIG += c++14
CONFIG += console
QT += core gui
QT += widgets
QT += opengl

TARGET = ClearColor1
TEMPLATE = app


SOURCES += main.cpp
SOURCES += MainWindow.cpp

include( $$PWD/../GLEW.pri )

HEADERS  += MainWindow.hpp

DEFINES += GLSL_PWD=$$join( PWD,,\\\",\\\")

DEFINES += LOCAL_LANGUAGE=$$join( LOCAL_LANGUAGE_,,\\\",\\\")
DEFINES += GLSL_PWD=$$join( PWD,,\\\",\\\")



