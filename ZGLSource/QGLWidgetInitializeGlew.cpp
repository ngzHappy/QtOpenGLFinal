#include <ZGL/QGLTool.hpp>
#include <QMessageBox>
#include <QGLWidget>

extern void _i_qInitializeQGLWidget(QGLWidget *glWidget);

namespace{
static inline bool __QGLWidgetInitializeGlew(
        QGLWidget * const ptr
        )
{
    _i_qInitializeQGLWidget(ptr);
    ptr->makeCurrent();
    glewExperimental = GL_TRUE;
    int glewErr = glewInit();
    if( glewErr != GLEW_OK )
    {
        QMessageBox box;
        box.setText("opengl/glew init error!");
        box.exec();
        return false;
    }//
    return true;
}
}

bool qGLWidgetInitializeGlew(QGLWidget * const ptr){
    return __QGLWidgetInitializeGlew(ptr);
}
