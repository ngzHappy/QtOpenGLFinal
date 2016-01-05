
CONFIG += c++14
QT += core gui
QT += widgets
QT += opengl

TARGET = Test0
TEMPLATE = app


SOURCES += main.cpp
SOURCES += MainWindow.cpp

include( $$PWD/../GLEW.pri )

HEADERS  += MainWindow.hpp


