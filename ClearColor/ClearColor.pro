CONFIG += c++14
QT += core gui
QT += widgets
QT += opengl

TARGET = ClearColor
TEMPLATE = app


SOURCES += main.cpp
SOURCES += MainWindow.cpp

include( $$PWD/../GLEW.pri )

HEADERS  += MainWindow.hpp


