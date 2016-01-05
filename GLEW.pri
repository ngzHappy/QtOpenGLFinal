

DEFINES *= GLEW_STATIC
SOURCES += $$PWD/ZGLSource/glew.c###glew
SOURCES += $$PWD/ZGLSource/QGLWidgetInitializeGlew.cpp###init glew
SOURCES += $$PWD/ZGLSource/QOpenGLInitTool.cpp##init qglwidget

HEADERS += $$PWD/ZGL/QGLTool.hpp

INCLUDEPATH += $$PWD

win32-msvc*{
LIBS += -lopengl32 -lgdi32 -luser32 -lkernel32
}

win32-g++{
LIBS += -lopengl32 -lgdi32 -luser32 -lkernel32
}

linux:{
###libglu1-mesa-dev
###libgl1-mesa-dev
LIBS+=-lXmu -lXi -lGL -lXext -lX11
}




















